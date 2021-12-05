#pragma once

#include "./addressof.hpp"

namespace neo {

/**
 * @brief A reference type that will bind to an rvalue OR an lvalue reference of
 * mutable T.
 *
 * This is intended as a parameter for functions that will accept either an lvalue
 * or an rvalue reference. Usage outside of a parameter list should be considered suspect.
 *
 * @tparam T The referred-to type
 */
template <typename T>
class mutref {
    T* _ref;

public:
    constexpr mutref(T& t) noexcept
        : _ref(neo::addressof(t)) {}
    constexpr mutref(T&& t) noexcept
        : _ref(neo::addressof(t)) {}

    constexpr    operator T&() const noexcept { return *_ref; }
    constexpr T& get() const noexcept { return *_ref; }
    constexpr T* operator->() const noexcept { return _ref; }
    constexpr T& operator*() const noexcept { return *_ref; }
};

}  // namespace neo
