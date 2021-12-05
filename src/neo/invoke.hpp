#pragma once

#include "./declval.hpp"
#include "./fwd.hpp"

namespace neo {

#define INVOKE_BODY(...)                                                                           \
    noexcept(noexcept(__VA_ARGS__)) requires requires { __VA_ARGS__; }                             \
    { return __VA_ARGS__; }                                                                        \
    static_assert(true)

/**
 * @brief "Invoke" an invocable object. Like std::invoke, but cleaner and less debug overhead
 *
 * @tparam Func The invocable. Must be a function, or object that has an operator() overload
 * @tparam Args The arguments to pass to the invocable.
 *
 * This is the base overload that will catch things that are callable, including with operator()
 */
template <typename Func, typename... Args>
constexpr decltype(auto) invoke(Func&& fn, Args&&... args)
    INVOKE_BODY(NEO_FWD(fn)(NEO_FWD(args)...));

/// Overload for a regular member object pointer
template <typename Owner, typename T, typename This>
constexpr decltype(auto) invoke(T(Owner::*memptr), This&& self) INVOKE_BODY(self.*memptr);

/// Overload (group) of member function pointers
#define DECL_MEMFUN_INV(Qual)                                                                      \
    template <typename Ret, typename Owner, typename... FunArgs, typename This, typename... Args>  \
    constexpr decltype(auto) invoke(Ret (Owner::*memfun)(FunArgs...) Qual,                         \
                                    This&& self,                                                   \
                                    Args&&... args)                                                \
        INVOKE_BODY((NEO_FWD(self).*memfun)(NEO_FWD(args)...))

DECL_MEMFUN_INV();
DECL_MEMFUN_INV(const);
DECL_MEMFUN_INV(volatile);
DECL_MEMFUN_INV(const volatile);
DECL_MEMFUN_INV(noexcept);
DECL_MEMFUN_INV(const noexcept);
DECL_MEMFUN_INV(volatile noexcept);
DECL_MEMFUN_INV(const volatile noexcept);

DECL_MEMFUN_INV(&);
DECL_MEMFUN_INV(const&);
DECL_MEMFUN_INV(volatile&);
DECL_MEMFUN_INV(const volatile&);
DECL_MEMFUN_INV(&noexcept);
DECL_MEMFUN_INV(const& noexcept);
DECL_MEMFUN_INV(volatile& noexcept);
DECL_MEMFUN_INV(const volatile& noexcept);

DECL_MEMFUN_INV(&&);
DECL_MEMFUN_INV(const&&);
DECL_MEMFUN_INV(volatile&&);
DECL_MEMFUN_INV(const volatile&&);
DECL_MEMFUN_INV(&&noexcept);
DECL_MEMFUN_INV(const&& noexcept);
DECL_MEMFUN_INV(volatile&& noexcept);
DECL_MEMFUN_INV(const volatile&& noexcept);

#undef DECL_MEMFUN_INV

// Overload for reference-wrapper-like objects
template <template <class> class RefWrapper, typename Inner, typename... Args>
constexpr decltype(auto) invoke(RefWrapper<Inner> ref, Args&&... args)
    INVOKE_BODY(neo::invoke(ref.get(), NEO_FWD(args)...));

#undef INVOKE_BODY

template <typename Fn, typename... Args>
concept invocable2 = requires(Fn&& fn, Args&&... args) {
    neo::invoke(NEO_FWD(fn), NEO_FWD(args)...);
};

template <typename Func, typename... Args>
using invoke_result_t = decltype(neo::invoke(NEO_DECLVAL(Func), NEO_DECLVAL(Args)...));

}  // namespace neo
