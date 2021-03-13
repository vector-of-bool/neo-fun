#pragma once

#include "./range_concepts.hpp"

#include "./assert.hpp"
#include "./iterator_facade.hpp"
#include "./memory.hpp"
#include "./tag.hpp"

namespace neo {

/**
 * @brief Base class of all erased iterators
 */
class erased_iterator_base {
protected:
    erased_iterator_base()  = default;
    ~erased_iterator_base() = default;

    virtual void* do_void_ptr() noexcept = 0;

public:
    /**
     * @brief Get a tag identifying the actually underlying type of the object
     */
    virtual neo::type_tag type_tag() const noexcept = 0;

    /**
     * @brief Obtain a reference to the erased object, which must be of type T
     */
    template <typename T>
    T& get() noexcept {
        neo_assert(expects,
                   type_tag() == type_tag_v<T>,
                   "erased_iterator_base::get() was given the wrong type");
        return *static_cast<T*>(do_void_ptr());
    }

    template <typename T>
    const T& get() const noexcept {
        return const_cast<erased_iterator_base*>(this)->get<T>();
    }
};

/**
 * @brief Base class of erased range sentinels
 */
class erased_sentinel {
public:
    virtual ~erased_sentinel() = default;

    virtual bool operator==(const erased_iterator_base&) const noexcept = 0;
};

/**
 * @brief Implementation of erased sentinels.
 *
 * An erased sentinel contains an invocable object accepting an
 * `erased_iterator_base`. The invocable should return a boolean indicating
 * whether the given iterator is at the end of the range. Prefer any_sentinel
 * for most cases.
 *
 * Use this class with CTAD.
 */
template <invocable<const erased_iterator_base&> Func>
class erase_sentinel : public erased_sentinel {
    Func _fn;

public:
    erase_sentinel() = default;

    explicit erase_sentinel(Func&& fn)
        : _fn(NEO_FWD(fn)) {}

    bool operator==(const erased_iterator_base& iter) const noexcept override { return _fn(iter); }
};

/**
 * @brief A type-erased sentinel object.
 *
 * Should be constructed with an invocable that acts as a boolean predicate on an
 * `erased_iterator_base const&`, returning `true` iff the iterator has reached the end denoted by
 * this sentinel.
 */
class any_sentinel {
    std::shared_ptr<erased_sentinel> _impl;

public:
    any_sentinel() = default;

    template <invocable<const erased_iterator_base&> Func>
    explicit any_sentinel(Func&& fn)
        : _impl(std::make_shared<erase_sentinel<Func>>(NEO_FWD(fn))) {}

    bool operator==(const erased_iterator_base& iter) const noexcept { return *_impl == iter; }
};

/**
 * @brief Base class of erased input iterators with the given reference type. Can only be
 * incremented, dereferenced, and compared to iterators/sentinels. The type parameter is the
 * iter_reference_t type.
 */
template <typename RefType>
class erased_input_iterator : public erased_iterator_base,
                              public iterator_facade<erased_input_iterator<RefType>> {
public:
    enum { single_pass_iterator = true };

    virtual ~erased_input_iterator() = default;

    virtual void    increment()                  = 0;
    virtual RefType dereference() const noexcept = 0;
    // Compare to an iterator or a sentinel
    virtual bool operator==(const erased_iterator_base& other) const noexcept = 0;
    // Type-erased copy of the impl
    virtual std::unique_ptr<erased_input_iterator<RefType>> clone() const noexcept = 0;
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
template <typename RefType, typename Iter>
requires convertible_to<iter_reference_t<Iter>, RefType>  //
    class erase_input_iterator : public erased_input_iterator<RefType> {
    // The actual iterator:
    Iter _it;

    void* do_void_ptr() noexcept { return std::addressof(_it); }

public:
    erase_input_iterator() = default;

    explicit erase_input_iterator(Iter it)
        : _it(it) {}

    // Advance the iterator:
    void increment() override { ++_it; }
    // Dereference the iterator, of course
    RefType dereference() const noexcept override { return *_it; }

    // Return the type tag of the underlying iterator
    neo::type_tag type_tag() const noexcept override { return type_tag_v<Iter>; }

    bool operator==(const erased_iterator_base& other) const noexcept override {
        return other.get<Iter>() == _it;
    }

    // Clone this object:
    std::unique_ptr<erased_input_iterator<RefType>> clone() const noexcept override {
        return copy_unique(*this);
    }
};

template <typename It>
explicit erase_input_iterator(const It&) -> erase_input_iterator<iter_reference_t<It>, It>;

/**
 * @brief Type-erased input_iterator whose iter_reference_t is 'Ref' or convertible to 'Ref'
 *
 * Can be constructed with CTAD. The reference type of the erased iterator must be convertible to
 * 'Ref'.
 */
template <typename Ref>
class any_input_iterator : public iterator_facade<any_input_iterator<Ref>> {
public:
    using reference = Ref;

private:
    using erased_iter_type = erased_input_iterator<reference>;
    std::unique_ptr<erased_iter_type> _iter;

public:
    // We are single-pass (input_iterator)
    enum { single_pass_iterator = true };

    // Default-construct (required for std::regular)
    any_input_iterator() = default;

    // Simple copy:
    any_input_iterator(const any_input_iterator& other)
        : _iter(other._iter ? other.impl().clone() : nullptr) {}

    // Simple assign:
    any_input_iterator& operator=(const any_input_iterator& other) {
        _iter = other._iter ? other.impl().clone() : nullptr;
        return *this;
    }

    // Default-move
    any_input_iterator(any_input_iterator&&) = default;
    any_input_iterator& operator=(any_input_iterator&&) = default;

    /**
     * @brief Convert from some erased_input_iterator<reference>
     *
     * @param it The erased iterator
     */
    any_input_iterator(const erased_iter_type& it)
        : _iter(it.clone()) {}

    /**
     * @brief Construct from any input_iterator whose reference_type is convertible to our own
     *
     * Excludes our own type as not to grab the copy/move constructors. Excludes
     * erased_iterator types, to prevent recursing on this. infinitely.
     */
    // clang-format off
    template <typename Iter>
    requires (!std::is_base_of_v<erased_iterator_base, Iter>)
          && input_iterator<Iter>
          && unalike<Iter, any_input_iterator>  // (Exclude our own type to not grab copy/move)
          && convertible_to<iter_reference_t<Iter>, reference>
    any_input_iterator(Iter it)
        : any_input_iterator(erase_input_iterator<reference, Iter>(it)) {}
    // clang-format on

    /**
     * @brief Convert from an erased input_iterator of a different return type
     * that is able to convert to our own reference_type.
     */
    template <unalike<reference> U>
    requires(convertible_to<U, reference>) any_input_iterator(const erased_input_iterator<U>& it)
        : any_input_iterator(any_input_iterator<U>(it)) {}

    auto& impl() const noexcept {
        neo_assert(expects, _iter != nullptr, "Dereference of null any_input_iterator");
        return *_iter;
    }

    reference dereference() const { return impl().dereference(); }
    void      increment() { impl().increment(); }

    template <convertible_to<const any_input_iterator&> S>
    bool operator==(S&& other) const noexcept {
        const any_input_iterator& r{other};
        return impl() == r.impl();
    }

    neo::type_tag type_tag() const noexcept { return impl().type_tag(); }

    friend bool operator==(const any_input_iterator& it, const any_sentinel& s) noexcept {
        return it.impl() == s;
    }
};

template <input_iterator It>
any_input_iterator(It) -> any_input_iterator<iter_reference_t<It>>;

}  // namespace neo
