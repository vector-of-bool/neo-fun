#pragma once

#include "./addressof.hpp"
#include "./concepts.hpp"
#include "./iterator_concepts.hpp"

#include <coroutine>
#include <stdexcept>

namespace neo {

/**
 * @brief A coroutine type that yields values sequentially from an arbitrary computation.
 *
 * @tparam YieldType The type of objects that will be generated
 */
template <typename T>
class [[nodiscard]] generator {
public:
    using yield_type = T;

    class promise_type {
    public:
        using reference_type = yield_type&;
        using pointer_type   = add_pointer_t<reference_type>;
        using value_type     = remove_cvref_t<yield_type>;

    private:
        std::exception_ptr    _exc     = nullptr;
        pointer_type          _current = nullptr;
        constexpr static bool is_ref   = neo_is_reference(yield_type);

        using lref  = value_type&;
        using rref  = value_type&&;
        using clref = value_type const&;

    public:
        generator<yield_type> get_return_object() noexcept;

        constexpr auto initial_suspend() const noexcept { return std::suspend_always{}; }
        constexpr auto final_suspend() const noexcept { return std::suspend_always{}; }

        constexpr auto yield_value(lref v) noexcept
            requires constructible_from<T, lref>
        {
            _current = NEO_ADDRESSOF(v);
            return std::suspend_always{};
        }

        constexpr auto yield_value(clref v) noexcept
            requires constructible_from<T, clref>
        {
            _current = NEO_ADDRESSOF(v);
            return std::suspend_always{};
        }

        constexpr auto yield_value(rref v) noexcept
            requires constructible_from<T, rref>
        {
            _current = NEO_ADDRESSOF(v);
            return std::suspend_always{};
        }

        constexpr void unhandled_exception() noexcept { _exc = std::current_exception(); }

        constexpr void return_void() noexcept {}

        // Disable 'co_await' inside of a generator (We aren't an async generator)
        template <typename Other>
        [[deprecated("You cannot use 'co_await' inside a generator")]]  //
        std::suspend_never
        await_transform(Other&&)
            = delete;

        constexpr reference_type get_value() const noexcept { return *_current; }

        void throw_if_exc() const {
            if (_exc) {
                std::rethrow_exception(_exc);
            }
        }
    };

    struct iterator {
        using handle_type = std::coroutine_handle<promise_type>;

    private:
        handle_type _coro = nullptr;

    public:
        constexpr iterator() = default;

        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = remove_cvref_t<yield_type>;
        using reference         = T&&;
        using pointer           = typename promise_type::pointer_type;

    private:
        friend neo::generator<T>;
        constexpr explicit iterator(handle_type h) noexcept
            : _coro(h) {}

    public:
        struct sentinel_type {};

        [[nodiscard]] constexpr bool operator==(sentinel_type) const noexcept {
            return not _coro or _coro.done();
        }

        [[nodiscard]] constexpr reference operator*() const noexcept {
            return static_cast<reference>(_coro.promise().get_value());
        }
        constexpr iterator& operator++() {
            _coro.resume();
            if (_coro.done()) {
                _coro.promise().throw_if_exc();
            }
            return *this;
        }

        constexpr void operator++(int) { ++(*this); }
    };

private:
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type _coro = nullptr;

    friend promise_type;
    constexpr explicit generator(handle_type h) noexcept
        : _coro(h) {}

public:
    constexpr generator() noexcept = default;

    constexpr generator(generator&& other) noexcept
        : _coro(other._coro) {
        other._coro = nullptr;
    }

    ~generator() {
        if (_coro) {
            _coro.destroy();
        }
    }

    constexpr generator& operator=(generator&& other) noexcept {
        if (_coro) {
            _coro.destroy();
        }
        _coro       = other._coro;
        other._coro = nullptr;
        return *this;
    }

    /**
     * @brief Start the generator and prepare the first value.
     *
     * @return A new iterator.
     */
    [[nodiscard]] constexpr input_iterator auto begin() {
        if (_coro) {
            _coro.resume();
            if (_coro.done()) {
                _coro.promise().throw_if_exc();
            }
        }
        return iterator{_coro};
    }

    /// Obtain a sentinel indicating completion of the generator
    [[nodiscard]] constexpr auto end() const noexcept { return typename iterator::sentinel_type{}; }
};

template <typename T>
generator<T> generator<T>::promise_type::get_return_object() noexcept {
    return generator<T>{std::coroutine_handle<generator<T>::promise_type>::from_promise(*this)};
}

}  // namespace neo
