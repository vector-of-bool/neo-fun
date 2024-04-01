#pragma once

#include "./channel.detail.hpp"

#include <neo/attrib.hpp>
#include <neo/channel_fwd.hpp>
#include <neo/type_traits.hpp>
#include <neo/unit.hpp>

#include <coroutine>
#include <type_traits>

namespace neo {

/**
 * @brief A bidirectional communication channel coroutine type.
 *
 * The channel starts in a suspended state, and may be launched by calling `open()` to
 * get a `channel_pipe`, or `begin()` to return an iterator (only available for some
 * configurations of the channel).
 *
 * @tparam Yield The type that will be produced by the coroutine, accessed using the pipe's
 *      `current()`/`take_current()` method.
 * @tparam Send The type that will be sent to the coroutine, delivered by the pipe `send` method,
 *      and the return value of plain `co_yield` expressions within the coroutine.
 * @tparam Return The type that should be `co_returned` from the coroutine. Accessible
 *      from the invoker as the `return_value()` method on the channel pipe.
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
template <typename Yield, typename Send, typename Return>
class channel {
public:
    /// The type given for the Yield template parameter
    using yield_type = Yield;
    /// The type given for the Send template parameter
    using send_type = Send;
    /// The type given for the Return template parameter
    using return_type = Return;
    // The promise type of the channel
    using promise_type = _channel_detail::promise<yield_type, send_type, return_type>;

    /**
     * @brief The type returned by `channel::open()`, used for communicating with
     * the channel.
     */
    using pipe_type = channel_pipe<yield_type, send_type, return_type>;

public:
    /**
     * @brief Launch the channel and begin execution.
     *
     * The behavior of calling this function more than once is undefined
     *
     * @return pipe_type A new active channel pipe
     */
    pipe_type open() {
        // Initiate the channel:
        _coro.resume();
        // The channel is now paused and ready with its first value (or immediately done)
        return pipe_type(_coro);
    }

    // Destroy
    NEO_CONSTEXPR_DESTRUCTOR ~channel() {
        if (_coro) {
            _coro.destroy();
        }
    }

    // Move-construct
    constexpr channel(channel&& o) noexcept
        : _coro(o._coro) {
        o._coro = nullptr;
    }

    // Move-assign
    constexpr channel& operator=(channel&& o) noexcept {
        if (_coro) {
            _coro.destroy();
        }
        _coro   = o._coro;
        o._coro = nullptr;
        return *this;
    }

    /**
     * @brief Return a from_channel that can be co_yielded to start a nested channel.
     *
     * See `from_channel` for more information.
     */
    constexpr from_channel<channel> operator*() noexcept;

private:
    template <typename>
    friend class from_channel;

    using handle_type = std::coroutine_handle<promise_type>;

public:
    class iterator {
        handle_type _coro;

        friend channel;

        explicit iterator(handle_type h) noexcept
            : _coro(h) {}

        struct sender {
            handle_type co;

            void operator=(nonvoid_t<send_type>&& send) const&& { co.promise().send_value(send); }
        };

    public:
        iterator() = default;

        using difference_type = std::ptrdiff_t;
        using value_type = conditional_t<void_type<send_type>, remove_cvref_t<yield_type>, void>;

        yield_type operator*() const noexcept
            requires void_type<send_type>
        {
            return pipe_type(_coro).take_current();
        }

        sender operator*() const noexcept
            requires(not void_type<send_type>)
        {
            return sender{_coro};
        }

        iterator& operator++() {
            if constexpr (void_type<send_type>) {
                pipe_type(_coro).send();
            }
            return *this;
        }
        iterator operator++(int) {
            ++*this;
            return *this;
        }

        constexpr bool operator==(std::default_sentinel_t) const noexcept { return _coro.done(); }
    };

    iterator begin() {
        _coro.resume();
        return iterator(_coro);
    }

    constexpr std::default_sentinel_t end() const noexcept { return {}; }

    constexpr channel(typename promise_type::init init) noexcept
        : _coro(init.co) {}

private:
    handle_type _coro;
};

/**
 * @brief Communication handle for a neo::channel
 *
 * Do not construct this directly. Use neo::channel::open to obtain an instance
 * of this type.
 */
template <typename Yield, typename Send, typename Return>
class channel_pipe {

public:
    /// The type to send to the channel
    using send_type = Send;
    /// The type yielded by the channel
    using yield_type = Yield;
    /// The final result type of the channel
    using return_type = Return;

    /**
     * @brief Returns `true` if the channel has co_returned normally
     */
    constexpr bool done() const noexcept { return _coro.done(); }

    /**
     * @brief Obtain the most-recently yielded value from the channel.
     *
     * If `yield_type` is `void`, returns `void`. Otherwise, returns an lvalue-reference
     * to the yielded value.
     *
     * @pre `not done()`
     */
    constexpr add_lvalue_reference_t<yield_type> current() const noexcept {
        return static_cast<add_lvalue_reference_t<yield_type>>(_coro.promise().get_yielded());
    }

    /**
     * @brief Take the current object yielded by the channel. May move-construct
     * from the underlying object if the yield type is an object type
     *
     * @pre `not done()`
     */
    constexpr yield_type take_current() noexcept {
        return static_cast<add_rvalue_reference_t<yield_type>>(_coro.promise().get_yielded());
    }

    /**
     * @brief Send a value and resume the channel.
     *
     * Accepts an rvalue-reference to the send type. This overload is only
     * available if the send type is not `void`.
     *
     * @pre `not done()`
     */
    constexpr void send(nonvoid_t<send_type>&& arg)
        requires(not void_type<send_type>)
    {
        _coro.promise().send_value(arg);
    }

    /**
     * @brief Send a value with a possible conversion, and resume the channel
     *
     * @note This overload is only visible if Send IS NOT a reference type.
     *
     * This will always accept a reference-to-const of the underlying type, which
     * results in allowing conversions to the yielded type
     *
     * @param y
     * @return yield_awaiter<remove_cvref_t<yield_forward>>
     *
     * @pre `not done()`
     */
    constexpr void send(nonvoid_t<send_type> const& arg)
        // Require that we are not sending lvalue-references:
        requires(not reference_type<send_type>)
        // Require that we can construct a send value from the const-reference:
        and convertible_to<nonvoid_t<send_type> const&, send_type>
    {
        nonvoid_t<send_type> tmp(arg);
        _coro.promise().send_value(tmp);
    }

    /**
     * @brief Send a void value and resume the channel
     *
     * This overload is only visible if Send is `void`
     *
     * @pre `not done()`
     */
    constexpr void send()
        requires void_type<send_type>
    {
        unit nil;
        _coro.promise().send_value(nil);
    }

    /**
     * @brief Obtain the final return value from the coroutine.
     *
     * If the Return type is void, this function returns void. Otherwise,
     * this function will always return an lvalue-reference.
     *
     * @pre `done()`
     */
    constexpr add_lvalue_reference_t<return_type> return_value() const noexcept {
        return static_cast<add_lvalue_reference_t<return_type>>(_coro.promise().get_returned());
    }

    /**
     * @brief Take the final return value from the coroutine.
     *
     * This returns exactly yield_type, which may be an rvalue.
     *
     * @pre `done()`
     */
    constexpr return_type take_return_value() noexcept {
        return static_cast<add_rvalue_reference_t<return_type>>(_coro.promise().get_returned());
    }

private:
    // Promise type for the coroutine
    using promise = _channel_detail::promise<Yield, Send, Return>;
    // Coroutine handle type for the channel
    using handle_type = std::coroutine_handle<promise>;

    // Allow our channel to call our constructor
    friend channel<yield_type, send_type, return_type>;

    /**
     * @brief Construct a new channel pipe object from a channel's coroutine handle
     */
    constexpr explicit channel_pipe(handle_type co) noexcept
        : _coro(co) {}

private:
    // The coroutine handle for the channel
    handle_type _coro;
};

/**
 * @brief Within a channel coroutine, allows yielding control to a sub-channel.
 *
 * Within a channel coroutine, a `co_yield` expression may accept a `from_channel`
 * as an operand, which tells the coroutine to suspend and delegate further
 * execution to the new channel. Values sent/yielded into the channel will be
 * piped into the child coroutine until it returns. The return value from the
 * child coroutine is the result value of the `co_yield` expression in the
 * parent.
 */
template <typename C>
class from_channel {
    // The wrapped channel coroutine
    typename remove_cvref_t<C>::handle_type _coro;

    friend _channel_detail::from_chan_coro_getter;

public:
    explicit from_channel(const C& c) noexcept
        : _coro(c._coro) {}
};

// CTAD for from_channnel
template <typename C>
explicit from_channel(C const&) -> from_channel<C>;

template <typename Y, typename S, typename R>
constexpr from_channel<channel<Y, S, R>> channel<Y, S, R>::operator*() noexcept {
    return from_channel(*this);
}

}  // namespace neo
