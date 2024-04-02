#pragma once

#include "./attrib.hpp"

#if !NEO_HAS_BUILTIN(__builtin_addressof)
#include <memory>
#endif

namespace neo {

/**
 * @brief Obtain a pointer to the given argument, even if that object overloads the address-of
 * operator
 */
template <typename T>
constexpr T* addressof(T& arg) noexcept {
#if NEO_HAS_BUILTIN(__builtin_addressof)
    return __builtin_addressof(arg);
#else
    return std::addressof(arg);
#endif
}

template <typename T>
const T& addressof(const T&&) = delete;

#if NEO_HAS_BUILTIN(__builtin_addressof)
#define NEO_ADDRESSOF(X) __builtin_addressof((X))
#else
#define NEO_ADDRESSOF(X) std::addressof((X))
#endif

}  // namespace neo
