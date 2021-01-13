#pragma once

#include <neo/concepts.hpp>

#include <cstddef>      // ptrdiff_t
#include <iterator>     // various
#include <memory>       // pointer_traits
#include <type_traits>  // various
#include <utility>      // forward, move

#if !__cpp_lib_concepts

namespace neo {

struct contiguous_iterator_tag : std::random_access_iterator_tag {};

template <typename T>
constexpr auto to_address(T* ptr) noexcept {
    static_assert(!std::is_function_v<T>);
    return ptr;
}

template <typename T>
requires requires(T ptr) {
    std::pointer_traits<T>::to_adderss(ptr);
}
constexpr auto to_address(const T& p) { return std::pointer_traits<T>::to_address(p); }

template <typename T>
requires requires(T it) {
    it.operator->();
}
constexpr auto to_address(const T& ptr) { return to_address(ptr.operator->()); }

// clang-format off

namespace detail {

template <typename T, typename = void>
struct ITER_TRAITS_1 {
    using type = T;
};

template <typename T>
requires requires { typename std::iterator_traits<T>::pointer; }
struct ITER_TRAITS_1<T> {
    using type = std::iterator_traits<T>;
};

template <typename T>
using ITER_TRAITS = typename ITER_TRAITS_1<T>::type;


template <typename T>
struct ITER_CONCEPT_1 {};

template <typename T>
requires requires { typename ITER_TRAITS<T>::iterator_concept; }
struct ITER_CONCEPT_1<T> {
    using type = typename ITER_TRAITS<T>::iterator_concept;
};

template <typename T>
requires (
    requires { typename ITER_TRAITS<T>::iterator_category; }
    && ! requires { typename ITER_TRAITS<T>::iterator_concept; }
)
struct ITER_CONCEPT_1<T> {
    using type = typename ITER_TRAITS<T>::iterator_category;
};

/// XXX: Approximates the ITER_CONCEPT from std::
template <typename T>
using ITER_CONCEPT = typename ITER_CONCEPT_1<T>::type;

} // namespace detail

template <typename> struct incrementable_traits {};

/// Pointers
template <typename T>
    requires std::is_object_v<T>
struct incrementable_traits<T*> {
    using difference_type = std::ptrdiff_t;
};

/// Strip `const`
template <typename T>
struct incrementable_traits<const T> : incrementable_traits<T> {};

/// Types with `difference_type`
template <typename T>
    requires requires { typename T::difference_type; }
struct incrementable_traits<T> {
    using difference_type = typename T::difference_type;
};

namespace detail {

template <typename T, typename = void>
struct iter_difference_type_t_impl {
    using type = typename incrementable_traits<T>::difference_type;
};

template <typename T>
struct iter_difference_type_t_impl<T, std::void_t<typename std::iterator_traits<T>::difference_type>> {
    using type = typename std::iterator_traits<T>::difference_type;
};

} // namespace detail

/**
 * Obtain the difference_type of an iterator.
 */
template <typename T>
using iter_difference_t = typename detail::iter_difference_type_t_impl<T>::type;

namespace detail {

    template <typename>
    struct irt_cond_value_type {};

    template <typename T>
        requires std::is_object_v<T>
    struct irt_cond_value_type<T> {
        using value_type = std::remove_cv_t<T>;
    };

}  // namespace detail

template <typename>
struct indirectly_readable_traits {};

/// Pointers
template <typename T>
struct indirectly_readable_traits<T*> : detail::irt_cond_value_type<T> {};

/// Arrays
template <typename Array>
    requires std::is_array_v<Array>
struct indirectly_readable_traits<Array> {
    using value_type = std::remove_cv_t<std::remove_extent_t<Array>>;
};

/// Strip `const`
template <typename T>
struct indirectly_readable_traits<const T> : indirectly_readable_traits<T> {};

/// Anything with `value_type`
template <typename T>
    requires requires { typename T::value_type; }
struct indirectly_readable_traits<T> : detail::irt_cond_value_type<typename T::value_type> {};

/// Anything with `element_type`
template <typename T>
    requires requires { typename T::element_type; }
struct indirectly_readable_traits<T> : detail::irt_cond_value_type<typename T::element_type> {};

namespace detail {

    template <typename T, typename = void>
    struct iter_value_t_impl {
        using type = typename indirectly_readable_traits<T>::value_type;
    };

    template <typename T>
    struct iter_value_t_impl<T, std::void_t<typename std::iterator_traits<T>::value_type>> {
        using type = typename std::iterator_traits<T>::value_type;
    };

} // namespace detail

/**
 *  Get the value type of an iterator or of a type which exposes element_type or value_type
 */
template <typename Iter>
using iter_value_t = typename detail::iter_value_t_impl<std::remove_cv_t<std::remove_reference_t<Iter>>>::type;


namespace detail {

    template <typename T>
    using with_reference = T&;

    template <typename T>
    concept can_reference = requires { typename with_reference<T>; };

    template <typename T>
    concept dereferenceable =
        requires (T& t) {
            { *t } -> detail::can_reference;
        };


} // namespace detail

template <detail::dereferenceable T>
using iter_reference_t = decltype(*std::declval<T&>());

/// XXX: Not exactly equivalent to std:: version. Requires ranges::iter_move.
/// This is an approximation.
template <detail::dereferenceable T>
using iter_rvalue_reference_t = decltype(std::move(*std::declval<T&>()));

namespace detail {

    template <typename In>
    concept indirectly_readable_1 =
        requires(const In in) {
            typename iter_value_t<In>;
            typename iter_reference_t<In>;
            typename iter_rvalue_reference_t<In>;
            #if NEO_CONCEPTS_IS_CONCEPTS_TS
            { *in } -> detail::can_reference;
            { std::move(*in) } -> detail::can_reference;
            #else
            { *in } -> same_as<iter_reference_t<In>>;
            { std::move(*in) } -> same_as<iter_rvalue_reference_t<In>>;
            #endif
        };

} // namespace detail

/// XXX: Not exactly equivalent. Missing common_reference_with checks.
template <typename In>
concept indirectly_readable = detail::indirectly_readable_1<std::remove_cv_t<std::remove_reference_t<In>>>;

/**
 * Test if we can write-thru an object with the dereference operator
 */
template <typename Out, typename T>
concept indirectly_writable = requires(Out&& o, T&& t) {
    *o = std::forward<T>(t);
    *std::forward<Out>(o) = std::forward<T>(t);
    const_cast<const iter_reference_t<Out>&&>(*o) = std::forward<T>(t);
    const_cast<const iter_reference_t<Out>&&>(*std::forward<Out>(o)) = std::forward<T>(t);
};

/**
 * Model an iterator that can be incremented, but result of `iter++` need not
 * be an iterator that can be dereferenced (this "weak").
 */
template<typename T>
concept weakly_incrementable =
    default_initializable<T>
    && movable<T>
    && requires(T item) {
        typename iter_difference_t<T>;
        requires std::is_signed_v<iter_difference_t<T>>;  /// XXX: Approx
        #if !NEO_CONCEPTS_IS_CONCEPTS_TS
        /// GCC 9 with -foncepts has trouble with the trailing expression
        { ++item } -> same_as<T&>;
        #else
        ++item;
        #endif
        item++;
    };

/**
 * Model an iterator that can be incremented, but the result of `iter++` must
 * be an instance of the iterator.
 */
template <typename T>
concept incrementable =
    regular<T>
    && weakly_incrementable<T>
    && requires(T item) {
        { item++ } -> same_as<T>;
    };

/**
 * Model any type of iterator. The basis of all other iterator types.
 */
template <typename T>
concept input_or_output_iterator =
    weakly_incrementable<T>
    && requires (T iter) {
        { *iter } -> detail::can_reference;
    };

/**
 * Test if the given sentinel and iterator can be compared for equality.
 */
template <typename Sent, typename Iter>
concept sentinel_for =
    semiregular<Sent>
    && input_or_output_iterator<Iter>
    && detail::weakly_equality_comparable_with<Sent, Iter>; // Defined in concepts.hpp

/**
 * Customization point to disable sized_sentinel_for between an iterator and
 * sentinel type.
 */
template <typename Sent, typename Iter>
inline constexpr bool disabled_sized_sentinel_for = false;

/**
 * Get a sentinel which can be used to compute the distance from the iterator
 * to the sentinel in constant time. Effectively computes the distance from the
 * end of the range.
 */
template <typename Sent, typename Iter>
concept sized_sentinel_for =
    sentinel_for<Sent, Iter>
    && !disabled_sized_sentinel_for<std::remove_cv_t<Sent>, std::remove_cv_t<Iter>>
    && requires (const Iter& iter, const Sent& sent) {
        { sent - iter } -> same_as<iter_difference_t<Iter>>;
        { iter - sent } -> same_as<iter_difference_t<Iter>>;
    };

/**
 * Model an input iterator. That is, an iterator that can be read from.
 */
template <typename Iter>
concept input_iterator =
    input_or_output_iterator<Iter>
    && indirectly_readable<Iter>
    && requires { typename detail::ITER_CONCEPT<Iter>; }
    && derived_from<detail::ITER_CONCEPT<Iter>, std::input_iterator_tag>;

template <typename Iter, typename T>
concept output_iterator =
    input_or_output_iterator<Iter>
    && indirectly_writable<Iter, T> &&
    requires(Iter it, T&& val) {
        *it++ = std::forward<T>(val);
    };

template <typename Iter>
concept forward_iterator =
    input_iterator<Iter>
    && derived_from<detail::ITER_CONCEPT<Iter>, std::forward_iterator_tag>
    && incrementable<Iter>
    && sentinel_for<Iter, Iter>;

template <typename Iter>
concept bidirectional_iterator =
    forward_iterator<Iter>
    && derived_from<detail::ITER_CONCEPT<Iter>, std::bidirectional_iterator_tag>
    && requires(Iter it) {
        #if NEO_CONCEPTS_IS_CONCEPTS_TS
        --it;
        #else
        { --it } -> same_as<Iter&>;
        #endif
        { it-- } -> same_as<Iter>;
    };

template <typename Iter>
concept random_access_iterator =
    bidirectional_iterator<Iter>
    && derived_from<detail::ITER_CONCEPT<Iter>, std::random_access_iterator_tag>
    && totally_ordered<Iter>
    && sized_sentinel_for<Iter, Iter>
    && requires(Iter it, const Iter c_it, const iter_difference_t<Iter> off) {
        #if NEO_CONCEPTS_IS_CONCEPTS_TS
        it += off;
        it -= off;
        { c_it[off] } -> detail::can_reference;
        #else
        { it += off } -> same_as<Iter&>;
        { it -= off } -> same_as<Iter&>;
        { c_it[off] } -> same_as<iter_reference_t<Iter>>;
        #endif
        { c_it + off } -> same_as<Iter>;
        { off + c_it } -> same_as<Iter>;
        { c_it - off } -> same_as<Iter>;
    };

template <typename Iter>
concept contiguous_iterator =
    random_access_iterator<Iter>
    && (derived_from<detail::ITER_CONCEPT<Iter>, contiguous_iterator_tag>
        // stdlib doesn't do contiguous_iterator_Tag yet, so we'll force it through
        // with a check if the iterator is a pointer
        || std::is_pointer_v<Iter>)
    && std::is_lvalue_reference_v<iter_reference_t<Iter>>
    && same_as<iter_value_t<Iter>, std::remove_cv_t<std::remove_reference_t<iter_reference_t<Iter>>>>
    && requires (const Iter& it) {
        { to_address(it) } -> same_as<std::add_pointer_t<iter_reference_t<Iter>>>;
    };

// clang-format on

}  // namespace neo

#else  // __cpp_lib_concepts

namespace neo {

using std::iter_difference_t;
using std::iter_reference_t;
using std::iter_rvalue_reference_t;
using std::iter_value_t;

using std::bidirectional_iterator;
using std::contiguous_iterator;
using std::forward_iterator;
using std::incrementable;
using std::indirectly_readable;
using std::indirectly_writable;
using std::input_iterator;
using std::input_or_output_iterator;
using std::output_iterator;
using std::random_access_iterator;
using std::sentinel_for;
using std::sized_sentinel_for;
using std::weakly_incrementable;

}  // namespace neo

#endif