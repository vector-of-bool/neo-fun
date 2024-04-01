#pragma once

#include "./declval.hpp"
#include "./fwd.hpp"
#include "./meta.hpp"
#include "./returns.hpp"
#include "./type_traits.hpp"

namespace neo {

/**
 * @brief A type which is suitable for invocation via regular function-call syntax
 *
 * @tparam Fn A function or function object
 * @tparam Args Arguments to give to the function
 */
template <typename Fn, typename... Args>
concept simple_invocable = requires(Fn&& fn, Args&&... args) {
    NEO_FWD(fn)
    (NEO_FWD(args)...);
};

namespace invoke_detail {

struct memfun_invoker {

/// Overload (group) of member function pointers
#define DECL_MEMFUN_INV(Qual)                                                                      \
    template <typename Ret, typename Owner, typename... FunArgs, typename This, typename... Args>  \
    static constexpr auto invoke(Ret (Owner::*memfun)(FunArgs...) Qual,                            \
                                 This&& self,                                                      \
                                 Args&&... args)                                                   \
        NEO_RETURNS((NEO_FWD(self).*memfun)(NEO_FWD(args)...))

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
    DECL_MEMFUN_INV(& noexcept);
    DECL_MEMFUN_INV(const& noexcept);
    DECL_MEMFUN_INV(volatile& noexcept);
    DECL_MEMFUN_INV(const volatile& noexcept);

    DECL_MEMFUN_INV(&&);
    DECL_MEMFUN_INV(const&&);
    DECL_MEMFUN_INV(volatile&&);
    DECL_MEMFUN_INV(const volatile&&);
    DECL_MEMFUN_INV(&& noexcept);
    DECL_MEMFUN_INV(const&& noexcept);
    DECL_MEMFUN_INV(volatile&& noexcept);
    DECL_MEMFUN_INV(const volatile&& noexcept);

#undef DECL_MEMFUN_INV
};

struct memobj_invoker {
    template <typename MemPtr, typename Self>
    constexpr static auto invoke(MemPtr memptr, Self&& self) NEO_RETURNS(NEO_FWD(self).*memptr);
};

struct call_invoker {
    template <typename Function, typename... Args>
        requires simple_invocable<Function&&, Args&&...>
    constexpr static auto invoke(Function&& fn, Args&&... args)
        NEO_RETURNS(NEO_FWD(fn)(NEO_FWD(args)...));
};

template <bool IsMemberPointer>
struct pick_invoker_1;

template <>
struct pick_invoker_1<false> {
    // Invocable is not a member pointer, so defer to the regular calling semantics
    template <typename>
    using f = call_invoker;
};

template <>
struct pick_invoker_1<true> {
    // Invocable is a pointer-to-member.
    template <typename Ptr>
    using f = conditional_t<member_function_pointer<Ptr>,
                            // Invoke as a member function
                            memfun_invoker,
                            // "Invoke" as member object
                            memobj_invoker>;
};

template <typename Fn, typename D = remove_cvref_t<Fn>>
using pick_invoker_t = pick_invoker_1<member_pointer<D>>::template f<D>;

}  // namespace invoke_detail

struct invoke_fn {
    // Overload for reference-wrapper-like objects
    template <template <class> class RefWrapper, typename Inner, typename... Args>
    constexpr auto operator()(RefWrapper<Inner> ref, Args&&... args) const
        NEO_RETURNS((*this)(ref.get(), NEO_FWD(args)...));

    template <typename Func, typename... Args>
    constexpr auto operator()(Func&& fn, Args&&... args) const
        NEO_RETURNS(invoke_detail::pick_invoker_t<Func>::invoke(NEO_FWD(fn), NEO_FWD(args)...));
};

/**
 * @brief "Invoke" an invocable object. Like std::invoke, but cleaner and less debug overhead
 *
 * @tparam Func The invocable. Must be a function, or object that has an operator() overload, or a
 * pointer-to-member.
 * @tparam Args The arguments to pass to the invocable.
 */
inline constexpr invoke_fn invoke{};

#define NEO_INVOKE(F, ...)                                                                         \
    (::neo::invoke_detail::pick_invoker_t<decltype(F)>::invoke(F __VA_OPT__(, ) __VA_ARGS__))

template <typename Fn, typename... Args>
concept invocable2 = simple_invocable<Fn, Args...>
    or requires(Fn&& fn, Args&&... args) { neo::invoke(NEO_FWD(fn), NEO_FWD(args)...); };

/**
 * @brief The result type of a neo::invoke with the given argument types
 *
 * @tparam Func An invocable object
 * @tparam Args Arguments for the invocable
 */
template <typename Func, typename... Args>
    requires invocable2<Func, Args...>
using invoke_result_t = decltype(NEO_INVOKE(NEO_DECLVAL(Func), NEO_DECLVAL(Args)...));

}  // namespace neo
