#pragma once

#include "./declval.hpp"

#include "./overload.hpp"
#include "./tl.hpp"

#include <coroutine>

namespace neo {

namespace awt_detail {

template <typename T>
concept awaiter = requires(T&& f, std::coroutine_handle<> co) {
    f.await_ready();
    f.await_suspend(co);
    f.await_resume();
};

constexpr inline auto get_awaiter_impl = neo::ordered_overload{
    [](auto&& arg) NEO_RETURNS_L(NEO_FWD(arg).operator co_await()),
    [](auto&& arg) NEO_RETURNS_L(operator co_await(NEO_FWD(arg))),
    [](awaiter auto&& arg) NEO_RETURNS_L(NEO_FWD(arg)),
};

}  // namespace awt_detail

/**
 * @brief Detect a type valid for co_await
 *
 * @note A coroutine may change the `co_await` operator with await_transform()
 *
 * @tparam T A type to check.
 *
 * A type is `awaitable` if it has a member `operator co_await`, non-member ADL-visible `operator
 * co_await`, or is itself an `awaiter`.
 *
 * A type is an `awaiter` if it provides all of `await_ready`, `await_suspend`, and `await_resume`
 */
template <typename T>
concept awaitable = requires(T&& arg) {
    { awt_detail::get_awaiter_impl(NEO_FWD(arg)) } -> awt_detail::awaiter;
};

template <typename T>
struct awaitable_traits {};

template <awaitable T>
struct awaitable_traits<T> {
    using awaiter_type      = decltype(awt_detail::get_awaiter_impl(NEO_DECLVAL(T)));
    using await_result_type = decltype(NEO_DECLVAL(awaiter_type).await_resume());
};

/// Obtain the awaiter type of the given awaitable type
template <typename T>
using awaiter_t = typename awaitable_traits<T>::awaiter_type;

/// Obtain the result of a `co_await` expression on the given awaitable type
template <typename T>
using await_result_t = typename awaitable_traits<T>::await_result_type;

}  // namespace neo
