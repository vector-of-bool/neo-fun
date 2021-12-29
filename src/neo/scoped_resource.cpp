#include "./scoped_resource.hpp"

#include <neo/assert.hpp>

void neo::sr_detail::assert_coro_yield_once(std::coroutine_handle<> coro) {
    neo_assert(expects,
               coro.done(),
               "neo::scoped_resource<> coroutine suspended more than once. A scoped_resource<> "
               "coroutine must yield *exactly* once and then co_return.");
}

void neo::sr_detail::assert_coro_did_yield(std::coroutine_handle<> coro, const void* yielded_value) {
    neo_assert(expects,
               !coro.done() && yielded_value != nullptr,
               "neo::scoped_resource<> coroutine did not yield a value before for its initial "
               "suspension.");
}
