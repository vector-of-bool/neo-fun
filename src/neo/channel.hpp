#pragma once

#include "./channel_fwd.hpp"

#include "./attrib.hpp"
#include "./coroutine.hpp"
#include "./opt_ref.hpp"
#include "./optional.hpp"
#include "./type_traits.hpp"

namespace neo {

namespace detail {

template <typename Traits>
struct channel_types;

template <typename Traits, typename Types = channel_types<Traits>>
class channel_promise;

}  // namespace detail

template <typename Traits>
class basic_channel {
private:
    using _types = detail::channel_types<Traits>;

    template <typename, typename>
    friend class detail::channel_promise;

public:
    using promise_type = detail::channel_promise<Traits>;
    using yielded      = _types::yielded;
    using sent         = _types::sent;
    using returned     = _types::returns;

    /**
     * @brief Returns `true` if the channel has co_returned
     */
    bool done() const noexcept { return _coro.promise().has_returned(); }
    /**
     * @brief Obtain the most-recently yielded value from the channel.
     *
     * The yielded type is a reference derived from the yield type of the channel
     */
    yielded current() const noexcept
        requires(not neo_is_void(yielded))
    {
        return static_cast<yielded>(_coro.promise().get_yielded());
    }

    void send(typename _types::send_param arg)
        requires(not _types::send_is_void)
    {
        _coro.promise().send_value(NEO_FWD(arg));
    }

    template <typename S>
        requires convertible_to<S, typename _types::send_param>
    void send(S&& arg)
        requires(not _types::send_is_void)
    {
        _coro.promise().send_value(arg);
    }

    void send()
        requires(!!_types::send_is_void)
    {
        _coro.promise().send_value(nullptr);
    }

    typename Traits::return_type return_value() const noexcept {
        return _coro.promise().returned_value();
    }

    ~basic_channel() {
        if (_coro) {
            _coro.destroy();
        }
    }

    basic_channel(basic_channel&& o) noexcept
        : _coro(o._coro) {
        o._coro = nullptr;
    }

    basic_channel& operator=(basic_channel&& o) noexcept {
        if (_coro) {
            _coro.destroy();
        }
        _coro   = o._coro;
        o._coro = nullptr;
        return *this;
    }

private:
    using _handle_type = std::coroutine_handle<promise_type>;
    _handle_type _coro;

    explicit basic_channel(_handle_type h) noexcept
        : _coro(h) {}
};

template <typename Yield, typename Send, typename Return>
class channel : public basic_channel<default_channel_traits<Yield, Send, Return>> {
    using base_type = basic_channel<default_channel_traits<Yield, Send, Return>>;
    using base_type::base_type;

public:
    channel(base_type&& o) noexcept
        : channel::basic_channel(NEO_MOVE(o)) {}
};

template <typename C>
class from_channel {
    // The wrapped channel
    C _arg;

    template <typename, typename>
    friend class detail::channel_promise;

public:
    explicit from_channel(C&& c) noexcept
        : _arg(NEO_FWD(c)) {}

    using yielded = remove_cvref_t<C>::yielded;
    using sent    = remove_cvref_t<C>::sent;
};

template <typename C>
explicit from_channel(C&&) -> from_channel<C>;

template <typename Yield, typename Send, typename Return>
struct default_channel_traits {
    using yield_type  = Yield;
    using send_type   = Send;
    using return_type = Return;

    using yield_value_type = void;
    using send_value_type  = void;
};

namespace detail {

struct voidish {
    voidish() = default;
    constexpr voidish(decltype(nullptr)) noexcept {}
    constexpr voidish operator*() const noexcept { return {}; }
};

template <bool IsVoid, bool IsReference, bool IsSmartRef>
struct chan_type_helper;

// Case: Void types
template <>
struct chan_type_helper<true, false, false> {
    template <typename>
    using present_as = void;
    template <typename>
    using forward_as = void;
    template <typename>
    using param = decltype(nullptr);
    template <typename>
    using transit = voidish;
    template <typename>
    using opt = voidish;
};

// Case: Language references
template <>
struct chan_type_helper<false, true, false> {
    template <typename T>
    using present_as = T&;
    template <typename T>
    using forward_as = T&&;
    template <typename T>
    using param = T;
    template <typename T>
    using transit = T&;
    template <typename T>
    using opt = opt_ref<remove_reference_t<T>>;
};

// Case: Non-references
template <>
struct chan_type_helper<false, false, false> {
    template <typename T>
    using present_as = T&;
    template <typename T>
    using forward_as = T&&;
    template <typename T>
    using param = const T&;
    template <typename T>
    using transit = T&;
    template <typename T>
    using opt = opt_ref<T>;
};

// Case: Fancy references
template <>
struct chan_type_helper<false, false, true> {
    template <typename T>
    using present_as = T;
    template <typename T>
    using forward_as = T;
    template <typename T>
    using param = T;
    template <typename T>
    using transit = T;
    template <typename T>
    using opt = std::optional<T>;
};

template <typename Traits>
struct channel_types {
    using yield_arg = Traits::yield_type;
    using send_arg  = Traits::send_type;
    enum {
        yield_is_smartref = not neo_is_void(typename Traits::yield_value_type),
        send_is_smartref  = not neo_is_void(typename Traits::send_value_type),
        yield_is_void     = neo_is_void(yield_arg),
        send_is_void      = neo_is_void(send_arg),
        yield_is_ref      = neo_is_reference(yield_arg),
        send_is_ref       = neo_is_reference(send_arg),
    };

    using _yield = chan_type_helper<yield_is_void, yield_is_ref, yield_is_smartref>;
    using _send  = chan_type_helper<send_is_void, send_is_ref, send_is_smartref>;

    // The forwarding-reference type (returned from co_yield and take())
    using yield_fwd = _yield::template forward_as<yield_arg>;
    using send_fwd  = _send::template forward_as<send_arg>;

    // The types presented on the API surface
    // yielded is an lvalue-reference, since it can be requested multiple times
    using yielded = _yield::template present_as<yield_arg>;
    // co_yield always forwards, since it cannot be requested more than once
    using sent = send_fwd;

    // The parameter types (for send_value and yield_value, guards against void too)
    using yield_param = _yield::template param<yield_arg>;
    using send_param  = _send::template param<send_arg>;

    // The type of reference stored in-transit
    using yield_transit = _yield::template transit<yield_arg>;
    using send_transit  = _send::template transit<send_arg>;

    // The optional-reference type
    using yield_opt = _yield::template opt<yield_arg>;
    using send_opt  = _send::template opt<send_arg>;

    using returns = Traits::return_type;
};

template <typename ReturnVal>
struct return_part {
    neo::nano_opt<ReturnVal> _return_value;
    template <neo::convertible_to<ReturnVal> U>
    constexpr void return_value(U&& arg) noexcept(nothrow_constructible_from<ReturnVal, U>) {
        _return_value.emplace(NEO_MOVE(arg));
    }
    constexpr bool       has_returned() const noexcept { return _return_value.has_value(); }
    constexpr ReturnVal& returned_value() noexcept { return _return_value.get(); }
};

template <reference_type ReturnRef>
struct return_part<ReturnRef> {
    neo::opt_ref<remove_reference_t<ReturnRef>> _return_value;
    template <neo::convertible_to<ReturnRef> U>
    constexpr void return_value(U&& arg) noexcept(nothrow_constructible_from<ReturnRef, U>) {
        _return_value = arg;
    }
    constexpr bool      has_returned() const noexcept { return !!_return_value; }
    constexpr ReturnRef returned_value() const noexcept {
        return static_cast<ReturnRef>(*_return_value);
    }
};

template <>
struct return_part<void> {
    bool           _did_return = false;
    constexpr void return_void() noexcept { _did_return = true; }
    constexpr bool has_returned() const noexcept { return _did_return; }
    constexpr void returned_value() noexcept {}
};

template <typename YieldTransit, typename SendTransit>
struct channel_promise_base {
    /**
     * @brief Sends a value and resumes the innermost channel
     *
     * @return true If the channel returned
     * @return false Otherwise
     */
    virtual bool do_send_value(SendTransit) = 0;
    /**
     * @brief Obtain the yielded value
     */
    virtual YieldTransit do_get_yielded() noexcept = 0;
};

template <typename Traits, typename Types>
class channel_promise : public channel_promise_base<typename Types::yield_transit,  //
                                                    typename Types::send_transit>,
                        public return_part<typename Types::returns> {
    /// The channel associated with this promise
    using channel_type = basic_channel<Traits>;
    /// The coroutine handle type
    using handle_type = std::coroutine_handle<channel_promise>;
    /// Types for this channel
    using types = channel_types<Traits>;
    /// Base class, used for delegation
    using base_type
        = channel_promise_base<typename Types::yield_transit, typename Types::send_transit>;

    /// The recently yielded value
    NEO_NO_UNIQUE_ADDRESS typename types::yield_opt _yielded;
    /// The recently sent value
    NEO_NO_UNIQUE_ADDRESS typename types::send_opt _sent;

    bool do_send_value(typename types::send_transit arg) noexcept override final {
        if (_inner) {
            // Delegating to an inner coroutine:
            bool inner_finished = _inner->do_send_value(arg);
            if (inner_finished) {
                // The inner channel finished, so now we resume ourself
                handle().resume();
                return handle().done();
            }
            // Not done yet
            return false;
        } else {
            // We're the leaf channel
            this->_sent = arg;
            handle().resume();
            return handle().done();
        }
    }

    typename types::yield_transit do_get_yielded() noexcept override final {
        if (_inner) {
            // Ask the inner channel:
            return _inner->do_get_yielded();
        }
        return *_yielded;
    }

    base_type* _inner = nullptr;

public:
    // Run to the first yield expression first:
    std::suspend_never  initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }
    handle_type         handle() noexcept { return handle_type::from_promise(*this); }
    void                unhandled_exception() const { throw; }
    channel_type        get_return_object() noexcept { return channel_type(handle()); }

    // Awaiter for regular yield expressions
    struct yield_awaiter {
        channel_promise& self;

        constexpr bool                 await_ready() const noexcept { return false; }
        constexpr typename types::sent await_resume() const noexcept {
            // Return the value that was sent to the coroutine upon resume:
            self._yielded = {};
            return static_cast<types::sent>(*self._sent);
        }

        constexpr void await_suspend(auto) const noexcept { self._sent = {}; }
    };

    // Yielding a value:
    yield_awaiter yield_value(typename types::yield_param p) noexcept {
        _yielded = p;
        return yield_awaiter{*this};
    }

    // Yielding a value, which can be converted to the yield reference:
    template <convertible_to<typename types::yield_param> Y>
    yield_awaiter yield_value(Y&& y) noexcept {
        _yielded = y;
        return yield_awaiter{*this};
    }

    // Awaiter for inner channels:
    template <typename Promise>
    struct nested_awaiter {
        channel_promise& self;
        // The inner channel that we are waiting for
        Promise& other;

        constexpr bool await_ready() const noexcept { return other.has_returned(); }
        constexpr void await_suspend(auto) const noexcept { self._inner = &other; }

        constexpr decltype(auto) await_resume() const noexcept {
            self._inner   = nullptr;
            self._yielded = {};
            return other.returned_value();
        }
    };

    template <typename Ch, typename Promise = remove_reference_t<Ch>::promise_type>
        requires convertible_to<typename Ch::yielded, typename types::yielded>
    auto yield_value(from_channel<Ch> ch) noexcept {
        Promise& pr = ch._arg._coro.promise();
        return nested_awaiter<Promise>{*this, pr};
    }

    typename types::yield_transit get_yielded() const noexcept {
        if (_inner) {
            return _inner->do_get_yielded();
        }
        return *_yielded;
    }

    void send_value(typename types::send_transit s) { do_send_value(s); }
};

}  // namespace detail

}  // namespace neo
