#include "./mutref.hpp"

#include <concepts>

#include <catch2/catch.hpp>

static_assert(std::convertible_to<int&&, neo::mutref<int>>);

void foo(neo::mutref<int> i) { *i += 2; }

TEST_CASE("Create a mutref") {
    int val = 33;
    foo(val);
    CHECK(val == 35);
    foo(5);  // Allows rvalues
}
