#include "./unit.hpp"
#include "./concepts.hpp"

#include <catch2/catch.hpp>

#include <concepts>

static_assert(neo::regular<neo::unit>);
static_assert(neo::totally_ordered<neo::unit>);
static_assert(neo::unit{} == neo::unit{});

TEST_CASE("make_nonvoid_t") {
    STATIC_REQUIRE(std::same_as<neo::nonvoid_t<int>, int>);
    STATIC_REQUIRE(std::same_as<neo::nonvoid_t<neo::unit>, neo::unit>);
    STATIC_REQUIRE(std::same_as<neo::nonvoid_t<void>, neo::unit>);
    STATIC_REQUIRE(std::same_as<neo::nonvoid_t<const void>, neo::unit>);
    STATIC_REQUIRE(std::same_as<neo::nonvoid_t<const void*>, const void*>);
}

TEST_CASE("Default construct") {
    constexpr neo::unit a, b;
    STATIC_REQUIRE(a == b);
    STATIC_REQUIRE_FALSE(a != b);
}

TEST_CASE("Copy") {
    constexpr neo::unit a;
    constexpr auto      b = a;
    STATIC_REQUIRE(a == b);
}

TEST_CASE("Order") {
    constexpr neo::unit a, b;
    STATIC_REQUIRE(a <= b);
    STATIC_REQUIRE_FALSE(a < b);
}

TEST_CASE("Construct from zero") {
    constexpr neo::unit a = 0;
    STATIC_REQUIRE(a == a);
}

TEST_CASE("Construct from arbitrary objects") {
    constexpr neo::unit a(42);
    STATIC_REQUIRE(a == a);

    constexpr neo::unit b(0);
    STATIC_REQUIRE(a == b);

    constexpr neo::unit c("hey");
    STATIC_REQUIRE(c == b);
}
