#pragma once

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

template <typename Yield = void, typename Send = void, typename Return = void>
struct default_channel_traits;

template <typename Traits>
class basic_channel {
private:
    using _types = detail::channel_types<Traits>;

    template <typename, typename>
    friend class detail::channel_promise;

public:
    using promise_type = detail::channel_promise<Traits>;
    using yielded      = _types::yield::presented;
    using sent         = _types::send::presented;
    using returned     = _types::return_;

    bool done() const noexcept { return _coro.promise().has_returned(); }
    typename _types::yield::reference current() const noexcept
        requires(not neo_is_void(yielded))
    {
        return static_cast<yielded>(_coro.promise().get_yielded());
    }

    void send(typename _types::send::nonvoid arg)
        requires(not _types::send::is_void)
    {
        _coro.promise().send_value(NEO_FWD(arg));
    }

    template <typename S>
        requires rvalue_reference_type<sent>
        and constructible_from<remove_cvref_t<sent>, S&&>
    void send(S&& arg)
        requires(not _types::send::is_void)
    {
        _coro.promise().send_value(arg);
    }

    void send()
        requires(_types::send::is_void)
    {
        _coro.promise().send_value(nullptr);
    }

    typename _types::return_lvalue_reference return_value() const noexcept {
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

template <typename Yield = void, typename Send = void, typename Return = void>
class channel : public basic_channel<default_channel_traits<Yield, Send, Return>> {
    using channel::basic_channel::basic_channel;

public:
    channel(typename channel::basic_channel&& o) noexcept
        : channel::basic_channel(NEO_MOVE(o)) {}
};

template <typename C>
class from_channel {
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

    using yield_value_type  = void;
    using send_value_type   = void;
    using return_value_type = void;

    using allocator = std::allocator<std::byte>;
};

namespace detail {

struct void_placeholder {
    constexpr void_placeholder(decltype(nullptr)) noexcept {}
};

template <typename T>
using nonvoid_t = conditional_t<neo_is_void(T), void_placeholder, T>;

// Case: Ref is non-void non-reference type and a Value was explicitly provided
template <typename Ref, typename Value>
struct yield_send_traits {
    // The value type of the channel
    using value = Value;
    // The reference type of the channel (may be a fancy reference)
    using reference = Ref;
    // We present the user with a reference-to-const
    using presented = const Ref&;
    using nonvoid   = presented;
    // The reference type that is used in transit of the value, but not exposed
    // at the surface API
    using transit_reference = Ref;
    // The "optional" of the reference type
    using opt_reference           = std::optional<Ref>;
    constexpr static bool is_void = false;
};

// Case: The type is void
template <>
struct yield_send_traits<void, void> {
    using value     = void;
    using reference = void;
    using presented = void;
    using nonvoid   = decltype(nullptr);
    struct opt_reference {
        opt_reference() = default;
        constexpr opt_reference(decltype(nullptr)) noexcept {}
    };
    using transit_reference       = decltype(nullptr);
    constexpr static bool is_void = true;
};

// Case: Ref is a reference type, and Value was explicitly provided
template <reference_type Ref, typename Value>
    requires(not neo_is_void(Value))
struct yield_send_traits<Ref, Value> {
    // Value is given
    using value = Value;
    // Ref is a language reference
    using reference = Ref;
    // We present as that reference
    using presented         = reference;
    using nonvoid           = presented;
    using transit_reference = Ref&;
    // We store an optional of the referree
    using opt_reference           = neo::opt_ref<remove_reference_t<Ref>>;
    constexpr static bool is_void = false;
};

// Common case: No explicit value type
template <typename Ref>
struct yield_send_traits<Ref, void> {
    using value = remove_cvref_t<Ref>;
    // The reference type in this case is presented by reference collapsing
    using reference               = Ref&&;
    using presented               = reference;
    using nonvoid                 = presented;
    using transit_reference       = reference&;
    using opt_reference           = neo::opt_ref<remove_reference_t<Ref>>;
    constexpr static bool is_void = false;
};

template <typename Traits>
struct channel_types {
    using yield = yield_send_traits<typename Traits::yield_type, typename Traits::yield_value_type>;
    using send  = yield_send_traits<typename Traits::send_type, typename Traits::send_value_type>;

    using return_ = Traits::return_type;

    constexpr static bool returns_void = neo_is_void(return_);

    using return_lvalue_reference = conditional_t<returns_void, void, nonvoid_t<return_>&>;
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
    virtual bool         do_send_value(SendTransit) = 0;
    virtual YieldTransit do_get_yielded() noexcept  = 0;
};

template <typename Traits, typename Types>
class channel_promise : public channel_promise_base<typename Types::yield::transit_reference,  //
                                                    typename Types::send::transit_reference>,
                        public return_part<typename Types::return_> {
    using channel_type = basic_channel<Traits>;
    using handle_type  = std::coroutine_handle<channel_promise>;
    using types        = channel_types<Traits>;
    using base_type    = channel_promise_base<typename Types::yield::transit_reference,
                                           typename Types::send::transit_reference>;

    NEO_NO_UNIQUE_ADDRESS typename types::yield::opt_reference _yielded;
    NEO_NO_UNIQUE_ADDRESS typename types::send::opt_reference  _sent;

    using yielded = types::yield::presented;
    using sent    = types::yield::presented;

    bool do_send_value(typename types::send::transit_reference arg) noexcept override final {
        if (_this_or_inner != this) {
            bool inner_finished = _this_or_inner->do_send_value(arg);
            if (inner_finished) {
                handle().resume();
                return handle().done();
            }
            return false;
        } else {
            this->_sent = arg;
            handle().resume();
            return handle().done();
        }
    }
    typename types::yield::transit_reference do_get_yielded() noexcept override final {
        if constexpr (not types::yield::is_void) {
            return *_yielded;
        } else {
            return 0;
        }
    }

    base_type* _this_or_inner = this;

public:
    std::suspend_never  initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }

    handle_type handle() noexcept { return handle_type::from_promise(*this); }

    void unhandled_exception() const { throw; }

    channel_type get_return_object() noexcept { return channel_type(handle()); }

    struct yield_awaiter {
        channel_promise& self;

        constexpr bool                            await_ready() const noexcept { return false; }
        constexpr typename types::send::reference await_resume() const noexcept {
            if constexpr (types::send::is_void) {
                // Nothing to do/return
            } else {
                return static_cast<types::send::reference>(*self._sent);
            }
        }

        constexpr void await_suspend(auto) const noexcept {}
    };

    yield_awaiter yield_value(typename types::yield::nonvoid y) noexcept {
        _yielded = y;
        return yield_awaiter{*this};
    }

    template <typename Y>
        requires rvalue_reference_type<yielded> and constructible_from<remove_cvref_t<yielded>, Y&&>
    yield_awaiter yield_value(Y&& y) noexcept {
        _yielded = y;
        return yield_awaiter{*this};
    }

    template <typename Promise>
    struct nested_awaiter {
        channel_promise& self;
        Promise&         other;

        constexpr bool await_ready() const noexcept { return other.has_returned(); }
        constexpr void await_suspend(auto) const noexcept { self._this_or_inner = &other; }

        constexpr decltype(auto) await_resume() const noexcept {
            self._this_or_inner = &self;
            return other.returned_value();
        }
    };

    template <typename Ch, typename Promise = remove_reference_t<Ch>::promise_type>
        requires convertible_to<typename Ch::yielded, typename types::yield::reference>
    auto yield_value(from_channel<Ch> ch) noexcept {
        Promise& pr = ch._arg._coro.promise();
        return nested_awaiter<Promise>{*this, pr};
    }

    typename types::yield::transit_reference get_yielded() const noexcept {
        return _this_or_inner->do_get_yielded();
    }

    void send_value(typename types::send::transit_reference s) { do_send_value(s); }
};

}  // namespace detail

}  // namespace neo
