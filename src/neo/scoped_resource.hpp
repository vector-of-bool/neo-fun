#pragma once

#if !__cpp_impl_coroutine
#error "<neo/scoped_resource.hpp> requires coroutine support"
#endif

#include "./scoped_resource_fwd.hpp"

#include "./addressof.hpp"

#include <coroutine>
#include <stdexcept>

namespace neo {

namespace sr_detail {

template <typename T>
struct promise;

/// Conversion helper for the resource, lets it intercept exceptions on startup
template <typename T>
struct init {
    std::coroutine_handle<promise<T>> coro;
};

/// Common base of the promise type
template <typename T>
struct promise_base {
    std::exception_ptr exc{};

    auto get_return_object() noexcept {
        return init<T>{
            std::coroutine_handle<promise<T>>::from_promise(static_cast<promise<T>&>(*this))};
    }
    auto initial_suspend() const noexcept { return std::suspend_never{}; }
    auto final_suspend() const noexcept { return std::suspend_always{}; }
    void unhandled_exception() noexcept { exc = std::current_exception(); }
    void return_void() const noexcept {}
    void await_transform() = delete;
};

/// Specialization for non-void:
template <typename T>
struct promise : promise_base<T> {
private:
    std::remove_reference_t<const T>* _value_ptr;
    friend neo::scoped_resource<T>;

public:
    std::suspend_always yield_value(std::remove_reference_t<const T>& value) noexcept {
        _value_ptr = neo::addressof(value);
        return {};
    }
};

/// Specialization for void:
template <>
struct promise<void> : promise_base<void> {
    std::suspend_always yield_value(decltype(nullptr)) noexcept { return {}; }
};

void assert_coro_yield_once(std::coroutine_handle<>);
void assert_coro_did_yield(std::coroutine_handle<>, const void*);

}  // namespace sr_detail

/**
 * @brief Implement a coroutine-based scope-base resource management.
 *
 * Use this object as the return type of a coroutine function that implements
 * resource management around a 'co_yield' expression.
 *
 * @tparam T The type of object that is accessible through this managed resource.
 */
template <typename T>
class [[nodiscard]] scoped_resource {
public:
    using promise_type = sr_detail::promise<T>;

    using coroutine_handle_type = std::coroutine_handle<promise_type>;

private:
    friend promise_type;
    friend sr_detail::promise_base<T>;

    coroutine_handle_type _coro;

    void _finish() noexcept {
        if (!_coro) {
            return;
        }
        if (!_coro.promise().exc) {
            _coro.resume();
            if (!_coro.done()) {
                sr_detail::assert_coro_yield_once(_coro);
            }
            if (_coro.promise().exc) {
                // This will always terminate, which is the correct behavior during cleanup:
                std::rethrow_exception(_coro.promise().exc);
            }
        }
        _coro.destroy();
        _coro = nullptr;
    }

public:
    scoped_resource(sr_detail::init<T> init)
        : _coro{init.coro} {
        auto& pr = init.coro.promise();
        if (pr.exc) {
            std::rethrow_exception(pr.exc);
        }
    }

    ~scoped_resource() { _finish(); }

    scoped_resource(scoped_resource&& o) noexcept
        : _coro{o._coro} {
        o._coro = nullptr;
    }

    scoped_resource& operator=(scoped_resource&& o) noexcept {
        _finish();
        _coro   = o._coro;
        o._coro = nullptr;
        return *this;
    }

    auto operator->() const noexcept {
        return neo::addressof(**this);
        return _coro.promise()._value_ptr;
    }

    auto&& operator*() const noexcept {
        if (_coro.done() || !_coro.promise()._value_ptr) {
            sr_detail::assert_coro_did_yield(_coro, _coro.promise()._value_ptr);
        }
        return *_coro.promise()._value_ptr;
    }
};

}  // namespace neo
