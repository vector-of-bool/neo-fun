#pragma once

#include <neo/assert.hpp>
#include <neo/pp.hpp>

namespace neo {

#define NEO_CORO_BEGIN(LineVar)                                                                    \
    auto& _neo_coro_lineno_storage_ = (LineVar);                                                   \
    switch (_neo_coro_lineno_storage_) {                                                           \
    default:                                                                                       \
        neo_assert_always(invariant,                                                               \
                          !NEO_CORO_IS_FINISHED(_neo_coro_lineno_storage_),                        \
                          "neo-switch-coroutine was reentered after completion");                  \
        neo_assert_always(invariant,                                                               \
                          false,                                                                   \
                          "neo-switch-coroutine encountered an invalid state",                     \
                          _neo_coro_lineno_storage_);                                              \
    case 0:

#define NEO_CORO_YIELD(...)                                                                        \
    NEO_FN_MACRO_BEGIN                                                                             \
    _neo_coro_lineno_storage_ = __LINE__;                                                          \
    return __VA_ARGS__;                                                                            \
    case __LINE__:                                                                                 \
        static_assert(true);                                                                       \
        NEO_FN_MACRO_END

#define NEO_CORO_IS_FINISHED(Var) (Var == 999999)

#define NEO_CORO_END                                                                               \
    }                                                                                              \
    _neo_coro_lineno_storage_ = 999999;                                                            \
    static_assert(true)

}  // namespace neo
