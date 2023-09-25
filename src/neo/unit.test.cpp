#include "./unit.hpp"
#include "./concepts.hpp"

#include <catch2/catch.hpp>

static_assert(neo::regular<neo::unit>);
static_assert(neo::totally_ordered<neo::unit>);
static_assert(neo::unit{} == neo::unit{});

TEST_CASE("Default construct") {
    neo::unit a, b;
    CHECK(a == b);
    CHECK_FALSE(a != b);
}

TEST_CASE("Copy") {
    neo::unit a;
    auto      b = a;
    CHECK(a == b);
}

TEST_CASE("Order") {
    neo::unit a, b;
    CHECK(a <= b);
    CHECK_FALSE(a < b);
}
