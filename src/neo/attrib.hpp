#pragma once

#include <neo/platform.hpp>

#if NEO_COMPILER_IS_MSVC
#define NEO_ALWAYS_INLINE __forceinline
#elif NEO_COMPILER_IS_GNU_LIKE
#define NEO_ALWAYS_INLINE [[gnu::always_inline]]
#else
#define NEO_ALWAYS_INLINE                                                                          \
    [[deprecated("NEO_ALWAYS_INLINE is not defined for this compiler. Please submit an issue.")]]
#endif
