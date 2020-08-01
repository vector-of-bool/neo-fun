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

/**
 * If `T` is a reference type, the `type` member names a
 * std::reference_wrapper<U>, where `U` is the referred-to type of `T`
 * (including cv-qualifierd). For all other types, the `type` member is `T`
 * unmodified.
 */
template <typename T>
struct wrap_if_reference {
    using type = T;
};

template <typename T>
requires std::is_reference_v<T>  //
    struct wrap_if_reference<T> {
    using type = std::reference_wrapper<std::remove_reference_t<T>>;
};

/**
 * If the given `T` is a reference, becomes a `std::reference_wrapper<U>` where
 * `U` is the referred-to type. Otherwise, becomes `T` unmodified.
 */
template <typename T>
using wrap_if_reference_t = typename wrap_if_reference<T>::type;

/**
 * Un-wraps a reference_wrapper<T>. Returns a reference to the referred-to
 * object. If the given object is not a reference_wrapper, returns a reference
 * to the argument.
 */
template <typename T>
constexpr T&& unref(T&& t) noexcept {
    return std::forward<T>(t);
}

template <typename T>
constexpr T& unref(std::reference_wrapper<T> t) noexcept {
    return t;
}

}  // namespace neo
