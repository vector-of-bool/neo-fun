#pragma once

#include "./config-pp.hpp"

#include <coroutine>

#if defined __GNUC__ && (!defined(__clang__)) && __GNUC__ < 13
// See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99576
#define NEO_GCC_Broken_Coroutines Enabled
#else
#define NEO_GCC_Broken_Coroutines Disabled
#endif
