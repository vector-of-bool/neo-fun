#pragma once

#include <neo/fwd.hpp>

#include <functional>

namespace neo {

/**
 * Represents an 'output' parameter. An output parameter of type `T` should be declared with type
 * output<T>. The callee can assign directly into the output using the `put` method, or can
 * bind/obtain a reference via `get()`.
 */
template <typename T>
struct output {
    std::reference_wrapper<T> _dest;

public:
    constexpr explicit output(T& out)
        : _dest(out) {}

    template <typename Value>
    constexpr void put(Value&& value) {
        _dest.get() = NEO_FWD(value);
    }

    constexpr T& get() noexcept { return _dest; }

    constexpr operator T&() const noexcept { return _dest; }
};

/**
 * An `optional_output` is an output parameter than may be left null by a caller.
 * It otherwise behaves similar to an `output<T>`, but the `put` method is a no-op
 * if the `optional_output` is not engaged.
 */
template <typename T>
class optional_output {
    opt_ref<T> _dest;

public:
    constexpr optional_output() = default;
    constexpr optional_output(nullopt_t) {}

    constexpr explicit optional_output(T& out)
        : _dest(out) {}

    template <typename Value>
    constexpr void put(Value&& value) {
        if (_dest) {
            *_dest = NEO_FWD(value);
        }
    }

    constexpr T& operator*() const noexcept { return *_dest; }
    constexpr T* operator->() const noexcept { return _dest.operator->(); }

    constexpr explicit operator bool() const noexcept { return bool(_dest); }
};

template <typename T>
output<T> into(T& dest) {
    return output<T>(dest);
}

}  // namespace neo