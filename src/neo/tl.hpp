#pragma once

#include "./fwd.hpp"

#include <cstddef>
#include <utility>

namespace neo {

namespace tl_detail {

struct no_argument_provided {};

template <std::size_t>
constexpr no_argument_provided nth_arg() noexcept {
    return {};
}

template <std::size_t I, typename A = no_argument_provided, typename... Tail>
constexpr decltype(auto) nth_arg(A&& first, Tail&&... tail) {
    if constexpr (I == 0) {
        return NEO_FWD(first);
    } else {
        return nth_arg<I - 1>(NEO_FWD(tail)...);
    }
}

template <std::size_t I, typename... Args>
using nth_arg_t = decltype(nth_arg<I>(std::declval<Args>()...));

}  // namespace tl_detail

// clang-format off
#define NEO_TL_REQS_ARGS                            \
    ::neo::tl_detail::nth_arg_t<0, Args...> && _1,  \
    ::neo::tl_detail::nth_arg_t<1, Args...> && _2,  \
    ::neo::tl_detail::nth_arg_t<2, Args...> && _3,  \
    ::neo::tl_detail::nth_arg_t<3, Args...> && _4

#define NEO_TL_IS_NOEXCEPT(...) requires(NEO_TL_REQS_ARGS) { {__VA_ARGS__} noexcept; }
// clang-format on

// Roughly based on https://github.com/Quincunx271/TerseLambda
#define NEO_CTL(...)                                                                               \
    <typename... Args>(Args && ... args) mutable noexcept(NEO_TL_IS_NOEXCEPT(__VA_ARGS__))         \
        ->decltype(auto) requires(requires(NEO_TL_REQS_ARGS) { {__VA_ARGS__}; }) {                 \
        [[maybe_unused]] auto&& _1 = ::neo::tl_detail::nth_arg<0>(NEO_FWD(args)...);               \
        [[maybe_unused]] auto&& _2 = ::neo::tl_detail::nth_arg<1>(NEO_FWD(args)...);               \
        [[maybe_unused]] auto&& _3 = ::neo::tl_detail::nth_arg<2>(NEO_FWD(args)...);               \
        [[maybe_unused]] auto&& _4 = ::neo::tl_detail::nth_arg<3>(NEO_FWD(args)...);               \
        return (__VA_ARGS__);                                                                      \
    }

#define NEO_TL [&] NEO_CTL

}  // namespace neo
