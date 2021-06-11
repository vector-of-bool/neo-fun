#pragma once

#include "./declval.hpp"
#include "./fwd.hpp"

namespace neo {

namespace tl_detail {

class no_argument_provided {
    ~no_argument_provided()                      = default;
    no_argument_provided()                       = default;
    no_argument_provided(no_argument_provided&&) = delete;

public:
    static no_argument_provided instance;
};

inline no_argument_provided no_argument_provided::instance;

template <int>
constexpr no_argument_provided& nth_arg() noexcept {
    return no_argument_provided::instance;
}

template <int I, typename A, typename... Tail>
constexpr decltype(auto) nth_arg(A&& first [[maybe_unused]], Tail&&... tail [[maybe_unused]]) {
    if constexpr (I == 0) {
        return NEO_FWD(first);
    } else {
        return nth_arg<I - 1>(NEO_FWD(tail)...);
    }
}

template <int I, typename... TlArgs>
using nth_arg_t = decltype(nth_arg<I>(NEO_DECLVAL(TlArgs)...));

}  // namespace tl_detail

#define NEO_TL_REQUIRES(...)                                                                       \
    requires(::neo::tl_detail::nth_arg_t<0, TlArgs...> _1 [[maybe_unused]],                        \
             ::neo::tl_detail::nth_arg_t<1, TlArgs...> _2 [[maybe_unused]],                        \
             ::neo::tl_detail::nth_arg_t<2, TlArgs...> _3 [[maybe_unused]],                        \
             ::neo::tl_detail::nth_arg_t<3, TlArgs...> _4 [[maybe_unused]]) {                      \
        __VA_ARGS__;                                                                               \
    }
// clang-format on

// Roughly based on https://github.com/Quincunx271/TerseLambda
#define NEO_CTL(...)                                                                               \
    <typename... TlArgs>(TlArgs && ... _args) noexcept(NEO_TL_REQUIRES({ __VA_ARGS__ } noexcept))  \
        ->decltype(auto) requires(NEO_TL_REQUIRES(__VA_ARGS__)) {                                  \
        [[maybe_unused]] auto&& _1 = ::neo::tl_detail::nth_arg<0>(NEO_FWD(_args)...);              \
        [[maybe_unused]] auto&& _2 = ::neo::tl_detail::nth_arg<1>(NEO_FWD(_args)...);              \
        [[maybe_unused]] auto&& _3 = ::neo::tl_detail::nth_arg<2>(NEO_FWD(_args)...);              \
        [[maybe_unused]] auto&& _4 = ::neo::tl_detail::nth_arg<3>(NEO_FWD(_args)...);              \
        return (__VA_ARGS__);                                                                      \
    }

#define NEO_TL [&] NEO_CTL

}  // namespace neo
