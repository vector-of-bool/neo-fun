#pragma once

#include "./attrib.hpp"

// clang-format off
#if (NEO_HAS_BUILTIN(__builtin_addressof) || defined(_MSC_VER))
#define HAS_ADDROF_INTRIN 1
#endif
// clang-format on

#ifndef HAS_ADDROF_INTRIN
#include <memory>
#endif

namespace neo {

/**
 * @brief Obtain a pointer to the given argument, even if that object overloads the address-of
 * operator
 */
template <typename T>
constexpr T* addressof(T& arg) noexcept {
#if HAS_ADDROF_INTRIN
    return __builtin_addressof(arg);
#else
    return std::addressof(arg);
#endif
}

template <typename T>
const T& addressof(const T&&) = delete;

#if HAS_ADDROF_INTRIN
#define NEO_ADDRESSOF(X) __builtin_addressof((X))
#else
#define NEO_ADDRESSOF(X) std::addressof((X))
#endif

}  // namespace neo

#ifdef HAS_ADDROF_INTRIN
#undef HAS_ADDROF_INTRIN
#endif
