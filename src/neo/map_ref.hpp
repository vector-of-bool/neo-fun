#pragma once

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
 * @brief Given a reference type `T`, nested type `::type` is a reference to non-const.
 * If `T` is not a reference, `::type` is not present.
 */
template <typename T>
struct make_mref {};

template <typename T>
struct make_mref<const T&> {
    using type = T&;
};

template <typename T>
struct make_mref<const T&&> {
    using type = T&&;
};

template <typename T>
struct make_mref<T&> {
    using type = T&;
};

template <typename T>
struct make_mref<T&&> {
    using type = T&&;
};

/**
 * Convert the given reference type into a reference-to-const of the same
 * referenced type.
 */
template <typename T>
using make_cref_t = typename make_cref<T>::type;

/**
 * @brief Convert the given reference type into a mutable reference of the referred type.
 */
template <typename T>
using make_mref_t = typename make_mref<T>::type;

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
 * @brief Obtain a mutable reference for the given `T`.
 *
 * `mref_v` must not be ODR-used. Unevaluated contexts only!
 */
template <typename T>
extern make_mref_t<T&&> mref_v;

struct lref_fn {
    template <typename T>
    constexpr T& operator()(T&& t) const noexcept {
        return t;
    }

    template <typename Left>
    constexpr friend Left& operator|(Left&& l, lref_fn) noexcept {
        return l;
    }
};

/**
 * @brief Function-like and pipelinable type that converts its operand to an lvalue
 */
inline constexpr lref_fn lref;

}  // namespace neo