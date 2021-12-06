#pragma once

#include "./any_iterator_fwd.hpp"

#include "./assert.hpp"
#include "./iterator.hpp"
#include "./iterator_facade.hpp"
#include "./range_concepts.hpp"
#include "./tag.hpp"

#include <iterator>
#include <memory>

namespace neo {

class any_iterator_base {
    virtual neo::type_tag do_type_tag() const noexcept  = 0;
    virtual const void*   do_cvoid_ptr() const noexcept = 0;

public:
    neo::type_tag type_tag() const noexcept { return do_type_tag(); }

    template <typename T>
    const T& get() const noexcept {
        neo_assert(expects,
                   type_tag() == type_tag_v<T>,
                   "Invalid any_iterator::get<T>() of incorrect 'T'");
        return *static_cast<const T*>(do_cvoid_ptr());
    }
};

namespace iter_detail {

/**
 * @brief Base class of all erased iterators
 */
class erased_iface_base {
private:
    virtual void*         do_void_ptr() noexcept       = 0;
    virtual neo::type_tag do_type_tag() const noexcept = 0;

public:
    virtual ~erased_iface_base() = default;
    /**
     * @brief Get a tag identifying the actually underlying type of the object
     */
    neo::type_tag type_tag() const noexcept { return do_type_tag(); }

    void*       void_ptr() noexcept { return do_void_ptr(); }
    const void* void_ptr() const noexcept {
        return const_cast<erased_iface_base*>(this)->do_void_ptr();
    }

    /**
     * @brief Obtain a reference to the erased object, which MUST be of type T
     */
    template <typename T>
    T& get() noexcept {
        neo_assert(expects,
                   type_tag() == type_tag_v<T>,
                   "erased_iface_base::get() was given the wrong type");
        return *static_cast<T*>(do_void_ptr());
    }

    template <typename T>
    const T& get() const noexcept {
        return const_cast<erased_iface_base*>(this)->get<T>();
    }
};

/**
 * @brief Partially applied callable object that compares two common iterators
 */
template <std::forward_iterator Iter>
struct common_erased_sentinel_compare_parts {
    Iter           iter;
    constexpr bool operator()(const any_iterator_base& other) const noexcept {
        return iter == other.get<Iter>();
    }
};

/**
 * @brief Base class of erased range sentinels
 */
class erased_sentinel_parts {
    virtual bool do_compare_equal(const any_iterator_base& iter) const noexcept = 0;

public:
    virtual ~erased_sentinel_parts() = default;

    bool compare_equal(const any_iterator_base& iter) const noexcept {
        return do_compare_equal(iter);
    }
};

/**
 * @brief Implementation of erased sentinels.
 *
 * An erased sentinel contains an invocable object accepting an
 * `erased_iface_base`. The invocable should return a boolean indicating
 * whether the given iterator is at the end of the range. Prefer any_sentinel
 * for most cases.
 *
 * Use this class with CTAD.
 */
template <invocable<const any_iterator_base&> Func>
class erase_sentinel : public erased_sentinel_parts {
    Func _fn;

    bool do_compare_equal(const any_iterator_base& iter) const noexcept override {
        return _fn(iter);
    }

public:
    erase_sentinel() = default;

    explicit erase_sentinel(Func&& fn)
        : _fn(NEO_FWD(fn)) {}

    template <std::forward_iterator Iter>
    explicit erase_sentinel(const Iter& it)
        : _fn{it} {}
};

/**
 * @brief Abstract base class types for type-erased iterator classes.
 *
 * @tparam Category The category of the erased iterator. Determines which interfaces are available
 * @tparam Ref The reference type of the iterator (returned by operator*)
 *
 * For any 'tag', the `erased_iface<tag, R>` will inherit from the
 * `erased_iface<PT, R>` where 'PT' is the next iterator in the heirarchy.
 *
 * (e.g. an erased_iface<random_access_iterator_tag> inherits from
 * erased_iface<bidirectional_iterator_tag>.)
 *
 * This inheritance allows binding references to more restricted iterators to
 * names of more capable iterators (i.e. you can bind a ref to a forward iterator to a
 * random access iterator, but not the other way around.)
 *
 * Each `erased_iface` in the chain declares pure virtuals of the methods that can be given
 * to `iterator_facade`.
 */
template <typename Category, typename Ref>
struct erased_iface;

/**
 * @brief Implementation class for the corresponding erased_iface
 *
 * @tparam Category The part that is implemented by this class
 * @tparam Ref The reference type of the interface
 * @tparam Iter The actual type of iterator being erased
 * @tparam Interface The actual base class interface. This might be different from the erased_iface
 * of 'Category', since we may ourselves inherit from a more-expressive category
 */
template <typename Category, typename Ref, typename Iter, typename Interface>
struct erased_impl;

// Base impl for iterator storage and dereference
template <typename Ref, typename Iter, typename Interface>
struct erased_impl<void, Ref, Iter, Interface> : Interface {
private:
    Iter _iter;

    void*         do_void_ptr() noexcept override { return std::addressof(_iter); }
    neo::type_tag do_type_tag() const noexcept override { return type_tag_v<Iter>; }

public:
    using void_part        = erased_impl;
    erased_impl() noexcept = default;

    explicit erased_impl(Iter it) noexcept
        : _iter(it) {}
};

/*
#### ##    ## ########  ##     ## ########
 ##  ###   ## ##     ## ##     ##    ##
 ##  ####  ## ##     ## ##     ##    ##
 ##  ## ## ## ########  ##     ##    ##
 ##  ##  #### ##        ##     ##    ##
 ##  ##   ### ##        ##     ##    ##
#### ##    ## ##         #######     ##
*/

/// Specialized for input iterators
template <typename Ref>
struct erased_iface<std::input_iterator_tag, Ref> : erased_iface_base {
    enum { single_pass_iterator = true };

    // Every impl will have its own clone()
    std::unique_ptr<erased_iface> clone() const noexcept {
        return std::unique_ptr<erased_iface>(do_clone());
    }

    void increment() { do_increment(); }
    Ref  dereference() const { return do_dereference(); }

private:
    // input iterators are requires to be incrementable
    virtual void do_increment() = 0;
    // And dereferencable
    virtual Ref do_dereference() const = 0;

    // The private virtual that will copy the erased iterator
    virtual erased_iface* do_clone() const noexcept = 0;
};

template <typename Ref, typename Iter, typename Interface>
struct erased_impl<std::input_iterator_tag, Ref, Iter, Interface>
    : erased_impl<void, Ref, Iter, Interface> {
    using input_part = erased_impl;
    using input_part::void_part::void_part;

    void do_increment() override { ++this->template get<Iter>(); }
    Ref  do_dereference() const override { return *this->template get<Iter>(); }
};

/*
########  #######  ########  ##      ##    ###    ########  ########
##       ##     ## ##     ## ##  ##  ##   ## ##   ##     ## ##     ##
##       ##     ## ##     ## ##  ##  ##  ##   ##  ##     ## ##     ##
######   ##     ## ########  ##  ##  ## ##     ## ########  ##     ##
##       ##     ## ##   ##   ##  ##  ## ######### ##   ##   ##     ##
##       ##     ## ##    ##  ##  ##  ## ##     ## ##    ##  ##     ##
##        #######  ##     ##  ###  ###  ##     ## ##     ## ########
*/

template <typename Ref>
struct erased_iface<std::forward_iterator_tag, Ref> : erased_iface<std::input_iterator_tag, Ref> {
    enum { single_pass_iterator = false };

    std::unique_ptr<erased_iface> clone() const noexcept {
        return std::unique_ptr<erased_iface>(do_clone());
    }

    friend bool operator==(const erased_iface& left, const erased_iface& right) noexcept {
        return left.do_compare_equal(right);
    }

private:
    virtual erased_iface* do_clone() const noexcept                                  = 0;
    virtual bool          do_compare_equal(const erased_iface& right) const noexcept = 0;
};

template <typename Ref, typename Iter, typename Interface>
struct erased_impl<std::forward_iterator_tag, Ref, Iter, Interface>
    : erased_impl<std::input_iterator_tag, Ref, Iter, Interface> {
    using forward_part = erased_impl;
    using forward_part::input_part::input_part;

private:
    bool do_compare_equal(
        const erased_iface<std::forward_iterator_tag, Ref>& other) const noexcept override {
        return this->type_tag() == other.type_tag()
            && this->template get<Iter>() == other.template get<Iter>();
    }
};

/*
########  #### ########  #### ########
##     ##  ##  ##     ##  ##  ##     ##
##     ##  ##  ##     ##  ##  ##     ##
########   ##  ##     ##  ##  ########
##     ##  ##  ##     ##  ##  ##   ##
##     ##  ##  ##     ##  ##  ##    ##
########  #### ########  #### ##     ##
*/

template <typename Ref>
struct erased_iface<std::bidirectional_iterator_tag, Ref>
    : erased_iface<std::forward_iterator_tag, Ref> {
    std::unique_ptr<erased_iface> clone() const noexcept {
        return std::unique_ptr<erased_iface>(do_clone());
    }

    void decrement() noexcept { do_decrement(); }

private:
    virtual erased_iface* do_clone() const noexcept = 0;
    virtual void          do_decrement() noexcept   = 0;
};

template <typename Ref, typename Iter, typename Interface>
struct erased_impl<std::bidirectional_iterator_tag, Ref, Iter, Interface>
    : erased_impl<std::forward_iterator_tag, Ref, Iter, Interface> {
    using bidirectional_part = erased_impl;
    using bidirectional_part::forward_part::forward_part;

private:
    void do_decrement() noexcept override { --this->template get<Iter>(); }
};

/*
########     ###    ##    ## ########   #######  ##     ##
##     ##   ## ##   ###   ## ##     ## ##     ## ###   ###
##     ##  ##   ##  ####  ## ##     ## ##     ## #### ####
########  ##     ## ## ## ## ##     ## ##     ## ## ### ##
##   ##   ######### ##  #### ##     ## ##     ## ##     ##
##    ##  ##     ## ##   ### ##     ## ##     ## ##     ##
##     ## ##     ## ##    ## ########   #######  ##     ##
*/

template <typename Ref>
struct erased_iface<std::random_access_iterator_tag, Ref>
    : erased_iface<std::bidirectional_iterator_tag, Ref> {
    std::unique_ptr<erased_iface> clone() const noexcept {
        return std::unique_ptr<erased_iface>(do_clone());
    }

    void           advance(std::ptrdiff_t offset) { do_advance(offset); }
    std::ptrdiff_t distance_to(erased_iface const& other) const { return do_distance_to(other); }

private:
    virtual erased_iface*  do_clone() const noexcept                       = 0;
    virtual void           do_advance(std::ptrdiff_t)                      = 0;
    virtual std::ptrdiff_t do_distance_to(erased_iface const& other) const = 0;
};

template <typename Ref, typename Iter, typename Interface>
struct erased_impl<std::random_access_iterator_tag, Ref, Iter, Interface>
    : erased_impl<std::bidirectional_iterator_tag, Ref, Iter, Interface> {
    using random_access_part = erased_impl;
    using random_access_part::bidirectional_part::bidirectional_part;

private:
    void do_advance(std::ptrdiff_t off) override {
        auto diff = static_cast<std::iter_difference_t<Iter>>(off);
        this->template get<Iter>() += diff;
    }

    std::ptrdiff_t
    do_distance_to(const erased_iface<std::random_access_iterator_tag, Ref>& other) const override {
        neo_assert(expects,
                   this->type_tag() == other.type_tag(),
                   "Comparison with random_access_iterators with different underlying types");
        return other.template get<Iter>() - this->template get<Iter>();
    }
};

template <typename Ref>
struct erased_iface<std::contiguous_iterator_tag, Ref>
    : erased_iface<std::random_access_iterator_tag, Ref> {
    std::unique_ptr<erased_iface> clone() const noexcept {
        return std::unique_ptr<erased_iface>(do_clone());
    }

private:
    virtual erased_iface* do_clone() const noexcept = 0;
};

template <typename Ref, typename Iter, typename Interface>
struct erased_impl<std::contiguous_iterator_tag, Ref, Iter, Interface>
    : erased_impl<std::random_access_iterator_tag, Ref, Iter, Interface> {
    using erased_impl::random_access_part::random_access_part;
};

/**
 * @brief Implementation class of erased input iterators.
 *
 * Prefer to use any_input_iterator<RefType>.
 *
 * If used with CTAD, RefType will be the iter_reference_t of Iter. Note that
 * the RefType does not necessicarily equal to reference type of Iter: It must
 * simply be convertible_to<iter_reference_t<Iter>, RefType>. This allows
 * erasing of `int&` iterators as `const int&` iterators.
 *
 * @tparam RefType The reference type to be returned by the erased iterator.
 * @tparam Iter The iterator type that has been erased.
 */
template <typename Ref, std::input_or_output_iterator Iter, typename Cat = iter_concept_t<Iter>>
struct erase_iterator : erased_impl<Cat, Ref, Iter, erased_iface<Cat, Ref>> {
    erase_iterator() = default;

    explicit erase_iterator(Iter it) noexcept
        : erase_iterator::erased_impl(it) {}

private:
    erase_iterator* do_clone() const noexcept { return new erase_iterator(*this); }
};

// clang-format off
template <typename From, typename To>
concept iter_ref_conversion_is_safe =
       // Converting to a value is always safe
       (!std::is_reference_v<To>)
    || requires (std::add_pointer_t<From> from, std::add_pointer_t<To> to) {
        // If 'from' is a value type and the 'to' is a reference type, thas is never safe:
        requires std::is_reference_v<From>;
        // Only valid if rebinding those as pointers is safe (i.e. no conversion operators occur:
        // Such would leave the resulting reference bound to a temporary)
        to = from;
    };
// clang-format on

template <typename From, typename To>
concept iter_ref_convertible_to
    = std::convertible_to<From, To> && iter_ref_conversion_is_safe<From, To>;

}  // namespace iter_detail

template <typename Ref, typename Category>
class any_iterator : public any_iterator_base, public iterator_facade<any_iterator<Ref, Category>> {
public:
    using reference         = Ref;
    using iterator_category = Category;

    template <typename, typename>
    friend class any_iterator;

private:
    using iface_type = iter_detail::erased_iface<iterator_category, reference>;
    std::unique_ptr<iface_type> _iter;

    template <typename FromIter>
    static std::unique_ptr<iface_type> _make_impl(const FromIter& arg) {
        return std::make_unique<iter_detail::erase_iterator<reference, FromIter>>(arg);
    }

    neo::type_tag do_type_tag() const noexcept override { return impl().type_tag(); }
    const void*   do_cvoid_ptr() const noexcept override { return impl().void_ptr(); }

public:
    // We are single-pass (input_iterator)
    enum {
        single_pass_iterator = !std::derived_from<iterator_category, std::forward_iterator_tag>
    };

    // Default-construct (required for std::regular)
    any_iterator() = default;

    // Simple copy:
    any_iterator(const any_iterator& other) noexcept
        : _iter(other._iter ? other.impl().clone() : nullptr) {}

    // Simple assign:
    any_iterator& operator=(const any_iterator& other) {
        _iter = other._iter ? other.impl().clone() : nullptr;
        return *this;
    }

    // Default-move
    any_iterator(any_iterator&&) = default;
    any_iterator& operator=(any_iterator&&) = default;

    /**
     * @brief Construct from any input_iterator whose reference_type is convertible to our own
     *
     * Excludes our own type as not to grab the copy/move constructors.
     */
    // clang-format off
    template <unalike<any_iterator> Iter>
    requires derived_from<iter_concept_t<Iter>, iterator_category>
          && iter_detail::iter_ref_convertible_to<std::iter_reference_t<Iter>, reference>
    any_iterator(const Iter& it)
        : _iter(_make_impl(it)) {}
    // clang-format on

    auto& impl() const noexcept {
        neo_assert(expects, _iter != nullptr, "Dereference of null any_iterator");
        return *_iter;
    }

    reference dereference() const { return impl().dereference(); }
    void      increment() { impl().increment(); }

    void decrement() requires derived_from<iterator_category, std::bidirectional_iterator_tag> {
        impl().decrement();
    }

    void advance(std::ptrdiff_t off) requires
        derived_from<iterator_category, std::random_access_iterator_tag> {
        impl().advance(off);
    }

    std::ptrdiff_t distance_to(const any_iterator& other) const requires
        derived_from<iterator_category, std::random_access_iterator_tag> {
        return impl().distance_to(other.impl());
    }

    // Compare two any_iterators iff they are forward iterators
    friend bool operator==(const any_iterator& left, const any_iterator& right) noexcept  //
        requires derived_from<iterator_category, std::forward_iterator_tag> {
        return left.impl() == right.impl();
    }
};

template <std::input_or_output_iterator It>
any_iterator(It) -> any_iterator<std::iter_reference_t<It>, iter_concept_t<It>>;

class any_sentinel {
    std::shared_ptr<iter_detail::erased_sentinel_parts> _impl;

    struct from_fn {};

    template <std::invocable<const any_iterator_base&> Func>
    explicit any_sentinel(Func&& fn, from_fn)
        : _impl(std::make_shared<iter_detail::erase_sentinel<Func>>(NEO_FWD(fn))) {}

public:
    any_sentinel() = default;

    any_sentinel(const any_sentinel&) noexcept = default;

    template <std::forward_iterator Iter>
    any_sentinel(const Iter& it)
        : any_sentinel(iter_detail::common_erased_sentinel_compare_parts<Iter>{it}, from_fn{}) {}

    template <std::input_or_output_iterator Iter, std::sentinel_for<Iter> S>
    any_sentinel(neo::tag<Iter>, S sentinel)
        : any_sentinel(
            [sentinel = std::move(sentinel)](const any_iterator_base& it) {
                auto iter = it.get<Iter>();
                return iter == sentinel;
            },
            from_fn{}) {}

    // Inhibit conversions with 'derived_from'
    bool operator==(std::derived_from<any_iterator_base> auto const& iter) const noexcept {
        return _impl->compare_equal(iter);
    }
};

}  // namespace neo
