#pragma once

namespace neo {

/**
 * @brief A bidirectional communication channel coroutine type.
 *
 * @tparam Yield The type that will be produced by the coroutine, accessed using the `current()`
 *      method.
 * @tparam Send The type that will be sent to the coroutine, delivered by the `send` method, and
 *      the return value of plain co_yield expressions
 * @tparam Return The type that should be co_returned from the coroutine. Accessible
 *      from the invoker as the `return_value()` method.
 *
 * Any of the above template parameters may be `void`, with the following notes:
 *
 * • If the `Yield` parameter is void, then the `co_yield` expressions must yield
 *   a literal zero "0", and the `current()` method will return void.
 * • If the `Send` parameter is void, then the `co_yield` expression will have
 *   type `void`, and `send()` must be called with no arguments.
 * • If the `Return` parameter is void, then the `return_value()` function will
 *   also have a return type of `void`.
 *
 * Channel coroutines may also co_yield other channels, using the `from_channel`
 * wrapper.
 */
template <typename Yield = void, typename Send = void, typename Return = void>
class channel;

template <typename Yield, typename Send, typename Return>
class channel_pipe;

/**
 * @brief Generate a co_yield-able expression that can be used to delegate to
 * another channel.
 *
 * @tparam C The channel type to yield from
 *
 * The yielded channel must have compatible send and yield types. The return
 * value from the inner channel will be the result of the co_yield expression
 * in the delegating coroutine.
 */
template <typename C>
class from_channel;

namespace _channel_detail {

template <typename Y, typename S, typename R>
class promise;

template <typename, typename>
struct promise_yield_send;

}  // namespace _channel_detail

}  // namespace neo
