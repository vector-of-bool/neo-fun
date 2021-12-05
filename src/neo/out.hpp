#pragma once

#include "./declval.hpp"
#include <neo/fwd.hpp>
#include <neo/opt_ref.hpp>

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

    constexpr T& get() const noexcept { return _dest; }

    constexpr operator T&() const noexcept { return _dest; }

    friend constexpr void do_repr(auto out, const output* self) noexcept requires requires {
        out.repr(NEO_DECLVAL(const T&));
    }
    {
        out.type("neo::output");
        if (self) {
            if constexpr (out.just_value) {
                out.value("->{}", out.repr_value(self->get()));
            } else {
                out.value("->{}", out.repr(self->get()));
            }
        }
    }
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

    template <typename U>
    constexpr optional_output(output<U> o)
        : _dest(o.get()) {}

    constexpr explicit optional_output(T& out)
        : _dest(out) {}

    template <typename Value>
    constexpr void put(Value&& value) {
        if (_dest) {
            *_dest = NEO_FWD(value);
        }
    }

    constexpr opt_ref<T> get() const noexcept { return _dest; }

    constexpr T& operator*() const noexcept { return *_dest; }
    constexpr T* operator->() const noexcept { return _dest.operator->(); }

    constexpr explicit operator bool() const noexcept { return bool(_dest); }

    friend constexpr void do_repr(auto                   out,
                                  const optional_output* self) noexcept requires requires {
        out.repr(NEO_DECLVAL(const T&));
    }
    {
        out.type("neo::optional_output<{}>", out.template repr_type<T>());
        if (self and *self) {
            out.value("->{}", out.repr_value(**self));
        } else {
            out.value("nullopt");
        }
    }
};

template <typename T>
output<T> into(T& dest) {
    return output<T>(dest);
}

}  // namespace neo