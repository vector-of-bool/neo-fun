#pragma once

#include "./attrib.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./object_box.hpp"
#include "./returns.hpp"
#include "./tag.hpp"

#include <type_traits>
#include <utility>

namespace neo {

namespace detail {

template <typename Func>
class overload_fn {
    NEO_NO_UNIQUE_ADDRESS neo::object_box<Func> _func;

public:
    constexpr overload_fn() = default;

    constexpr explicit overload_fn(Func&& fn) noexcept(noexcept(Func(NEO_FWD(fn))))
        : _func(NEO_FWD(fn)) {}

    template <typename... Args>
    constexpr auto operator()(Args&&... args)
        NEO_RETURNS(NEO_INVOKE(_func.get(), NEO_FWD(args)...));

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const
        NEO_RETURNS(NEO_INVOKE(_func.get(), NEO_FWD(args)...));
};

template <typename Func>
    requires(not neo_is_member_pointer(remove_cvref_t<Func>))
class overload_fn<Func> {
    NEO_NO_UNIQUE_ADDRESS neo::object_box<Func> _func;

public:
    constexpr overload_fn() = default;

    constexpr explicit overload_fn(Func&& fn) noexcept(noexcept(Func(NEO_FWD(fn))))
        : _func(NEO_FWD(fn)) {}

    template <typename... Args>
    constexpr auto operator()(Args&&... args) NEO_RETURNS(_func.get()(NEO_FWD(args)...));

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const NEO_RETURNS(_func.get()(NEO_FWD(args)...));
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

    NEO_NO_UNIQUE_ADDRESS neo::object_box<Fn> _my_fn;

    // Default constructor
    constexpr ordered_overload_impl() = default;

    // Construct this invocable, and its parent
    constexpr explicit ordered_overload_impl(Fn&& fn, Tail&&... tail) noexcept(
        nothrow_constructible_from<base_type, Tail...>and nothrow_constructible_from<Fn, Fn>)
        : base_type(NEO_FWD(tail)...)
        , _my_fn(NEO_FWD(fn)) {}

    // clang-format off
    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) const
        noexcept(nothrow_invocable<const Fn&, Args&&...>)
        requires neo::invocable2<const Fn&, Args&&...>
    {
        return NEO_INVOKE(_my_fn.get(), NEO_FWD(args)...);
    }

    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) const
        noexcept(nothrow_invocable<base_type&, Args&&...>)
        requires (not neo::invocable2<Fn&, Args&&...>)
             and neo::invocable2<base_type&, Args&&...>
    {
        return base_type::operator()(NEO_FWD(args)...);
    }

    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args)
        noexcept(nothrow_invocable<Fn&, Args&&...>)
        requires neo::invocable2<Fn&, Args&&...>
    {
        return NEO_INVOKE(_my_fn.get(), NEO_FWD(args)...);
    }

    template <typename... Args>
    constexpr decltype(auto) operator()(Args&&... args)
        noexcept(nothrow_invocable<base_type&, Args&&...>)
        requires (not neo::invocable2<Fn&, Args&&...>)
             and neo::invocable2<base_type&, Args&&...>
    {
        return base_type::operator()(NEO_FWD(args)...);
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
