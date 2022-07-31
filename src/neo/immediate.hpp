#pragma once

#include "./addressof.hpp"
#include "./assert.hpp"
#include "./fwd.hpp"
#include "./optional.hpp"

#include <coroutine>
#include <type_traits>
#include <utility>

namespace neo {

/**
 * @brief An awaitable type that is immediately ready, will never block, and will not suspend.
 *
 * @tparam T The result type of `co_await`
 */
template <typename T>
class immediate;

/**
 * @brief Create an `immediate<T>` that is immediately ready with a value
 *
 * @tparam T The result type of the immediate
 * @param value The value
 * @return immediate<std::remove_cvref_t<T>>
 */
template <typename T>
immediate<std::remove_cvref_t<T>> make_immediate(T&& value) noexcept {
    co_return NEO_FWD(value);
}

inline immediate<void> make_immediate() noexcept;

namespace imm_detail {

template <typename T>
struct immediate_promise;

template <typename T>
struct defer_convert {
    immediate_promise<T>& self;

    constexpr operator immediate<T>() const noexcept;
};

struct immediate_promise_base {
    auto initial_suspend() const noexcept { return std::suspend_never{}; }
    auto final_suspend() const noexcept { return std::suspend_always{}; }
    auto unhandled_exception() { throw; }
};

template <typename T>
struct immediate_promise : immediate_promise_base {
    nano_opt_storage<T> _value;

    auto get_return_object() noexcept { return defer_convert<T>{*this}; }

    template <std::convertible_to<T> U>
    void return_value(U&& v) noexcept(noexcept(T(NEO_FWD(v)))) {
        new (neo::addressof(_value.value)) T(NEO_FWD(v));
    }

    ~immediate_promise() { _value.value.~T(); }

    T& value() noexcept { return _value.value; }
};

template <>
struct immediate_promise<void> : immediate_promise_base {
    auto get_return_object() noexcept { return defer_convert<void>{*this}; }

    void return_void() noexcept {}
};

}  // namespace imm_detail

/**
 * @brief A coroutine return type that never suspends and runs to completion immediately.
 *
 * @tparam T The result type of the coroutine (the co_return type)
 */
template <typename T>
class immediate {
public:
    using type = T;

    using promise_type = imm_detail::immediate_promise<T>;
    using co_type      = std::coroutine_handle<promise_type>;

    template <typename Ref>
    struct awaiter {
        co_type _co;

        constexpr bool await_ready() noexcept { return true; }

        void           await_suspend(std::coroutine_handle<>) { neo::unreachable(); }
        decltype(auto) await_resume() const noexcept {
            if constexpr (std::is_void_v<T>) {
                // No data to return
            } else {
                return static_cast<Ref&&>(_co.promise().value());
            }
        }
    };

private:
    constexpr static bool is_void = std::is_void_v<T>;
    using NonVoid                 = std::conditional_t<is_void, decltype(nullptr), T>;
    co_type _co;

    NonVoid& _value() const noexcept requires(not is_void) { return _co.promise().value(); }

public:
    explicit immediate(co_type co) noexcept
        : _co(co) {}

    immediate(immediate&& o) noexcept
        : _co(o._co) {
        o._co = nullptr;
    }

    immediate& operator=(immediate&& o) noexcept {
        if (_co) {
            _co.destroy();
        }
        _co   = o._co;
        o._co = nullptr;
        return *this;
    }

    immediate(NonVoid&& init) noexcept
        : immediate(make_immediate(NEO_FWD(init))) {}

    immediate() noexcept requires is_void : immediate(make_immediate()) {}

    ~immediate() {
        if (_co) {
            _co.destroy();
        }
    }

    NonVoid&       operator*() & noexcept requires(not is_void) { return _value(); }
    const NonVoid& operator*() const& noexcept requires(not is_void) { return _value(); }
    NonVoid        operator*() && noexcept requires(not is_void) { return std::move(_value()); }
    const NonVoid& operator*() const&& noexcept requires(not is_void) {
        return std::move(_value());
    }

    auto operator->() noexcept requires(not is_void) { return neo::addressof(_value()); }
    auto operator->() const noexcept requires(not is_void) { return neo::addressof(_value()); }

    auto operator co_await() & noexcept { return awaiter<NonVoid&>{_co}; }
    auto operator co_await() const& noexcept { return awaiter<const NonVoid&>{_co}; }
    auto operator co_await() && noexcept { return awaiter<NonVoid>{_co}; }
    auto operator co_await() const&& noexcept { return awaiter<const NonVoid>{_co}; }
};

template <typename T>
explicit immediate(const T&) -> immediate<T>;

explicit immediate()->immediate<void>;

template <typename T>
constexpr imm_detail::defer_convert<T>::operator immediate<T>() const noexcept {
    return immediate<T>(immediate<T>::co_type::from_promise(this->self));
}

}  // namespace neo

inline neo::immediate<void> neo::make_immediate() noexcept { co_return; }
