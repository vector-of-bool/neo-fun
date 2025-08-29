#pragma once

#include <neo/ref.hpp>
#include <neo/type_traits.hpp>
#include <neo/unit.hpp>

namespace neo {

namespace detail {

template <bool IsObject, bool IsArray, bool IsReference, bool IsVoid>
struct object_impl;

template <>
struct object_impl<true, false, false, false> {
    template <typename T>
    using f = T;
};

template <>
struct object_impl<false, false, true, false> {
    template <typename T>
    using f = reference_object<T>;
};

template <>
struct object_impl<false, false, false, true> {
    template <typename>
    using f = unit;
};

}  // namespace detail

/**
 * @brief Extra-light alternative to object_box
 *
 * If `T` is an object type, yields `T`. If `T` is `void`, yields `neo::unit`. If `T` is a
 * reference type, yields `reference_object<T>`.
 */
template <typename T>
using object_t = detail::
    object_impl<object_type<T>, array_type<T>, reference_type<T>, void_type<T>>::template f<T>;

}  // namespace neo
