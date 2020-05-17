#pragma once

// clang-format off
#define NEO_RETURNS_L(...)          \
    noexcept(noexcept(__VA_ARGS__)) \
    -> decltype(__VA_ARGS__)        \
    { return __VA_ARGS__; }
// clang-format on

#define NEO_RETURNS(...) NEO_RETURNS_L(__VA_ARGS__) static_assert(true)
