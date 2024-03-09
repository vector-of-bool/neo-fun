#pragma once

#include "./type_traits.hpp"

namespace neo {

namespace like_detail {

template <bool IsConst, int Nref>
struct forward_like_impl;

template <>
struct forward_like_impl<true, 1> {
    template <typename T>
    using merge = remove_reference_t<T> const&;

    template <typename T>
    using tuple = T const&;

    template <typename T>
    using language = T const&;
};

template <>
struct forward_like_impl<true, 0> {
    template <typename T>
    using merge = remove_reference_t<T> const&&;

    template <typename T>
    using tuple = T const&&;

    template <typename T>
    using language = T const&;
};

template <>
struct forward_like_impl<false, 1> {
    template <typename T>
    using merge = remove_reference_t<T>&;

    template <typename T>
    using tuple = T&;

    template <typename T>
    using language = T&;
};

template <>
struct forward_like_impl<false, 0> {
    template <typename T>
    using merge = remove_reference_t<T>&&;

    template <typename T>
    using tuple = T&&;

    template <typename T>
    using language = T&;
};

template <>
struct forward_like_impl<false, 2> : forward_like_impl<false, 0> {
    template <typename T>
    using language = T&;
};

template <>
struct forward_like_impl<true, 2> : forward_like_impl<true, 0> {
    template <typename T>
    using language = const T&;
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

template <typename T, typename U>
constexpr forward_like_t<T, U> forward_like(U&& arg) noexcept {
    return static_cast<forward_like_t<T, U>>(arg);
}

}  // namespace neo
