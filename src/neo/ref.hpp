#pragma once

#include <functional>
#include <type_traits>

namespace neo {

/**
 * Given a reference type `T`, nested type `::type` is reference-to-const. If
 * `T` is not a reference, `::type` is not present.
 */
template <typename T>
struct make_cref {};

template <typename T>
struct make_cref<T&> {
    using type = const T&;
};

template <typename T>
struct make_cref<T&&> {
    using type = const T&&;
};

/**
 * Convert the given reference type into a reference-to-const of the same
 * referenced type.
 */
template <typename T>
using make_cref_t = typename make_cref<T>::type;

/**
 * Obtain a variable of `T&&`. If given an lvalue-reference, returns an
 * lvalue-reference, otherwise an rvalue-reference. Maintains cv-qualifiers of
 * the referenced `T`. Note that the result will ALWAYS be an lvalue expression,
 * even if the decltype() is an rvalue reference (This matches the rules for
 * regular variables of rvalue-reference type).
 *
 * `ref_v` must not be ODR-used. Unevaluated contexts only!
 */
template <typename T>
extern T&& ref_v;

/**
 * Obtain a reference-to-const for the given `T`. If `T` is an lvalue-reference,
 * yields `const T&`, otherwise `const T&&`. The result will *always* be an
 * lvalue expression.
 *
 * `cref_v` must not be ODR-used. Unevaluated contexts only!
 */
template <typename T>
extern make_cref_t<T&&> cref_v;

}  // namespace neo
