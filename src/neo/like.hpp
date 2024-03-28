#pragma once

#include "./attrib.hpp"
#include "./type_traits.hpp"

namespace neo {

namespace like_detail {

template <bool IsConst, int Nref>
struct forward_like_impl;

template <>
struct forward_like_impl<true, 1> {
    template <typename T>
    using merge = add_lvalue_reference_t<add_const_t<remove_reference_t<T>>>;

    template <typename T>
    using tuple = add_lvalue_reference_t<add_const_t<T>>;

    template <typename T>
    using language = add_lvalue_reference_t<add_const_t<T>>;
};

template <>
struct forward_like_impl<true, 0> {
    template <typename T>
    using merge = add_rvalue_reference_t<add_const_t<remove_reference_t<T>>>;

    template <typename T>
    using tuple = add_rvalue_reference_t<add_const_t<T>>;

    template <typename T>
    using language = add_lvalue_reference_t<add_const_t<T>>;
};

template <>
struct forward_like_impl<false, 1> {
    template <typename T>
    using merge = add_lvalue_reference_t<remove_reference_t<T>>;

    template <typename T>
    using tuple = add_lvalue_reference_t<T>;

    template <typename T>
    using language = add_lvalue_reference_t<T>;
};

template <>
struct forward_like_impl<false, 0> {
    template <typename T>
    using merge = add_rvalue_reference_t<remove_reference_t<T>>;

    template <typename T>
    using tuple = add_rvalue_reference_t<T>;

    template <typename T>
    using language = add_lvalue_reference_t<T>;
};

template <>
struct forward_like_impl<false, 2> : forward_like_impl<false, 0> {
    template <typename T>
    using language = add_lvalue_reference_t<T>;
};

template <>
struct forward_like_impl<true, 2> : forward_like_impl<true, 0> {
    template <typename T>
    using language = add_lvalue_reference_t<add_const_t<T>>;
};

template <typename T>
using model = forward_like_impl<const_type<remove_reference_t<T>>,
                                rvalue_reference_type<T>       ? 2
                                    : lvalue_reference_type<T> ? 1
                                                               : 0>;

}  // namespace like_detail

/**
 * @brief Merge the cvref-qualifiers of `T` onto `U`
 */
template <typename T, typename U>
using forward_like_t = like_detail::model<T>::template merge<U>;

/**
 * @brief Use a forward-like as if we were calling get<>() with a tuple.
 *
 * @tparam Owner The "owner" of the object.
 * @tparam T the type to be transformed.
 *
 * - If `T` is a non-reference type, the cvref qualifiers from `Owner` will be applied
 *   to `T`.
 * - Otherwise, the cv-qualifiers from `Owner` are ignored, and the reference qualifiers
 *   of `Owner` are concatenated with the reference qualifiers of `T` and the resulting
 *   collapsed reference type is the final result.
 */
template <typename Owner, typename T>
using forward_like_tuple_t = like_detail::model<Owner>::template tuple<T>;

template <typename T, typename U>
NEO_ALWAYS_INLINE constexpr forward_like_t<T, U> forward_like(U&& arg) noexcept {
    return static_cast<forward_like_t<T, U>>(arg);
}

}  // namespace neo
