#pragma once

#include <neo/channel_fwd.hpp>

#include <neo/attrib.hpp>
#include <neo/opt_ref.hpp>
#include <neo/optional.hpp>
#include <neo/type_traits.hpp>

#include <coroutine>
#include <exception>

namespace neo {

template <typename Yield, typename Send, typename Return>
class channel {
public:
    using promise_type = _channel_detail::promise<Yield, Send, Return>;
    using send_type    = Send;
    using yield_type   = Yield;
    using return_type  = Return;

    using pipe_type = channel_pipe<Yield, Send, Return>;

private:
    template <typename, typename, typename>
    friend class _channel_detail::promise;

    template <typename>
    friend class from_channel;

    using handle_type = std::coroutine_handle<promise_type>;

public:
    pipe_type open() {
        // Initiate the channel:
        _coro.resume();
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

    constexpr from_channel<channel> operator~() noexcept;

    class iterator {
        handle_type _coro;

        friend channel;

        explicit iterator(handle_type h) noexcept
            : _coro(h) {}

        struct sender {
            handle_type co;

            void operator=(typename promise_type::send_put send) const&& {
                co.promise().send_value(NEO_FWD(send));
            }
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

private:
    handle_type _coro;

    constexpr explicit channel(handle_type h) noexcept
        : _coro(h) {}
};

template <typename Yield, typename Send, typename Return>
class channel_pipe {
    using promise     = _channel_detail::promise<Yield, Send, Return>;
    using handle_type = std::coroutine_handle<promise>;
    handle_type _coro;

    friend class channel<Yield, Send, Return>;

    explicit channel_pipe(handle_type co)
        : _coro(co) {}

public:
    using send_type   = Send;
    using yield_type  = Yield;
    using return_type = Return;
    using send_put    = promise::send_put;

    /**
     * @brief Returns `true` if the channel has co_returned
     */
    constexpr bool done() const noexcept { return _coro.promise().has_returned(); }

    /**
     * @brief Obtain the most-recently yielded value from the channel.
     *
     * This function will always return an lvalue-reference. Use take_current()
     * to obtain the value from the coroutine casted to the true yield type.
     */
    constexpr add_lvalue_reference_t<yield_type> current() const noexcept {
        return _coro.promise().get_yielded();
    }

    /**
     * @brief Take the current object yielded by the channel.
     *
     * @return _types::yield_backward
     */
    constexpr add_rvalue_reference_t<yield_type> take_current() noexcept {
        return static_cast<add_rvalue_reference_t<yield_type>>(_coro.promise().get_yielded());
    }

    /**
     * @brief Send a value. This is the main overload
     *
     * The type of the parameter depends on the template parameter for sending.
     * If Send is void, this function is not available.
     *
     * @param arg The value to send
     */
    constexpr void send(send_put arg)
        requires(not neo_is_void(send_type))
    {
        _coro.promise().send_value(NEO_FWD(arg));
    }

    /**
     * @brief Send a value with a possible conversion.
     *
     * @note This overload is only visible if Send IS NOT an lvalue-reference type.
     *
     * If Send is an lvalue reference, then Send&& will be an lvalue reference, and the requires
     * constraint on this function will fail.
     *
     * This will always accept a reference-to-const of the underlying type, which
     * results in allowing conversions to the yielded type
     *
     * @param y
     * @return yield_awaiter<remove_cvref_t<yield_forward>>
     */
    constexpr void send(const_reference_t<send_put> arg)
        requires(not void_type<send_type>)
        // Require that we are not sending lvalue-references:
        and rvalue_reference_type<add_rvalue_reference_t<send_type>>
        // Require that we can construct a send value from the const-reference:
        and convertible_to<const_reference_t<send_type>, remove_cvref_t<send_type>>
    {
        remove_cvref_t<send_put> x = NEO_FWD(arg);
        _coro.promise().send_value(NEO_FWD(x));
    }

    /**
     * @brief Send a void value (resumes the coroutine channel)
     *
     * This overload is only visible if Send is `void`
     */
    constexpr void send()
        requires void_type<send_type>
    {
        _coro.promise().send_value(nullptr);
    }

    /**
     * @brief Obtain the final return value from the coroutine.
     *
     * If the Return type is void, this function returns void. Otherwise,
     * this function will always return an lvalue-reference.
     */
    constexpr add_lvalue_reference_t<return_type> return_value() const noexcept {
        return _coro.promise().get_returned();
    }

    /**
     * @brief Take the final return value from the coroutine.
     *
     * This returns exactly yield_type, which may be an rvalue
     */
    constexpr return_type take_return_value() noexcept {
        return _coro.promise().take_return_value();
    }
};

template <typename C>
class from_channel {
    // The wrapped channel coroutine
    typename remove_cvref_t<C>::handle_type _coro;

    template <typename, typename>
    friend struct _channel_detail::promise_base;

public:
    explicit from_channel(const C& c) noexcept
        : _coro(c._coro) {}
};

template <typename C>
explicit from_channel(C const&) -> from_channel<C>;

template <typename Y, typename S, typename R>
constexpr from_channel<channel<Y, S, R>> channel<Y, S, R>::operator~() noexcept {
    return from_channel(*this);
}

namespace _channel_detail {

template <typename ReturnVal>
struct promise_return {
    using return_get  = ReturnVal&;
    using return_take = ReturnVal;
    neo::nano_opt<ReturnVal> _return_value;
    bool                     _did_throw = false;
    template <neo::convertible_to<ReturnVal> U>
    constexpr void return_value(U&& arg) noexcept(nothrow_constructible_from<ReturnVal, U>) {
        _return_value.emplace(NEO_MOVE(arg));
    }
    constexpr bool has_returned() const noexcept { return _return_value.has_value() or _did_throw; }
    constexpr ReturnVal& get_returned() noexcept { return _return_value.get(); }
    constexpr ReturnVal  take_return_value() noexcept { return NEO_MOVE(_return_value.get()); }
};

template <reference_type ReturnRef>
struct promise_return<ReturnRef> {
    using return_get  = ReturnRef&;
    using return_take = ReturnRef&&;
    neo::opt_ref<remove_reference_t<ReturnRef>> _return_value;
    bool                                        _did_throw = false;
    template <neo::convertible_to<ReturnRef> U>
    constexpr void return_value(U&& arg) noexcept(nothrow_constructible_from<ReturnRef, U>) {
        _return_value = arg;
    }
    constexpr bool       has_returned() const noexcept { return !!_return_value or _did_throw; }
    constexpr return_get get_returned() const noexcept {
        return static_cast<return_get>(*_return_value);
    }
    constexpr return_take take_return_value() const noexcept {
        return static_cast<return_take>(*_return_value);
    }
};

template <>
struct promise_return<void> {
    bool           _did_return = false;
    bool           _did_throw  = false;
    constexpr void return_void() noexcept { _did_return = true; }
    constexpr bool has_returned() const noexcept { return _did_return or _did_throw; }
    constexpr void get_returned() noexcept {}
    constexpr void take_return_value() noexcept {}
    using return_get  = void;
    using return_take = void;
};

template <typename T>
struct channel_box {
    using put_type  = T&&;
    using get_type  = T&;
    using take_type = T;

    remove_reference_t<T>* _ptr = nullptr;

    constexpr void      put(put_type p) { _ptr = NEO_ADDRESSOF(p); }
    constexpr get_type  get() const noexcept { return static_cast<get_type>(*_ptr); }
    constexpr take_type take() const noexcept { return static_cast<take_type>(*_ptr); }
};

template <>
struct channel_box<void> {
    using put_type  = decltype(nullptr);
    using get_type  = void;
    using take_type = void;

    constexpr void put(put_type) {}
    constexpr void get() const noexcept {}
    constexpr void take() const noexcept {}
};

template <>
struct channel_box<const void> : channel_box<void> {};

template <typename Yield, typename Send>
struct promise_base {
    using yield_box = channel_box<Yield>;
    using send_box  = channel_box<Send>;
    using yield_put = yield_box::put_type;
    using yield_get = yield_box::get_type;
    using send_put  = send_box::put_type;

    virtual std::coroutine_handle<> do_get_handle() noexcept = 0;

    NEO_NO_UNIQUE_ADDRESS yield_box _yield_box;
    NEO_NO_UNIQUE_ADDRESS send_box  _send_box;

    // In the beginning, we believe that we are the leaf and the root channel in the chain:
    promise_base* _leaf = this;
    promise_base* _root = this;

    void connect_leaf(promise_base* leaf) {
        _leaf        = leaf;
        _leaf->_root = this;
    }

    // Keep track of what we are doing here
    struct parent_resume_info {
        std::exception_ptr      thrown;
        std::coroutine_handle<> resume;
    };

    parent_resume_info* _parent = nullptr;

    // Awaiter for inner channels:
    template <typename Promise, typename Ret>
    struct nested_awaiter {
        promise_base& self;
        // The inner channel that we are waiting for
        Promise&           child;
        parent_resume_info info{};

        constexpr bool await_ready() const noexcept { return child.has_returned(); }
        constexpr auto await_suspend(std::coroutine_handle<> this_co) noexcept {
            self._root->connect_leaf(&child);
            child._parent = &info;
            info.resume   = this_co;
            return std::coroutine_handle<Promise>::from_promise(child);
        }

        constexpr Ret await_resume() const {
            self._root->connect_leaf(&self);
            if (info.thrown) {
                std::rethrow_exception(info.thrown);
            }
            return child.take_return_value();
        }
    };

    template <typename Ch, typename Promise = remove_reference_t<Ch>::promise_type>
        requires derived_from<Promise, promise_base>
    constexpr auto yield_value(from_channel<Ch> ch) {
        Promise& pr = ch._coro.promise();
        return nested_awaiter<Promise, typename remove_reference_t<Ch>::return_type>{*this, pr};
    }

    struct parent_resumer {
        promise_base& self;

        constexpr bool                    await_ready() const noexcept { return false; }
        constexpr std::coroutine_handle<> await_suspend(auto) const noexcept {
            if (self._parent) {
                return self._parent->resume;
            }
            return std::noop_coroutine();
        }
        void await_resume() const noexcept { std::terminate(); }
    };

    constexpr parent_resumer final_suspend() noexcept { return {*this}; }

    std::suspend_always initial_suspend() const noexcept { return {}; }

    /**
     * @brief Sends a value and resumes the innermost channel
     *
     * @return true If the channel returned
     * @return false Otherwise
     */
    constexpr void send_value(send_put arg) {
        // We're the leaf channel
        _leaf->_send_box.put(NEO_FWD(arg));
        _leaf->do_get_handle().resume();
    }

    /**
     * @brief Obtain the yielded value
     */
    constexpr yield_get get_yielded() const noexcept { return _leaf->_yield_box.get(); }

    // Awaiter for regular yield expressions
    template <typename Store>
    struct yield_awaiter {
        promise_base& self;
        Store         _store;

        constexpr bool                         await_ready() const noexcept { return false; }
        constexpr typename send_box::take_type await_resume() const noexcept {
            // Return the value that was sent to the coroutine upon resume:
            return self._send_box.take();
        }

        constexpr void await_suspend(auto) noexcept { self._yield_box.put(NEO_FWD(_store)); }
    };

    /**
     * @brief Yield a value. This is the main overload
     *
     * The type of the parameter depends on the template parameter for yielding.
     * If Yield is void, yield_forward is nullptr_t, otherwise it is Yield&&
     *
     * @param y The value to yield
     * @return yield_awaiter<yield_lref>
     */
    constexpr yield_awaiter<yield_put> yield_value(yield_put y) noexcept {
        return {*this, NEO_FWD(y)};
    }

    /**
     * @brief Yield with a possible conversion.
     *
     * @note This overload is only visible if Yield IS NOT an lvalue-reference type.
     *
     * If Yield is an lvalue reference, then Yield&& will be an lvalue reference, and the requires
     * constraint on this function will fail.
     *
     * This will always accept a reference-to-const of the underlying type, which
     * results in allowing conversions to the yielded type
     *
     * @param y
     * @return yield_awaiter<remove_cvref_t<yield_forward>>
     */
    constexpr yield_awaiter<remove_cvref_t<yield_put>>
    yield_value(const_reference_t<yield_put> y) noexcept
        // This overload is only visible for non-lref types:
        requires rvalue_reference_type<yield_put>
        // We must be able to convert the forwarding reference to the underlying type:
        and convertible_to<const_reference_t<yield_put>, remove_cvref_t<yield_put>>
    {
        return {*this, NEO_FWD(y)};
    }
};

template <typename Yield, typename Send, typename Return>
class promise : public promise_base<Yield, Send>,  //
                public promise_return<Return> {
    /// The channel associated with this promise
    using channel_type = channel<Yield, Send, Return>;

    std::coroutine_handle<> do_get_handle() noexcept override {
        return std::coroutine_handle<promise>::from_promise(*this);
    }

public:
    void unhandled_exception() {
        if (this->_parent) {
            this->_parent->thrown = std::current_exception();
            return;
        }
        this->_did_throw = true;
        throw;
    }

    constexpr channel_type get_return_object() noexcept {
        return channel_type{std::coroutine_handle<promise>::from_promise(*this)};
    }

    constexpr bool did_throw() const noexcept { return this->_did_throw; }
};

}  // namespace _channel_detail

}  // namespace neo
