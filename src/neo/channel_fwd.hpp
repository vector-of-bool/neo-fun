#pragma once

namespace neo {

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

}  // namespace neo
