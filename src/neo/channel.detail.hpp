#pragma once

#include "./channel_fwd.hpp"

#include <neo/addressof.hpp>
#include <neo/attrib.hpp>
#include <neo/concepts.hpp>
#include <neo/optional.hpp>
#include <neo/unit.hpp>

#include <coroutine>
#include <exception>

namespace neo::_channel_detail {

// Information used to coordinate between nested channel coroutines
struct stack_resume_info {
    std::exception_ptr      thrown;
    std::coroutine_handle<> co_handle;
};

// Used to extract the coroutine handle from a `from_channel` wrapper.
struct from_chan_coro_getter {
    template <typename C>
    static auto getit(from_channel<C>& from) noexcept {
        return from._coro;
    }
};

/**
 * @brief Abstract base class for send/yield handling
 *
 * @tparam T
 */
template <typename T>
class io_base {
public:
    /**
     * @brief Push the referred-to value to the handler.
     */
    virtual void put(add_lvalue_reference_t<nonvoid_t<T>>) = 0;
};

/**
 * @brief Implements the return value aspect of channel
 *
 * @tparam Derived The derived promise type (CRTP)
 */
template <typename Derived>
struct return_part;

/**
 * @brief Implements the yielding aspect channels. `T` must be `void` or an
 * lvalue-reference type. This type may be empty.
 *
 * Derives from io_base<T>. Defines a `get() -> nonvoid_t<T>&` method.
 */
template <typename T>
struct yield_io;

/**
 * @brief Implements the sending+resuming aspect for channels. `T` must be `void`
 * or an lvalue-reference type. This type must be constructed with a coroutine
 * handle.
 *
 * Derives from io_base<T>. Defines a `get() -> nonvoid_t<T>&` method.
 */
template <typename T>
struct send_io;

template <typename OuterSend, typename InnerSend, typename OuterYield, typename InnerYield>
concept nested_channel_yield_compatible = requires {
    // The type being send to the outer channel should be convertible to the type that the inner
    // channel expects to be sent
    requires convertible_to<OuterSend, InnerSend>;
    // The type yielded by the sub-channel must be convertible to the parent channel's yield type
    requires convertible_to<InnerYield, OuterYield>;
    // Passing the yield/send values through should not construct any references to temporary
    // objects:
    requires not reference_constructible_from_temporary<InnerSend, OuterSend>;
    requires not reference_constructible_from_temporary<OuterYield, InnerYield>;
};

/**
 * @brief Promise type for channels
 */
template <typename Yield, typename Send, typename Return>
class promise : public return_part<promise<Yield, Send, Return>> {
    // The coroutine handle type for this promise
    using handle_type = std::coroutine_handle<promise>;

    // Grant other promises access to our components, to allow updating the yield/send IO
    template <typename, typename, typename>
    friend class _channel_detail::promise;

    // The yield handler for this promise.
    NEO_NO_UNIQUE_ADDRESS yield_io<add_lvalue_reference_t<Yield>> _self_yielder;
    // The send handler for this promise. We pass the handle to this coroutine so that
    // the send_io can resume this coroutine when the caller sends on the channel.
    send_io<add_lvalue_reference_t<Send>> _self_sender{handle_type::from_promise(*this)};

    /**
     * @brief Pointer to the active send-handler for the channel.
     *
     * In the non-recursive case, this pointer always points to our own send-handler.
     * If the coroutine yields-from another channel<>, this pointer will be temporarily
     * updated to point to the send-handler for that sub-channel.
     *
     * See `nested_awaiter` for where this pointer is manipulated.
     */
    io_base<add_lvalue_reference_t<Send>>* _sender = &_self_sender;
    /**
     * @brief Pointer to the yield-handler for the channel.
     *
     * Like the send-handler, this usually points to our own yield handler, but it
     * may be updated by a parent coroutine in `nested_awaiter`.
     */
    io_base<add_lvalue_reference_t<Yield>>* _yielder = &_self_yielder;

    /**
     * @brief Pointer-to-pointer-to the root sender for the current channel
     * stack. This pointer will be updated by calls to the nested_yielded, and
     * is used to update the `_sender` pointer on the root channel when a sub-channel
     * begins execution.
     */
    io_base<add_lvalue_reference_t<Send>>** _root_sender_ptr = &_sender;

    /**
     * @brief Information about the parent channel coroutine, if applicable.
     *
     * This pointer is only assigned by a parent coroutine in `nested_awaitable`.
     */
    stack_resume_info* _parent = nullptr;

public:
    // Conversion helper for the channel
    struct init {
        // Coroutine handle for the channel
        handle_type co;
    };

    constexpr init get_return_object() noexcept { return {handle_type::from_promise(*this)}; }

    // Always start the channel coroutine as suspended
    std::suspend_always initial_suspend() const noexcept { return {}; }

    // Push a value into the channel, also resuming it
    void send_value(nonvoid_t<Send>& arg) { this->_sender->put(arg); }

    /**
     * @brief Obtain a reference to the yielded value. If `void`, then returns
     * a `unit&`
     */
    nonvoid_t<Yield>& get_yielded() noexcept {
        return static_cast<nonvoid_t<Yield>&>(_self_yielder.get());
    }

    void unhandled_exception() {
        if (this->_parent) {
            // We are a sub-channel, and we want the parent coroutine to resume
            // with the exception that we just caught.
            this->_parent->thrown = std::current_exception();
            // Don't rethrow, just stop:
            return;
        }
        // No parent coroutine is running, so re-throw into the resumer
        throw;
    }

    // An awaitable used at the end of a channel to optionally resume a parent
    // channel, if present
    struct parent_resumer {
        promise& self;

        // We always suspend ourself
        constexpr bool await_ready() const noexcept { return false; }
        // Symmetric transfer to another coroutine:
        constexpr std::coroutine_handle<> await_suspend(auto) const noexcept {
            if (self._parent) {
                // We are executing as part of a sub-channel. We want to resume
                // the coroutine that started us
                return self._parent->co_handle;
            }
            // We are not executing as a child, so run the noop coroutine, which
            // will just immediately return to the resumer
            return std::noop_coroutine();
        }
        // This is never called, as we are done executing
        void await_resume() const noexcept { std::terminate(); }
    };

    parent_resumer final_suspend() noexcept { return {*this}; }

    /**
     * @brief Awaiter type generated by `co_yield` expressions. Unconditionally
     * suspends the coroutine and stores a pointer to the yielded value in a place
     * that is accessible to the caller
     *
     * @tparam YieldTmp The temporary object type used to store the yielded value.
     * May be a reference.
     */
    template <typename YieldTmp>
    struct yield_awaiter {
        promise& self;
        YieldTmp yielded_value;

        constexpr bool await_ready() const noexcept { return false; }

        /// Upon resumption, return the sent value that caused the resume:
        Send await_resume() const noexcept {
            return static_cast<add_rvalue_reference_t<Send>>(self._self_sender.get());
        }

        // Upon suspend, store a pointer to the yielded value:
        constexpr void await_suspend(auto) noexcept { self._yielder->put(yielded_value); }
    };

    /**
     * @brief Yield a value. Primary overload.
     *
     * @param y A reference to the value being yielded.
     *
     * This function only records a reference to the yielded value, and does not
     * copy/move the value. This relies on the compiler extending the lifetime of
     * the yield operand to the full yield expression, allowing the reference to
     * outlive the entire coroutine suspension that is caused by that `co_yield`.
     *
     * If the Yield type is a non-reference type, then this function
     * accepts an rvalue-reference to the yield type, which may incur an automatic
     * conversion at the yield site. If the Yield type is a reference type, then
     * this function will accept exactly that reference type as an operand.
     */
    yield_awaiter<nonvoid_t<Yield>&> yield_value(nonvoid_t<Yield>&& y) noexcept {
        return {*this, y};
    }

    template <typename Arg>
        requires reference_constructible_from_temporary<Yield, Arg>
    [[deprecated(
        "Your channel's yield-type is a reference type, but the operand of your `co_yield` "
        "expression would bind a the yielded reference to a temporary. This is safe, but probably"
        "a mistake.")]] void
    yield_value(Arg&& arg)
        = delete;

    /**
     * @brief Yield with a possible conversion.
     *
     * @note This overload is only visible if Yield IS NOT a reference type. This
     * makes two overloads of `yield_value` available: `yield_value(T&&)` and `yield_value(const
     * T&)`, which allows the yielding of `T` values that will either move or copy as-needed.
     *
     * If Yield is an lvalue reference, then Yield&& will be an lvalue reference, and the requires
     * constraint on this function will fail.
     *
     * This will always accept a reference-to-const of the underlying type, which
     * results in allowing conversions to the yielded type
     */
    constexpr yield_awaiter<nonvoid_t<Yield>> yield_value(nonvoid_t<Yield> const& y) noexcept
        // This overload is only visible for non-lref types:
        requires(not reference_type<Yield>)
        // We must be able to copy to the underlying type
        and convertible_to<nonvoid_t<Yield> const&, Yield>
    {
        // In this case, the yield_awaiter takes the yielded object by-value (using a copy), because
        // we cannot reference-extend the object.
        return {*this, y};
    }

    /**
     * @brief Awaiter generated by `co_yield from_channel(...)`.
     *
     * This connects sub-channels to this channel and resumes them immediately.
     *
     * @tparam OtherCo The coroutine handle type used by the other channel. Note
     * that the channel might not be equivalent to our own!
     */
    template <typename OtherCo>
    struct nested_awaiter;

    template <typename OtherYield, typename OtherSend, typename OtherReturn>
    struct nested_awaiter<std::coroutine_handle<promise<OtherYield, OtherSend, OtherReturn>>> {
        using OtherHandle = std::coroutine_handle<promise<OtherYield, OtherSend, OtherReturn>>;
        promise& self;
        // coroutine handle for the sub-channel
        OtherHandle child_co_handle;
        // Resumption info that is used to coordinate with the sub-channel
        stack_resume_info resume_info{};

        // We are only done if the sub-channel is already done
        constexpr bool await_ready() const noexcept { return child_co_handle.done(); }

        struct sender_link : io_base<add_lvalue_reference_t<Send>> {
            io_base<add_lvalue_reference_t<OtherSend>>* _send_child;

            void put(add_lvalue_reference_t<nonvoid_t<Send>> s) { _send_child->put(s); }
        };

        struct yielder_link : io_base<add_lvalue_reference_t<OtherYield>> {
            io_base<add_lvalue_reference_t<Yield>>* _parent_yield;

            void put(nonvoid_t<OtherYield>& s) { _parent_yield->put(s); }
        };

        sender_link  _sender2{};
        yielder_link _yielder2{};

        io_base<add_lvalue_reference_t<OtherSend>>*
        get_sender_link(io_base<add_lvalue_reference_t<OtherSend>>* child_sender) {
            if constexpr (weak_same_as<OtherSend, Send>) {
                return child_sender;
            } else {
                _sender2._send_child = child_sender;
                return &_sender2;
            }
        }

        io_base<add_lvalue_reference_t<OtherYield>>*
        get_yielder_link(io_base<add_lvalue_reference_t<Yield>>* parent_yielder) {
            if constexpr (weak_same_as<OtherYield, Yield>) {
                return parent_yielder;
            } else {
                _yielder2._parent_yield = parent_yielder;
                return &_yielder2;
            }
        }

        // Suspension semantics for yielding-from sub-channels:
        constexpr auto await_suspend(handle_type this_co) noexcept {
            // The promise of the sub-channel:
            auto& child_pr = child_co_handle.promise();
            // Connect the root send-handler for our own channel stack to the send-handler
            // for the sub-channel. This ensures that when the resumer calls `send()` on this
            // channel (or a parent thereof), the sent value will be immediately directed to the new
            // leaf channel, and that leaf channel will be resumed instead of the suspended parent
            // channels.
            *self._root_sender_ptr = this->get_sender_link(child_pr._sender);
            // Tell the sub-channel where the root sender-handler-pointer lives, in case it needs to
            // spawn any sub-channels of its own:
            child_pr._root_sender_ptr = self._root_sender_ptr;
            // Tell the sub-channel ot use our the current yielder as its yielder. This may point to
            // a yielder of a parent channel as well.
            child_pr._yielder = this->get_yielder_link(self._yielder);
            // Give the sub-channel a way to coordinate with this channel.
            child_pr._parent = &resume_info;
            // Tell the sub-channel our own coroutine handle so that it can resume us when it
            // returns.
            resume_info.co_handle = this_co;
            // Symmetric transfer: Begin executing the subchannel
            return child_co_handle;
        }

        // Sub-channel returns: Now we return the return value from that channel from
        // the `co_yield` expression itself.
        constexpr OtherReturn await_resume() const {
            // Re-connect the root sender to this channel now that it has become the leaf channel
            // again.
            *self._root_sender_ptr = &self._self_sender;
            if (resume_info.thrown) {
                // The subchannel threw an exception during execution, so we should rethrow it too
                std::rethrow_exception(resume_info.thrown);
            }
            // Get the return value
            return static_cast<add_rvalue_reference_t<OtherReturn>>(
                child_co_handle.promise().get_returned());
        }
    };

    template <typename OtherYield, typename OtherSend, typename OtherReturn>
        requires nested_channel_yield_compatible<Send, OtherSend, Yield, OtherYield>
    auto yield_value(from_channel<channel<OtherYield, OtherSend, OtherReturn>> from) {
        auto other_co = from_chan_coro_getter::getit(from);
        return nested_awaiter<decltype(other_co)>{*this, other_co};
    }
};

template <typename Yield, typename Send>
struct promise_base;

template <typename Yield, typename Send, typename Return>
struct return_part<promise<Yield, Send, Return>> {
    using Derived = promise<Yield, Send, Return>;

    optional<Return> _retval;

    constexpr Return& get_returned() noexcept { return *_retval; }

    template <implicit_convertible_to<Return> Arg>
    constexpr void return_value(Arg&& arg) {
        _retval.emplace(NEO_FWD(arg));
    }
};

template <typename Yield, typename Send, void_type Return>
struct return_part<promise<Yield, Send, Return>> {
    constexpr void return_void() noexcept {}
    constexpr unit get_returned() noexcept { return {}; }
};

template <lvalue_reference_type Ref>
struct send_io<Ref> : io_base<Ref> {
    explicit send_io(std::coroutine_handle<> co) noexcept
        : resumer(co) {}
    add_pointer_t<Ref>      ptr;
    std::coroutine_handle<> resumer;

    void put(Ref ref) {
        ptr = NEO_ADDRESSOF(ref);
        resumer.resume();
    }
    Ref get() const noexcept { return *ptr; }
};

template <void_type Void>
struct send_io<Void> : io_base<void> {
    explicit send_io(std::coroutine_handle<> co) noexcept
        : resumer(co) {}

    std::coroutine_handle<> resumer;
    static inline unit      nil;

    void  put(unit&) override { resumer.resume(); }
    unit& get() const noexcept { return nil; }
};

template <lvalue_reference_type T>
struct yield_io<T> : io_base<T> {
    add_pointer_t<T> ptr;
    void             put(T ref) override { ptr = NEO_ADDRESSOF(ref); }

    T get() const noexcept { return *ptr; }
};

template <void_type T>
struct yield_io<T> : io_base<void> {
    void put(unit&) override {}

    static inline unit nil;
    unit&              get() const noexcept { return nil; }
};

}  // namespace neo::_channel_detail
