#pragma once

#include "./fwd.hpp"
#include <neo/arrow_proxy.hpp>
#include <neo/ref.hpp>

#include <iterator>

namespace neo {

namespace detail {

// clang-format off
template <typename Sentinel, typename T>
concept sized_sentinel_of =
    requires(const T& t, const Sentinel& s) {
        t.distance_to(s);
    };

template <typename>
struct infer_difference_type {
    using type = std::ptrdiff_t;
};

template <typename T>
    requires sized_sentinel_of<T, T>
struct infer_difference_type<T> {
    static const T& _it;
    using type = decltype(_it.distance_to(_it));
};

template <typename T>
using infer_difference_type_t = typename infer_difference_type<T>::type;

template <typename T>
struct infer_value_type {
    static const T& _it;
    using type = std::remove_const_t<std::remove_reference_t<decltype(*_it)>>;
};

template <typename T>
    requires requires { typename T::value_type; }
struct infer_value_type<T> {
    using type = typename T::value_type;
};

template <typename T>
using infer_value_type_t = typename infer_value_type<T>::type;

template <typename Sentinel, typename T>
concept sentinel_of =
    requires (const T& it, const Sentinel& s) {
        it.equal_to(s);
    };

template <typename T>
concept can_increment =
    requires(T& t) {
        t.increment();
    };

template <typename T>
concept can_decrement =
    requires(T& t) {
        t.decrement();
    };

template <typename T>
concept can_advance =
    requires(T& t, const infer_difference_type_t<T> d) {
        t.advance(d);
    };

template <typename T>
concept iter_is_random_access =
    sized_sentinel_of<T, T> &&
    can_advance<T>;

template <typename T>
concept iter_is_bidirectional =
    iter_is_random_access<T> ||
    can_decrement<T>;

template <typename T>
concept iter_is_single_pass = bool(T::single_pass_iterator);

template <typename Other, typename Iter>
concept sentinel_check =
    sentinel_of<Other, Iter>
    || sized_sentinel_of<Other, Iter>;

// clang-format on

template <typename T, typename Iter>
concept iter_diff = std::is_convertible_v<T, infer_difference_type_t<Iter>>;

struct iterator_facade_base {};

}  // namespace detail

/**
 * An iterator_facade fills-out the interface of an iterator based on just a few methods be present
 * on the derived class (provided as the CRTP parameter).
 *
 * The following methods MUST be provided:
 *
 * - Derived::dereference() - Return value of the operator*(). Need not be an
 *   actual reference.
 * - Derived::increment() OR Derived::advance(ptrdiff_t) - (Or both) Used to
 *   implement operator++() and operator++(int). If both methods are provided,
 *   `increment()` will be prefered for single increment operations.
 *
 * With these two methods defined, the iterator is a forward_iterator. If
 * `advance(ptrdiff_t)` is provided, the iterator is also bidirectional.
 *
 *
 * ====== Iterator Equality
 *
 * The generated iterator type is equality-comparible with any object of type
 * S if the derived class implements `equal_to(S)` OR `distance_to(S)`. This
 * includes sentinel types and other instances of the iterator.
 *
 *
 * ====== Single-pass Input
 *
 * If the class provides a static member `single_pass_iterator` that is `true`,
 * then the iterator will be an input_iterator, and the operator++(int)
 * (postfix decrement) will return `void`.
 *
 *
 * ====== Bidirectional
 *
 * If the following are provided, the iterator is bidirectional:
 *
 * - Derived::decrement() OR Derived::advance(ptrdiff_t) - Used to implement
 *   operator--() and operator--(int).
 *
 * Note that unless the requirements for Random Access are met, `advance()`
 * will only be called with `1` or `-1`.
 *
 *
 * ====== Random Access
 *
 * If the following are provided, the iterator is random_access_iterator:
 *
 * - Derived::advance(ptrdiff_t p) - Move the iterator by `p` (may be negative!)
 * - Derived::distance_to(Derived other) - Return the "distance" to `other`,
 *   that is: The number of types *this must be incremented to be equal to
 *   `other`, or the numner of times *this must be decremented to reach `other`
 *   (which should yield a negative number).
 *
 * These two methods are used to implement the remainder of the iterator
 * functionality.
 *
 *
 * NOTE: A specialization of std::iterator_traits<Derived> is provided that
 * provides the required iterator typedefs. You cannot access e.g.
 * `iterator_facade<...>::value_type` directly, and must instead go through
 * `iterator_traits<Derived>::value_type`.
 */
template <typename Derived>
class iterator_facade : detail::iterator_facade_base {
public:
    using self_type = Derived;

private:
    constexpr self_type&       _self() noexcept { return static_cast<self_type&>(*this); }
    constexpr const self_type& _self() const noexcept {
        return static_cast<const self_type&>(*this);
    }

public:
    /**
     * Implement operator* in terms of `.dereference()`
     */
    [[nodiscard]] constexpr decltype(auto) operator*() const noexcept {
        return _self().dereference();
    }

    /**
     * Implement arrow in terms of `operator*`, but:
     *
     * If the return type of operator* is a non-reference type, returns an
     * arrow_proxy that wraps the returned value.
     *
     * If the return type is a reference type, returns a pointer to the returned
     * object.
     */
    constexpr decltype(auto) operator->() const noexcept {
        decltype(auto) deref = **this;
        if constexpr (std::is_reference_v<decltype(deref)>) {
            // If operator*() returns a reference, just return that address
            return std::addressof(deref);
        } else {
            // It returned a value, so we need to wrap it in an arrow_proxy for the caller
            return arrow_proxy{NEO_MOVE(deref)};
        }
    }

    /**
     * If this is a random_access_iterator, returns the distance from the right
     * to the left, i.e. how many times to apply ++right to reach `left`.
     */
    [[nodiscard]] constexpr friend auto operator-(const self_type& left,
                                                  const self_type& right) noexcept
        requires detail::iter_is_random_access<self_type> {
        return right.distance_to(left);
    }

    /**
     * Advance the iterator one position forward. Implemented as a call to
     * `.increment()`, if present, otherwise `*this += 1`
     */
    constexpr self_type& operator++() noexcept {
        if constexpr (detail::can_increment<self_type>) {
            // If there is an increment(), assume it is the most efficient way to
            // advance, even if we have an advance()
            _self().increment();
        } else if constexpr (detail::iter_is_random_access<self_type>) {
            // Just offset by one
            _self() += 1;
        } else {
            // Bad!
            static_assert(detail::iter_is_random_access<self_type>,
                          "Iterator subclass must provide an `increment` or `advance(n)` method");
        }
        return _self();
    }

    constexpr auto operator++(int) noexcept {
        if constexpr (detail::iter_is_single_pass<self_type>) {
            // The iterator is a single-pass iterator. It isn't safe to make and
            // return an old copy.
            ++*this;
        } else {
            auto cp = _self();
            ++*this;
            return cp;
        }
    }

    constexpr self_type& operator--() noexcept requires detail::iter_is_bidirectional<self_type> {
        if constexpr (detail::can_decrement<self_type>) {
            _self().decrement();
        } else {
            _self() -= 1;
        }
        return _self();
    }

    constexpr auto operator--(int) noexcept requires detail::iter_is_bidirectional<self_type> {
        auto cp = _self();
        --*this;
        return cp;
    }

    template <detail::iter_diff<self_type> Diff>
    [[nodiscard]] constexpr friend auto operator+(const self_type& left, Diff off) noexcept
        requires detail::iter_is_random_access<self_type> {
        auto cp = left;
        return cp += off;
    }

    template <detail::iter_diff<self_type> D>
    [[nodiscard]] constexpr friend auto operator+(D off, const self_type& self) noexcept
        requires detail::iter_is_random_access<self_type> {
        return self + off;
    }

    template <detail::iter_diff<self_type> D>
    [[nodiscard]] constexpr friend auto operator-(const self_type& self, D off) noexcept
        requires detail::iter_is_random_access<self_type> {
        using diff_type        = detail::infer_difference_type_t<self_type>;
        using signed_diff_type = std::make_signed_t<diff_type>;
        return self + -static_cast<signed_diff_type>(off);
    }

    template <detail::sized_sentinel_of<self_type> S>
    [[nodiscard]] constexpr friend auto operator-(const S& s, const self_type& self) noexcept {
        return self.distance_to(s);
    }

    template <detail::iter_diff<self_type> D>
    constexpr friend self_type& operator+=(self_type& self, D off) noexcept
        requires detail::iter_is_random_access<self_type> {
        self.advance(off);
        return self;
    }

    template <detail::iter_diff<self_type> D>
    constexpr friend self_type& operator-=(self_type& self, D off) noexcept
        requires detail::iter_is_random_access<self_type> {
        return self = self - off;
    }

    template <detail::iter_diff<self_type> D>
    [[nodiscard]] constexpr decltype(auto) operator[](D pos) const noexcept
        requires detail::iter_is_random_access<self_type> {
        return *(_self() + pos);
    }

#if !__cpp_impl_three_way_comparison
#error "neo/iterator_facade.hpp requires three-way-comparison support"
#endif

    /**
     * With three-way comparison, we can get away with much simpler comparison/equality
     * operators, since we can also rely on synthesized rewrites
     */
    template <detail::sentinel_check<self_type> S>
    [[nodiscard]] constexpr friend bool operator==(const self_type& self, const S& other) noexcept {
        if constexpr (detail::sentinel_of<S, self_type>) {
            return self.equal_to(other);
        } else if constexpr (detail::sized_sentinel_of<self_type, S>) {
            return self.distance_to(other) == 0;
        } else {
            static_assert(detail::sized_sentinel_of<self_type, S>,
                          "Iterator must provide either `distance_to(other)` or `equal_to(other)`");
        }
    }

    [[nodiscard]] constexpr friend bool operator==(const self_type& left,
                                                   const self_type& other) noexcept
        requires detail::sentinel_check<self_type, self_type> {
        if constexpr (detail::sentinel_of<self_type, self_type>) {
            return left.equal_to(other);
        } else if constexpr (detail::sized_sentinel_of<self_type, self_type>) {
            return left.distance_to(other) == 0;
        } else {
            static_assert(detail::sized_sentinel_of<self_type, self_type>,
                          "[should never be seen]");
        }
    }

    template <detail::sized_sentinel_of<self_type> S>
    [[nodiscard]] constexpr auto operator<=>(const S& right) const noexcept {
        auto dist = _self() - right;
        auto rel  = dist <=> 0;
        return rel;
    }
};  // namespace neo

template <typename Derived, typename InnerIterator>
class iterator_wrapper_facade : public iterator_facade<Derived> {
    using base      = typename iterator_wrapper_facade::iterator_facade;
    using self_type = Derived;

protected:
    InnerIterator wrapped_iterator;

public:
    iterator_wrapper_facade() = default;
    explicit iterator_wrapper_facade(InnerIterator it)
        : wrapped_iterator(it) {}

    template <detail::iter_diff<self_type> D>
    constexpr void advance(D off) noexcept {
        std::advance(wrapped_iterator, off);
    }

    // clang-format off
    constexpr auto distance_to(const self_type& other) const noexcept
        requires std::is_base_of_v<
            typename std::iterator_traits<InnerIterator>::iterator_category,
            std::random_access_iterator_tag> {
        return other.wrapped_iterator - wrapped_iterator;
    }
    // clang-format on

    constexpr bool equal_to(const self_type& other) const noexcept {
        return wrapped_iterator == other.wrapped_iterator;
    }
};

}  // namespace neo

namespace std {

template <typename Derived>
requires std::is_base_of_v<neo::detail::iterator_facade_base,
                           Derived>  //
    struct iterator_traits<Derived> {
    static const Derived& _const_it;
    using reference       = decltype(*_const_it);
    using pointer         = decltype(_const_it.operator->());
    using value_type      = neo::detail::infer_value_type_t<Derived>;
    using difference_type = neo::detail::infer_difference_type_t<Derived>;

    // Pick the iterator category based on the interfaces that it provides
    using iterator_category = std::conditional_t<
        // Random access?
        neo::detail::iter_is_random_access<Derived>,
        std::random_access_iterator_tag,
        // Nope
        std::conditional_t<
            // Bidirectional?
            neo::detail::iter_is_bidirectional<Derived>,
            std::bidirectional_iterator_tag,
            // Noh
            std::conditional_t<
                // Is it single-pass?
                neo::detail::iter_is_single_pass<Derived>,
                // Than means it is an input iterator
                std::input_iterator_tag,
                // Otherwise it is a forward iterator
                std::forward_iterator_tag>>>;

    using iterator_concept = iterator_category;
};

}  // namespace std
