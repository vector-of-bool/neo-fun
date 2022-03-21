#include <neo/pp.hpp>

#include <catch2/catch.hpp>

static_assert(NEO_FIRST(1, 2) == 1);
static_assert(NEO_FIRST(1) == 1);
static_assert(NEO_FIRST(0, 1) == 0);
static_assert(NEO_FIRST(662, 1) == 662);

static_assert(!NEO_IS_EMPTY(1, 2, 3));
static_assert(NEO_IS_EMPTY());
static_assert(!NEO_IS_EMPTY(()));
static_assert(NEO_IS_EMPTY(NEO_EMPTY));

TEST_CASE("Check macro evaluations") {
    std::string str = NEO_STR(Turn this into a string, please !);
    CHECK(str == "Turn this into a string, please !");
}

static_assert(NEO_IS_EMPTY());
static_assert(!NEO_IS_EMPTY(f));

#define DECL_INTEGER(Constant, Counter, X)                                                         \
    namespace {                                                                                    \
    constexpr int X = (Counter);                                                                   \
    }

NEO_MAP(DECL_INTEGER, ~, foo, bar, baz)

static_assert(foo == 0);
static_assert(bar == 1);
static_assert(baz == 2);

static_assert(NEO_BOOL(f));
static_assert(NEO_BOOL(1));
static_assert(!NEO_BOOL(0));
static_assert(!NEO_BOOL(NEO_EMPTY));
static_assert(!NEO_BOOL(false));
