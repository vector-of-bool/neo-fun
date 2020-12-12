#pragma once

#include "./fwd.hpp"

#include <utility>

namespace neo {

#define INVOKE_BODY(...)                                                                           \
    noexcept(noexcept(__VA_ARGS__)) requires requires { __VA_ARGS__; }                             \
    { return __VA_ARGS__; }                                                                        \
    static_assert(true)

template <typename Func, typename... Args>
constexpr decltype(auto) invoke(Func&& fn, Args&&... args)
    INVOKE_BODY(NEO_FWD(fn)(NEO_FWD(args)...));

template <typename Owner, typename T, typename This>
constexpr decltype(auto) invoke(T(Owner::*memptr), This&& self) INVOKE_BODY(self.*memptr);

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

template <template <class> class RefWrapper, typename Inner, typename... Args>
constexpr decltype(auto) invoke(RefWrapper<Inner> ref, Args&&... args)
    INVOKE_BODY(neo::invoke(ref.get(), NEO_FWD(args)...));

#undef INVOKE_BODY

template <typename Func, typename... Args>
using invoke_result_t = decltype(neo::invoke(std::declval<Func>(), std::declval<Args>()...));

template <typename Func, typename... Args>
concept invocable = requires(Func fn, Args... args) {
    neo::invoke(NEO_FWD(fn), NEO_FWD(args)...);
};

template <typename Func, typename... Args>
concept nothrow_invocable = invocable<Func, Args...>&& requires(Func fn, Args... args) {
    { neo::invoke(NEO_FWD(fn), NEO_FWD(args)...) }
    noexcept;
};

}  // namespace neo
