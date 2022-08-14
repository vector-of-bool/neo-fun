#pragma once

#include "./addressof.hpp"
#include "./attrib.hpp"
#include "./fwd.hpp"

#include <concepts>

namespace neo {

template <typename T>
class assignable_box {
    NEO_NO_UNIQUE_ADDRESS T _value;

public:
    assignable_box() = default;

    template <typename U>
    requires std::constructible_from<T, U&&>
    explicit constexpr assignable_box(U&& arg) noexcept(noexcept(T(NEO_FWD(arg))))
        : _value(NEO_FWD(arg)) {}

    [[nodiscard]] constexpr T&        operator*() & noexcept { return _value; }
    [[nodiscard]] constexpr const T&  operator*() const& noexcept { return _value; }
    [[nodiscard]] constexpr T&&       operator*() && noexcept { return NEO_MOVE(_value); }
    [[nodiscard]] constexpr const T&& operator*() const&& noexcept { return NEO_MOVE(_value); }

    constexpr T*       operator->() noexcept { return neo::addressof(_value); }
    constexpr const T* operator->() const noexcept { return neo::addressof(_value); }

    constexpr explicit operator T&() noexcept { return _value; }
    constexpr explicit operator const T&() const noexcept { return _value; }

    constexpr void friend do_repr(auto out, const assignable_box* self) noexcept requires requires {
        out.repr(**self);
    }
    {
        out.type("{}", out.template repr_type<T>());
        if (self) {
            out.value("{}", out.repr_value(**self));
        }
    }
};

template <typename T>
class assignable_box<T&> {
    T* _ptr;

public:
    assignable_box() = delete;

    template <typename U>
    explicit constexpr assignable_box(U&& arg) noexcept requires requires(void (*fn)(T*)) {
        fn(neo::addressof(arg));
    } : _ptr(neo::addressof(arg)) {
    }

    [[nodiscard]] constexpr T& operator*() const noexcept { return *_ptr; }

    constexpr T* operator->() const noexcept { return _ptr; }

    constexpr explicit operator T&() const noexcept { return *_ptr; }

    constexpr void friend do_repr(auto out, const assignable_box* self) noexcept requires requires {
        out.repr(**self);
    }
    {
        out.type("[reference to {}]", out.template repr_type<T>());
        if (self) {
            out.value("{}", out.repr_value(**self));
        }
    }
};

template <typename T>
explicit assignable_box(T&&) -> assignable_box<T>;

}  // namespace neo
