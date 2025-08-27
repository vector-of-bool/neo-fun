#include <neo/pp.hpp>

// Empty main for linking purposes
int main() {}

static_assert(NEO_IS_PROBE(nope) == 0);
static_assert(NEO_IS_PROBE(NEO_PROBE()) == 1);
static_assert(NEO_FIRST(1, 2) == 1);
static_assert(NEO_FIRST(1) == 1);
static_assert(NEO_FIRST(0, 1) == 0);
static_assert(NEO_FIRST(662, 1) == 662);

static_assert(NEO_HAS_ARGS(1, 2, 3));
static_assert(!NEO_HAS_ARGS());

#define DECL_INTEGER(Constant, Counter, X)                                                         \
    namespace {                                                                                    \
    constexpr int X = (Counter);                                                                   \
    }

NEO_MAP(DECL_INTEGER, ~, foo, bar, baz)

static_assert(foo == 0);
static_assert(bar == 1);
static_assert(baz == 2);

#define DECL_CONCAT(Const, Counter, X)                                                             \
    namespace {                                                                                    \
    constexpr int var_##Const##_##X [[maybe_unused]] = Counter;                                    \
    }
#define USES_MAP_INNER5(Const, Counter, X) neo_map(DECL_CONCAT, Const##_##X, foo, bar, baz)
#define USES_MAP_INNER4(Const, Counter, X) neo_map(USES_MAP_INNER5, Const##_##X, foo, bar, baz)
#define USES_MAP_INNER3(Const, Counter, X) neo_map(USES_MAP_INNER4, Const##_##X, foo, bar, baz)
#define USES_MAP_INNER2(Const, Counter, X) neo_map(USES_MAP_INNER3, Const##_##X, foo, bar, baz)
#define USES_MAP_INNER(Const, Counter, X) neo_map(USES_MAP_INNER2, X, foo, bar, baz)
neo_map(USES_MAP_INNER, ~, foo, bar, baz);

// Expands to nothing, since no arguments are given:
neo_map(not_defined, ~)
