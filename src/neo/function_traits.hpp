#pragma once

#include "./tag.hpp"

#include <type_traits>

namespace neo {

namespace function_detail {

template <typename... Args>
struct sole_arg_impl {};

template <typename T>
struct sole_arg_impl<T> {
    using sole_arg_type = T;
};

}  // namespace function_detail

/**
 * @brief A tag type that holds a function signature
 *
 * @tparam Sig A function signature type
 */
template <typename Sig>
struct signature_tag;

template <typename Ret, typename... Args>
struct signature_tag<Ret(Args...)> : function_detail::sole_arg_impl<Args...> {
    /// The return type of the function signature
    using return_type = Ret;
    /// A tag containing the argument types of the function signature
    using arg_types = tag<Args...>;

    /// Whether or not the function is declared to be noexcept(true)
    constexpr static bool is_noexcept = false;

    /// The number of arguments that is accepted by the signature
    constexpr static std::size_t arg_count = sizeof...(Args);
};

// Specialize for noexcept(true) to override the is_noexcept member, but inherit from the
// noexcept(false) version
template <typename Ret, typename... Args>
struct signature_tag<Ret(Args...) noexcept> : signature_tag<Ret(Args...)> {
    constexpr static bool is_noexcept = true;
};

/**
 * @brief Obtain attributes of the given invocable type.
 *
 * If the type's `operator()` is overloaded or templated, the invocable signature cannot be
 * obtained.
 *
 * Inherits from a signature_tag for the appropriate function signature type.
 *
 * @tparam T An invocable type, such as a function, function pointer, or object
 * with a single overloaded call-operator.
 */
template <typename T>
struct invocable_signature {};

/**
 * @brief Alias of the return type of an invocable type.
 */
template <typename T>
using invocable_return_type_t = typename invocable_signature<T>::return_type;

/**
 * @brief Alias of a tag type for the arguments of an invocable type.
 */
template <typename T>
using invocable_arg_types_t = typename invocable_signature<T>::arg_types;

/**
 * @brief Get the type of the sole argument to the given invocable type. If the invocable does not
 * accept exactly one argument, this is not a valid type.
 */
template <typename Func>
using sole_arg_type_t = typename invocable_signature<Func>::sole_arg_type;

/**
 * @brief Obtain the arity (argument count) of an invocable type
 */
template <typename T>
constexpr std::size_t invocable_arity_v = invocable_signature<T>::arg_count;

/**
 * @brief Determine if an invocable type is noexcept(true)
 */
template <typename T>
constexpr std::size_t invocable_is_noexcept_v = invocable_signature<T>::is_noexcept;

// Overload for function pointers
template <typename Ret, typename... Args>
struct invocable_signature<Ret (*)(Args...)> : signature_tag<Ret(Args...)> {};

template <typename Ret, typename... Args>
struct invocable_signature<Ret (*)(Args...) noexcept> : signature_tag<Ret(Args...) noexcept> {};

// ... and function references
template <typename Ret, typename... Args>
struct invocable_signature<Ret (&)(Args...)> : signature_tag<Ret(Args...)> {};

template <typename Ret, typename... Args>
struct invocable_signature<Ret (&)(Args...) noexcept> : signature_tag<Ret(Args...) noexcept> {};

// ... and function rvalue references
template <typename Ret, typename... Args>
struct invocable_signature<Ret(&&)(Args...)> : signature_tag<Ret(Args...)> {};

template <typename Ret, typename... Args>
struct invocable_signature<Ret(&&)(Args...) noexcept> : signature_tag<Ret(Args...) noexcept> {};

// Base overload for direct signatures:
template <typename Ret, typename... Args>
struct invocable_signature<Ret(Args...)> : signature_tag<Ret(Args...)> {};

template <typename Ret, typename... Args>
struct invocable_signature<Ret(Args...) noexcept> : signature_tag<Ret(Args...) noexcept> {};

// Several combinations for member function pointers:
#define DECL_MEM_INFER(Qual, ThisQual)                                                             \
    template <typename Owner, typename Ret, typename... Args>                                      \
    struct invocable_signature<Ret (Owner::*)(Args...) Qual>                                       \
        : signature_tag<Ret(Owner ThisQual, Args...)> {                                            \
        template <typename T>                                                                      \
        using apply_cvr_t = T Qual;                                                                \
    };                                                                                             \
    template <typename Owner, typename Ret, typename... Args>                                      \
    struct invocable_signature<Ret (Owner::*)(Args...) Qual noexcept>                              \
        : signature_tag<Ret(Owner ThisQual, Args...) noexcept> {                                   \
        template <typename T>                                                                      \
        using apply_cvr_t = T Qual;                                                                \
    }

DECL_MEM_INFER(, &);
DECL_MEM_INFER(const, const&);
DECL_MEM_INFER(volatile, volatile&);
DECL_MEM_INFER(const volatile, const volatile&);

DECL_MEM_INFER(&, &);
DECL_MEM_INFER(const&, const&);
DECL_MEM_INFER(volatile&, volatile&);
DECL_MEM_INFER(const volatile&, const volatile&);

DECL_MEM_INFER(&&, &&);
DECL_MEM_INFER(const&&, const&&);
DECL_MEM_INFER(volatile&&, volatile&&);
DECL_MEM_INFER(const volatile&&, const volatile&&);

#undef DECL_MEM_INFER

// Overload for member object pointers too
template <typename T, typename Owner>
requires std::is_member_object_pointer_v<T Owner::*>  //
    struct invocable_signature<T Owner::*> : signature_tag<T&(Owner&) noexcept> {};

namespace detail {

template <typename R, typename Tag, bool NoExcept>
struct make_obj_signature;

template <typename R, typename This, typename... Args>
struct make_obj_signature<R, tag<This, Args...>, false> : signature_tag<R(Args...)> {};

template <typename R, typename This, typename... Args>
struct make_obj_signature<R, tag<This, Args...>, true> : signature_tag<R(Args...) noexcept> {};

template <typename MemPtr>
struct callable_object_signature {
    using mem_signature = invocable_signature<MemPtr>;

    using type = make_obj_signature<typename mem_signature::return_type,
                                    typename mem_signature::arg_types,
                                    mem_signature::is_noexcept>;
};

}  // namespace detail

// This case catches objects with overloaded 'operator()'
template <typename Callable>
requires requires {
    &std::remove_cvref_t<Callable>::operator();
}
struct invocable_signature<Callable> : detail::callable_object_signature<decltype(
                                           &std::remove_cvref_t<Callable>::operator())>::type {};

/**
 * @brief A type which has a fixed signature, i.e. an invocable that is not overloaded
 */
template <typename T>
concept fixed_invocable = requires {
    typename invocable_return_type_t<T>;
};

}  // namespace neo
