#pragma once

#include <neo/platform.hpp>

// clang-format off
#if NEO_COMPILER_IS_MSVC
#  define NEO_ALWAYS_INLINE __forceinline
#  define NEO_PRETTY_FUNC __FUNCSIG__

#elif NEO_COMPILER_IS_GNU_LIKE
#  define NEO_ALWAYS_INLINE [[gnu::always_inline]] inline
#  define NEO_PRETTY_FUNC __PRETTY_FUNCTION__

#else
#  define NEO_ALWAYS_INLINE                                                                        \
    [[deprecated("NEO_ALWAYS_INLINE is not defined for this compiler. Please submit an issue.")]] inline
#  define NEO_PRETTY_FUNC __func__

#endif

#if __cpp_constexpr_dynamic_alloc
# define NEO_CONSTEXPR_DESTRUCTOR constexpr
#else
# define NEO_CONSTEXPR_DESTRUCTOR inline
#endif

// clang-format on
