#pragma once

// clang-format off
#if defined(__has_builtin)
    #if __has_builtin(__builtin_addressof)
        #define NEO_HAS_BUILTIN 1
    #else
        #define NEO_HAS_BUILTIN 0
    #endif
#elif _MSC_VER
    #define NEO_HAS_BUILTIN 1
#else
    #define NEO_HAS_BUILTIN 0
#endif
// clang-format on

#if !NEO_HAS_BUILTIN
#include <memory>
#endif

namespace neo {

/**
 * @brief Obtain a pointer to the given argument, even if that object overloads the address-of
 * operator
 */
template <typename T>
constexpr T* addressof(T& arg) noexcept {
#if NEO_HAS_BUILTIN
    return __builtin_addressof(arg);
#else
    return std::addressof(arg);
#endif
}

template <typename T>
const T& addressof(const T&&) = delete;

}  // namespace neo

#undef NEO_HAS_BUILTIN
