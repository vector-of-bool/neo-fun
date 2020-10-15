#pragma once

#include <neo/arrow_proxy.hpp>
#include <neo/ref.hpp>

#include <iterator>

namespace neo {

namespace detail {

// clang-format off
template <typename T>
concept iter_has_distance_to_method =
    requires(const T t) {
        t.distance_to(t);
    };

template <typename>
struct infer_difference_type {
    using type = std::ptrdiff_t;
};

template <iter_has_distance_to_method T>
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

template <typename T>
concept iter_has_equal_to_method =
    requires (const T it) {
        it.equal_to(it);
    };

template <typename T>
concept iter_has_increment_method =
    requires(T t) {
        t.increment();
    };

template <typename T>
concept iter_has_decrement_method =
    requires(T t) {
        t.decrement();
    };

template <typename T>
concept iter_has_advance_method =
    requires(T t, const infer_difference_type_t<T> d) {
        t.advance(d);
    };

template <typename T>
concept iter_is_random_access =
    iter_has_distance_to_method<T> &&
    iter_has_advance_method<T>;

template <typename T>
concept iter_is_bidirectional =
    iter_is_random_access<T> ||
    iter_has_decrement_method<T>;

template <typename T>
concept iter_is_single_pass = bool(T::single_pass_iterator);

template <typename T, typename Iter>
concept iter_sentinel = std::is_same_v<T, typename Iter::sentinel_type>;

template <typename Iter>
concept equal_compare_check =
    iter_has_equal_to_method<Iter>
    || iter_has_distance_to_method<Iter>;

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
 * In order for the iterator to be used in any situation that requires
 * comparison with other iterators (e.g. an `it != end()` check), then provide:
 *
 * - equal_to(Derived d) - determine if *this and `d` refer to the same
 *   position.
 *
 * Note that this is optional, and you can instead use sentinels:
 *
 *
 * ====== Sentinel Equality
 *
 * If your iterator uses a sentinel rather than an iterator to mark its end
 * state, you must inform `iterator_facade` of this fact by providing a
 * `sentinel_type` type member of the Derived class. If such a type member is
 * provided, then you must also provide:
 *
 * - at_end() - Return `true` if the iterator has reached the end. This is
 *   called automatically whenever the caller attempts to compare the iterator
 *   to an instance of `Derived::sentinel_type`.
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
            return arrow_proxy{std::move(deref)};
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
        if constexpr (detail::iter_has_increment_method<self_type>) {
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
        if constexpr (detail::iter_has_decrement_method<self_type>) {
            _self().decrement();
        } else {
            _self() -= 1;
        }
        return _self();
    }

    constexpr self_type operator--(int) noexcept requires detail::iter_is_bidirectional<self_type> {
        auto cp = _self();
        --*this;
        return cp;
    }

    template <detail::iter_diff<self_type> Diff>
    [[nodiscard]] constexpr friend self_type operator+(self_type left, Diff off) noexcept
        requires detail::iter_is_random_access<self_type> {
        return left += off;
    }

    template <detail::iter_diff<self_type> D>
    [[nodiscard]] constexpr friend self_type operator+(D off, const self_type& self) noexcept
        requires detail::iter_is_random_access<self_type> {
        return self + off;
    }

    template <detail::iter_diff<self_type> D>
    [[nodiscard]] constexpr friend self_type operator-(const self_type& self, D off) noexcept
        requires detail::iter_is_random_access<self_type> {
        using diff_type        = detail::infer_difference_type_t<self_type>;
        using signed_diff_type = std::make_signed_t<diff_type>;
        return self + -static_cast<signed_diff_type>(off);
    }

    template <detail::iter_sentinel<self_type> S>
    [[nodiscard]] constexpr friend auto operator-(S s, const self_type& self) noexcept {
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

    /**
     * Equality
     */
    [[nodiscard]] friend constexpr bool operator==(const self_type& me,
                                                   const self_type& right) noexcept
        requires detail::equal_compare_check<self_type> {
        if constexpr (detail::iter_has_equal_to_method<self_type>) {
            return me.equal_to(right);
        } else if constexpr (detail::iter_has_distance_to_method<self_type>) {
            return me.distance_to(right) == 0;
        } else {
            static_assert(detail::iter_has_distance_to_method<self_type>,
                          "Iterator must provide either `distance_to(other)` or `equal_to(other)`");
        }
    }

    template <detail::iter_sentinel<self_type> S>
    [[nodiscard]] friend constexpr bool operator==(const self_type& self, S) noexcept {
        return self.at_end();
    }

    /// XXX: GCC9 has a bug with the combination of features that are being used
    /// in this free hidden friend operator function when the sentinel is the
    /// left-hand argument and we are constrained through constraint syntax.
    /// However: It still works with the good-ol' enable_if trick.
    template <detail::iter_sentinel<self_type> S,
              typename = std::enable_if_t<detail::iter_sentinel<S, self_type>>>
    [[nodiscard]] friend constexpr bool operator==(S, const self_type& self) noexcept {
        return self.at_end();
    }

    /**
     * Inequality
     */
    [[nodiscard]] friend constexpr bool operator!=(const self_type& left,
                                                   const self_type& right) noexcept
        requires detail::equal_compare_check<self_type> {
        return !(left == right);
    }

    /// See comment above regarding GCC9
    template <detail::iter_sentinel<self_type> S>
    [[nodiscard]] friend constexpr bool operator!=(const self_type& self, S) noexcept {
        return !self.at_end();
    }

    template <detail::iter_sentinel<self_type> S,
              typename = std::enable_if_t<detail::iter_sentinel<S, self_type>>>
    [[nodiscard]] friend constexpr bool operator!=(S, const self_type& self) noexcept {
        return !self.at_end();
    }

    /**
     * Less-than
     */
    [[nodiscard]] friend constexpr bool operator<(const self_type& left,
                                                  const self_type& right) noexcept
        requires detail::iter_is_random_access<self_type> {
        return (left - right) < 0;
    }

    [[nodiscard]] friend constexpr bool operator<=(const self_type& left,
                                                   const self_type& right) noexcept
        requires detail::iter_is_random_access<self_type> {
        return (left - right) <= 0;
    }

    /**
     * Greater-than
     */
    [[nodiscard]] friend constexpr bool operator>(const self_type& left,
                                                  const self_type& right) noexcept
        requires detail::iter_is_random_access<self_type> {
        return (left - right) > 0;
    }

    [[nodiscard]] friend constexpr bool operator>=(const self_type& left,
                                                   const self_type& right) noexcept
        requires detail::iter_is_random_access<self_type> {
        return (left - right) >= 0;
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
