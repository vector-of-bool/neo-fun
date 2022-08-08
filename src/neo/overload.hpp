#pragma once

#include "./attrib.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./ref_member.hpp"
#include "./returns.hpp"

#include <type_traits>

namespace neo {

namespace detail {

template <typename Func>
class overload_fn {
    NEO_NO_UNIQUE_ADDRESS neo::wrap_ref_member_t<Func> _func;

public:
    constexpr explicit overload_fn(Func&& fn) noexcept(noexcept(Func(NEO_FWD(fn))))
        : _func(NEO_FWD(fn)) {}

    template <typename... Args>
    constexpr auto operator()(Args&&... args) NEO_RETURNS(neo::invoke(_func, NEO_FWD(args)...));

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const
        NEO_RETURNS(neo::invoke(_func, NEO_FWD(args)...));
};

}  // namespace detail

/**
 * @brief Create an overload set from one or more invocable objects.
 *
 * @tparam Funcs The invocable objects. Use CTAD to deduce these.
 */
template <typename... Funcs>
class overload : public detail::overload_fn<Funcs>... {
public:
    constexpr overload() = default;

    constexpr explicit overload(Funcs&&... args) noexcept(
        (noexcept(detail::overload_fn<Funcs>(NEO_FWD(args))) and ...))
        : detail::overload_fn<Funcs>(NEO_FWD(args))... {}

    using detail::overload_fn<Funcs>::operator()...;
};

template <>
class overload<> {};

template <typename... Fs>
explicit overload(Fs&&...) -> overload<Fs...>;

namespace detail {

template <typename... Funcs>
struct ordered_overload_impl;

template <>
struct ordered_overload_impl<> {};

template <typename Fn, typename... Tail>
struct ordered_overload_impl<Fn, Tail...> : ordered_overload_impl<Tail...> {
    using base_type = ordered_overload_impl<Tail...>;

    NEO_NO_UNIQUE_ADDRESS neo::wrap_ref_member_t<Fn> _my_fn;

    // Default constructor
    constexpr ordered_overload_impl() = default;

    // Construct this invocable, and its parent
    constexpr explicit ordered_overload_impl(Fn&& fn, Tail&&... tail) noexcept(
        std::is_nothrow_constructible_v<base_type, Tail...>and
            std::is_nothrow_constructible_v<Fn, Fn>)
        : base_type(NEO_FWD(tail)...)
        , _my_fn(NEO_FWD(fn)) {}

    // clang-format off
    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) const
        noexcept(neo::invocable2<const Fn&, Args&&...>
                     ? std::is_nothrow_invocable_v<const Fn&, Args&&...>
                     : std::is_nothrow_invocable_v<const base_type&, Args&&...>)
        requires neo::invocable2<const Fn&, Args&&...>
              or neo::invocable2<const base_type&, Args&&...>
    {
        if constexpr (neo::invocable2<const Fn&, Args&&...>) {
            return neo::invoke(neo::unref_member(_my_fn), NEO_FWD(args)...);
        } else {
            return base_type::operator()(NEO_FWD(args)...);
        }
    }

    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args)
        noexcept(neo::invocable2<Fn&, Args&&...>
                     ? std::is_nothrow_invocable_v<Fn&, Args&&...>
                     : std::is_nothrow_invocable_v<base_type&, Args&&...>)
        requires neo::invocable2<Fn&, Args&&...>
              or neo::invocable2<base_type&, Args&&...>
    {
        if constexpr (neo::invocable2<Fn&, Args&&...>) {
            return neo::invoke(neo::unref_member(_my_fn), NEO_FWD(args)...);
        } else {
            return base_type::operator()(NEO_FWD(args)...);
        }
    }
    // clang-format on
};

}  // namespace detail

/**
 * @brief Create an "ordered" overload: If more than one of the given invocables is invocable for a
 * given argument list, the first matching invocable will be invoked.
 */
template <typename... Fns>
struct ordered_overload : detail::ordered_overload_impl<Fns...> {
    using ordered_overload::ordered_overload_impl::ordered_overload_impl;
};

template <typename... Fns>
explicit ordered_overload(Fns&&...) -> ordered_overload<Fns...>;

}  // namespace neo
