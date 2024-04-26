#pragma once

#define NEO_CONCAT(a, ...) NEO_CONCAT_IMPL(a, __VA_ARGS__)
#define NEO_CONCAT_IMPL(a, ...) a##__VA_ARGS__

#define NEO_CONCAT_3(a, b, c) NEO_CONCAT(a, NEO_CONCAT(b, c))
#define NEO_CONCAT_4(a, b, c, d) NEO_CONCAT(a, NEO_CONCAT(b, NEO_CONCAT(c, d)))

#define NEO_STR(...) NEO_STR_1(__VA_ARGS__)
#define NEO_STR_1(...) #__VA_ARGS__

// Expands to its arguments immediately
#define NEO_JUST(...) __VA_ARGS__

// Function-like macro that ignores its arguments and expands to nothing
#define NEO_DISCARD(...)

// Pass a macro name, which will be prevented from expanding until another pass
#define NEO_DEFER(...) __VA_ARGS__ NEO_DISCARD()

// Force the preprocessor to perform N extra macro expansions on the arguments:
#define NEO_EVAL_1024(...) NEO_EVAL_512(NEO_EVAL_512(__VA_ARGS__))
#define NEO_EVAL_512(...) NEO_EVAL_256(NEO_EVAL_256(__VA_ARGS__))
#define NEO_EVAL_256(...) NEO_EVAL_128(NEO_EVAL_128(__VA_ARGS__))
#define NEO_EVAL_128(...) NEO_EVAL_64(NEO_EVAL_64(__VA_ARGS__))
#define NEO_EVAL_64(...) NEO_EVAL_32(NEO_EVAL_32(__VA_ARGS__))
#define NEO_EVAL_32(...) NEO_EVAL_16(NEO_EVAL_16(__VA_ARGS__))
#define NEO_EVAL_16(...) NEO_EVAL_8(NEO_EVAL_8(__VA_ARGS__))
#define NEO_EVAL_8(...) NEO_EVAL_4(NEO_EVAL_4(__VA_ARGS__))
#define NEO_EVAL_4(...) NEO_EVAL_2(NEO_EVAL_2(__VA_ARGS__))
#define NEO_EVAL_2(...) NEO_EVAL_1(NEO_EVAL_1(__VA_ARGS__))
#define NEO_EVAL_1(...) __VA_ARGS__


#define NEO_FIRST(A, ...) A
#define NEO_SECOND(A, B, ...) B

#define NEO_IS_PROBE(...) NEO_SECOND(__VA_ARGS__, 0)
#define NEO_PROBE() ~, 1

#define NEO_NOT(X) NEO_IS_PROBE(NEO_CONCAT(NEO_NOT_, X))
#define NEO_NOT_0 NEO_PROBE()

#define NEO_BOOL(X) NEO_NOT(NEO_NOT(X))

#define NEO_IF_ELSE_1(Cond) NEO_CONCAT(NEO_IF_, Cond)
#define NEO_IF_ELSE(Cond) NEO_IF_ELSE_1(NEO_BOOL(Cond))

#define NEO_IF_1(...) __VA_ARGS__ NEO_IF_1_ELSE
#define NEO_IF_0(...) NEO_IF_0_ELSE

#define NEO_IF_1_ELSE(...)
#define NEO_IF_0_ELSE(...) __VA_ARGS__

#define NEO_HAS_ARGS_END() 0
#define NEO_HAS_ARGS(...) NEO_BOOL(NEO_FIRST(NEO_HAS_ARGS_END __VA_ARGS__)())

#define NEO_IS_EMPTY(...) NEO_FIRST(__VA_OPT__(0, ) 1, ~)
#define NEO_NOT_EMPTY(...) NEO_FIRST(__VA_OPT__(1, ) 0, ~)

#define NEO_FN_MACRO_BEGIN if (true) {
#define NEO_FN_MACRO_END                                                                           \
    }                                                                                              \
    else(void(0))


#define NEO_LPAREN (
#define NEO_RPAREN )

#define neo_map \
    NEO_IF_ELSE(pNeoIsInMapExpansion())(pNeoMapDeferred)(pNeoMapImmediate)
// Expands the map macro immediately
#define pNeoMapImmediate(Action, Constant, ...) \
    NEO_EVAL_64(pNeoMapInit(Action, Constant, __VA_ARGS__))
// Inhibits the map macro to prevent attempts to expand itself
#define pNeoMapDeferred(Action, Constant, ...) \
    NEO_DEFER(pNeoMapInit)(Action, Constant, __VA_ARGS__)

// Expands to 1 if expanding during the evaluation of neo_map, otherwise zero
#define pNeoIsInMapExpansion() NEO_NOT_EMPTY(pTryGenerateEmptyMap())
// Expands to nothing iff we are not within an expansion of pNeoMapImmediate
#define pTryGenerateEmptyMap() pNeoMapImmediate(NEO_DISCARD, ~, nil)

// Initial loop step in applying the map
#define pNeoMapInit(Action, Constant, ...) __VA_OPT__(pNeoMapImpl(Action, Constant, 0, __VA_ARGS__))

// clang-format off
#define pNeoMapImpl(Action, Constant, Counter, Head, ...) \
    Action(Constant, (Counter), Head) \
    __VA_OPT__(NEO_DEFER(pNeoMapNext)()(Action, Constant, Counter + 1, __VA_ARGS__))
#define pNeoMapNext() pNeoMapImpl
// clang-format ons

// deprecated: Prefer `NEO_IF_ELSE()`
#define NEO_IIF(Cond, Then, Else)                                                                  \
    NEO_DISCARD(#Then, #Else)                                                                      \
    NEO_CONCAT(_neoIIF_, Cond)(Then, Else)

#define _neoIIF(Cond) NEO_CONCAT(_neoIIF_, Cond)

#define _neoIIF_0(Then, Else) Else NEO_DISCARD(#Then)
#define _neoIIF_1(Then, Else) Then NEO_DISCARD(#Else)


#define NEO_MAP_END(...)
#define NEO_MAP_GET_END(...) 0, NEO_MAP_END

#define NEO_MAP_NEXT_0(Element, Next, ...) Next NEO_DISCARD()
#define NEO_MAP_NEXT_1(Element, Next) NEO_MAP_NEXT_0(Element, Next, 0)
#define NEO_MAP_NEXT(Element, Next) NEO_MAP_NEXT_1(NEO_MAP_GET_END Element, Next)

#define NEO_MAP_A(Func, Constant, Counter, Element, NextElem, ...)                                 \
    Func(Constant, Counter, Element)                                                               \
        NEO_MAP_NEXT(NextElem, NEO_MAP_B)(Func, Constant, Counter + 1, NextElem, __VA_ARGS__)
#define NEO_MAP_B(Func, Constant, Counter, Element, NextElem, ...)                                 \
    Func(Constant, Counter, Element)                                                               \
        NEO_MAP_NEXT(NextElem, NEO_MAP_A)(Func, Constant, Counter + 1, NextElem, __VA_ARGS__)

#define NEO_MAP_1(Func, Constant, ...) NEO_EVAL_8(NEO_MAP_A(Func, Constant, 0, __VA_ARGS__, (), 0))
/// Deprecated: Prefer `neo_map()`
#define NEO_MAP(Func, Constant, ...)                                                               \
    NEO_IF_ELSE(NEO_HAS_ARGS(__VA_ARGS__))(NEO_MAP_1(Func, Constant, __VA_ARGS__))()
