#pragma once

/**
 * @brief Concatenate two preprocessor tokens using token-pasting after macro replacement
 */
#define NEO_CONCAT(a, ...) P_NEO_CONCAT_IMPL(a, __VA_ARGS__)
#define P_NEO_CONCAT_IMPL(a, ...) a##__VA_ARGS__

/// Concatenate three preprocessor tokens after macro replacement
#define NEO_CONCAT_3(a, b, c) NEO_CONCAT(a, NEO_CONCAT(b, c))
/// Concatenate four preprocessor tokens after macro replacement
#define NEO_CONCAT_4(a, b, c, d) NEO_CONCAT(a, NEO_CONCAT(b, NEO_CONCAT(c, d)))

#define P_NEO_STR_1(...) #__VA_ARGS__
/// Convert the given preprocessor tokens to a string after macro replacement
#define NEO_STR(...) P_NEO_STR_1(__VA_ARGS__)

/// Expand to the first argument given, ignoring any subsequent arguments
#define NEO_FIRST(Arg, ...) Arg

/**
 * @brief Expands to `1` if the given parameter list expands to an empty token sequence,
 * otherwise `0`
 */
#define NEO_IS_EMPTY(...) NEO_FIRST(__VA_OPT__(0, ) 1)

/**
 * @brief An object-like macro that expands to no tokens
 */
#define NEO_EMPTY

/**
 * @brief A function-like macro that ignores its arguments and expands to no tokens
 */
#define NEO_IGNORE(...)

#define P_NEO_NOT_0 1
#define P_NEO_NOT_1 0
/**
 * @brief Expand to `1` if given `0`. Expand to `0` if given `1`.
 *
 * Any other arguments will result in a nonsense token
 */
#define NEO_NOT(B) NEO_CONCAT(P_NEO_NOT_, B)

/**
 * @brief Function-like macro that expands to its given arguments.
 */
#define NEO_JUST(...) __VA_ARGS__

#define P_NEO_BOOL_EVAL_FALSEY_0
#define P_NEO_BOOL_EVAL_FALSEY_false

#define P_NEO_OR_11 1
#define P_NEO_OR_10 1
#define P_NEO_OR_01 1
#define P_NEO_OR_00 0
#define NEO_OR(CondA, CondB) NEO_CONCAT_3(P_NEO_OR_, CondA, CondB)

#define P_NEO_AND_00 0
#define P_NEO_AND_01 0
#define P_NEO_AND_10 0
#define P_NEO_AND_11 1
#define NEO_AND(CondA, CondB) NEO_CONCAT3(P_NEO_AND_, CondA, CondB)

/**
 * @brief Expand to `0` if given `0`, `false`, or an empty token sequence.
 * Expands to `1` for any other arguments.
 */
#define NEO_BOOL(...)                                                                              \
    NEO_NOT(NEO_OR(NEO_IS_EMPTY(NEO_CONCAT(P_NEO_BOOL_EVAL_FALSEY_, __VA_ARGS__)),                 \
                   NEO_IS_EMPTY(__VA_ARGS__)))

/**
 * @brief Create a conditional branch.
 *
 * @param Cond A condition, passed to @ref NEO_BOOL. If expands to `1`.
 */
// clang-format off
#define NEO_IF_ELSE(Cond, OnTrue, OnFalse)                                                         \
    NEO_IGNORE(#OnTrue, #OnFalse) \
    NEO_CONCAT(P_NEO_IF_, NEO_BOOL(Cond)) \
    NEO_EMPTY (OnTrue, OnFalse)
// clang-format on

#define P_NEO_IF_1(OnTrue, OnFalse) OnTrue NEO_IGNORE(#OnFalse)
#define P_NEO_IF_0(OnTrue, OnFalse) OnFalse NEO_IGNORE(#OnTrue)

#define P_NEO_EVAL_1024(...) P_NEO_EVAL_512(P_NEO_EVAL_512(__VA_ARGS__))
#define P_NEO_EVAL_512(...) P_NEO_EVAL_256(P_NEO_EVAL_256(__VA_ARGS__))
#define P_NEO_EVAL_256(...) P_NEO_EVAL_128(P_NEO_EVAL_128(__VA_ARGS__))
#define P_NEO_EVAL_128(...) P_NEO_EVAL_64(P_NEO_EVAL_64(__VA_ARGS__))
#define P_NEO_EVAL_64(...) P_NEO_EVAL_32(P_NEO_EVAL_32(__VA_ARGS__))
#define P_NEO_EVAL_32(...) P_NEO_EVAL_16(P_NEO_EVAL_16(__VA_ARGS__))
#define P_NEO_EVAL_16(...) P_NEO_EVAL_8(P_NEO_EVAL_8(__VA_ARGS__))
#define P_NEO_EVAL_8(...) P_NEO_EVAL_4(P_NEO_EVAL_4(__VA_ARGS__))
#define P_NEO_EVAL_4(...) P_NEO_EVAL_2(P_NEO_EVAL_2(__VA_ARGS__))
#define P_NEO_EVAL_2(...) P_NEO_EVAL_1(P_NEO_EVAL_1(__VA_ARGS__))
#define P_NEO_EVAL_1(...) __VA_ARGS__

#define P_NEO_MAP_DONE(Func, Constant, Counter, Elem)

// clang-format off
#define P_NEO_MAP_A(Func, Constant, Counter, Element, ...) \
    Func(Constant, Counter, Element) \
    NEO_IF_ELSE(NEO_IS_EMPTY(__VA_ARGS__), P_NEO_MAP_DONE, P_NEO_MAP_B) \
    NEO_EMPTY (Func, Constant, Counter + 1, __VA_ARGS__)
#define P_NEO_MAP_B(Func, Constant, Counter, Element, ...) \
    Func(Constant, Counter, Element) \
    NEO_IF_ELSE(NEO_IS_EMPTY(__VA_ARGS__), P_NEO_MAP_DONE, P_NEO_MAP_A) \
    NEO_EMPTY (Func, Constant, Counter + 1, __VA_ARGS__)
// clang-format on

#define NEO_MAP(Func, Constant, ...)                                                               \
    __VA_OPT__(P_NEO_EVAL_1024(P_NEO_MAP_A(Func, Constant, 0, __VA_ARGS__)))

#define NEO_FN_MACRO_BEGIN if ((bool)1) {
#define NEO_FN_MACRO_END                                                                           \
    }                                                                                              \
    else((void)0)
