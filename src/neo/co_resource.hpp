#pragma once

#if !__cpp_impl_coroutine
#error "<neo/co_resource.hpp> requires coroutine support"
#endif

#include "./co_resource_fwd.hpp"

#include "./addressof.hpp"

#include <coroutine>

namespace neo {

namespace cor_detail {

template <typename T>
struct promise;

/// Conversion helper for the resource, lets us properly intercept exceptions on startup
template <typename T>
struct init {
    std::coroutine_handle<promise<T>> coro;
};

/// Common base of the promise type
template <typename T>
struct promise_base {
    auto get_return_object() noexcept {
        return init<T>{
            std::coroutine_handle<promise<T>>::from_promise(static_cast<promise<T>&>(*this))};
    }
    auto initial_suspend() const noexcept { return std::suspend_never{}; }
    auto final_suspend() const noexcept { return std::suspend_always{}; }
    auto unhandled_exception() { throw; }
    auto return_void() const noexcept {}
    auto await_transform() = delete;
};

/// Specialization for non-void:
template <typename T>
struct promise : promise_base<T> {
    std::remove_reference_t<const T>* value_ptr;

    auto yield_value(std::remove_reference_t<const T>& value) noexcept {
        value_ptr = neo::addressof(value);
        return std::suspend_always{};
    }
};

/// Specialization for void:
template <>
struct promise<void> : promise_base<void> {
    auto yield_value(decltype(nullptr)) noexcept { return std::suspend_always{}; }
};

[[noreturn]] void assert_coro_yield_once(std::coroutine_handle<>) noexcept;
[[noreturn]] void assert_coro_did_yield(std::coroutine_handle<>, const void*) noexcept;

}  // namespace cor_detail

/**
 * @brief Implement a coroutine-based scope-base resource management.
 *
 * Use this object as the return type of a coroutine function that implements
 * resource management around a 'co_yield' expression.
 *
 * @tparam T The type of object that is accessible through this managed resource.
 *
 * @note If the coroutine has a co_resource<void> return type, the co_yield should
 * yield a literal zero '0'.
 */
template <typename T>
class [[nodiscard]] co_resource {
public:
    using promise_type = cor_detail::promise<T>;

private:
    std::coroutine_handle<promise_type> _coro{};

    void _finish() noexcept {
        if (!_coro) {
            return;
        }
        _coro.resume();
        if (!_coro.done()) {
            cor_detail::assert_coro_yield_once(_coro);
        }
        _coro.destroy();
        _coro = nullptr;
    }

public:
    co_resource(cor_detail::init<T> c) noexcept
        : _coro(c.coro) {}

    ~co_resource() { _finish(); }

    co_resource(co_resource&& o) noexcept
        : _coro{o._coro} {
        o._coro = nullptr;
    }

    co_resource& operator=(co_resource&& o) noexcept {
        _finish();
        _coro   = o._coro;
        o._coro = nullptr;
        return *this;
    }

    auto operator->() const noexcept { return neo::addressof(**this); }

    auto&& operator*() const noexcept {
        if (_coro.done() || !_coro.promise().value_ptr) {
            cor_detail::assert_coro_did_yield(_coro, _coro.promise().value_ptr);
        }
        return *_coro.promise().value_ptr;
    }
};

}  // namespace neo
