#pragma once

#include "./platform.hpp"
#include "./pp.hpp"

// clang-format off
#ifdef __has_include
#   if __has_include(<version>)
#       include <version>
#   endif
#endif

// https://wg21.link/CWG2598
#define pNeoCompilerImplementsFeature_cwg2598() ! NEO_COMPILER(Clang) || (NEO_COMPILER(LLVMClang) && __clang_major__ >= 18)
// https://wg21.link/CWG2096
#define pNeoCompilerImplementsFeature_cwg2096() ! NEO_COMPILER(Clang) || (NEO_COMPILER(LLVMClang) && __clang_major__ >= 18)

#define pNeoCompilerImplements(ignore1, ignore2, Item) \
    && (1 == NEO_CONCAT(pNeoCompilerImplementsFeature_, Item)())

#define neo_compiler_implements(...) (1 neo_map(pNeoCompilerImplements, ~, __VA_ARGS__))
// clang-format on
