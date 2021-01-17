#pragma once

#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./ref.hpp"
#include "./returns.hpp"

namespace neo {

namespace detail {

template <typename Func>
class overload_fn {
    [[no_unique_address]] Func _func;

public:
    constexpr explicit overload_fn(Func&& fn)
        : _func(NEO_FWD(fn)) {}

    template <typename... Args>
    constexpr auto operator()(Args&&... args) NEO_RETURNS(neo::invoke(_func, NEO_FWD(args)...));

    template <typename... Args>
    constexpr auto operator()(Args&&... args) const
        NEO_RETURNS(neo::invoke(_func, NEO_FWD(args)...));
};

}  // namespace detail

template <typename... Funcs>
class overload : public detail::overload_fn<Funcs>... {
public:
    constexpr explicit overload(Funcs&&... args)
        : detail::overload_fn<Funcs>(NEO_FWD(args))... {}

    using detail::overload_fn<Funcs>::operator()...;
};

template <typename... Fs>
explicit overload(const Fs&...) -> overload<Fs...>;

}  // namespace neo
