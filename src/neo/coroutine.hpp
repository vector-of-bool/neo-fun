#pragma once

#include "./config-pp.hpp"

#include <coroutine>

#if defined __GNUC__ && (!defined(__clang__)) && __GNUC__ < 13
// See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99576
#define NEO_GCC_Broken_Coroutines Enabled
#else
#define NEO_GCC_Broken_Coroutines Disabled
#endif

#if defined __clang__ && __clang_major__ < 17
// See: https://github.com/llvm/llvm-project/issues/61900
#define NEO_Clang_Broken_Coroutines Enabled
#else
#define NEO_Clang_Broken_Coroutines Disabled
#endif
