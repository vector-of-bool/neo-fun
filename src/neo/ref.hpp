#pragma once

#include <functional>
#include <type_traits>

#include "./attrib.hpp"

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

/**
 * If `T` is a reference type, the `type` member names a
 * std::reference_wrapper<U>, where `U` is the referred-to type of `T`
 * (including cv-qualifierd). For all other types, the `type` member is `T`
 * unmodified.
 */
template <typename T>
struct wrap_refs {
    using type = T;
};

template <typename T>
requires std::is_reference_v<T>  //
    struct wrap_refs<T> {
    using type = std::reference_wrapper<std::remove_reference_t<T>>;
};

/**
 * If the given `T` is a reference, becomes a `std::reference_wrapper<U>` where
 * `U` is the referred-to type. Otherwise, becomes `T` unmodified.
 */
template <typename T>
using wrap_refs_t = typename wrap_refs<T>::type;

template <typename T>
using wrap_if_reference_t [[deprecated("Use wrap_refs_t")]] = wrap_refs_t<T>;

template <typename T>
using add_rref_t = T&&;

/**
 * Un-wraps a reference_wrapper<T>. Returns a reference to the referred-to
 * object. If the given object is not a reference_wrapper, returns a reference
 * to the argument.
 */
template <typename T>
NEO_ALWAYS_INLINE constexpr T&& unref(T&& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
NEO_ALWAYS_INLINE constexpr T& unref(std::reference_wrapper<T> t) noexcept {
    return t;
}

/**
 * Declare a set of getter-methods that return a member variable after passing it through
 * neo::unref(). This is meant to be used in conjunction with wrap_refs_t
 */
#define NEO_DECL_UNREF_GETTER(FuncName, MemName)                                                   \
    constexpr auto&& FuncName()& noexcept { return ::neo::unref(this->MemName); }                  \
    constexpr auto&& FuncName() const& noexcept { return ::neo::unref(this->MemName); }            \
    constexpr auto&& FuncName()&& noexcept {                                                       \
        return ::neo::unref(static_cast<decltype(this->MemName)&&>(this->MemName));                \
    }                                                                                              \
    static_assert(true)

/**
 * @brief Declare a method 'FuncName' that rebinds a reference-wrapper member 'MemName'
 *
 * 'MemName' should be a datamember declared with wrap_refs_t, and 'Type' should be given
 * as the same argument that was given to wrap_refs_t. The declared method will only be valid
 * if 'Type' is a reference type.
 *
 * @param FuncName The name of the method to declare
 * @param Type The type parameters that was passed to wrap_refs_t for MemName
 * @param MemName The member variable declared with wrap_refs_t to rebind
 */
#define NEO_DECL_REF_REBINDER(FuncName, Type, MemName)                                             \
    constexpr auto&& FuncName(Type& arg) noexcept requires(std::is_reference_v<Type>) {            \
        MemName = std::ref(arg);                                                                   \
        return MemName.get();                                                                      \
    }                                                                                              \
    static_assert(true)

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

/**
 * @brief Equivalent of std::declval<T>(), but less compiler overhead
 */
#define NEO_DECLVAL(...) (((::neo::add_rref_t<__VA_ARGS__>(*)())(void*)(nullptr))())

}  // namespace neo
