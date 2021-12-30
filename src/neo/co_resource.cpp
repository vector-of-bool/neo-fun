#if __cpp_impl_coroutine

#include "./co_resource.hpp"

#include <neo/assert.hpp>

void neo::cor_detail::assert_coro_yield_once(std::coroutine_handle<> coro) noexcept {
    neo_assert(expects,
               coro.done(),
               "neo::scoped_resource<> coroutine suspended more than once. A scoped_resource<> "
               "coroutine must yield *exactly* once and then co_return.");
    neo::unreachable();
}

void neo::cor_detail::assert_coro_did_yield(std::coroutine_handle<> coro,
                                           const void*             yielded_value) noexcept {
    neo_assert(expects,
               !coro.done() && yielded_value != nullptr,
               "neo::scoped_resource<> coroutine did not yield a value before for its initial "
               "suspension.");
    neo::unreachable();
}

#endif