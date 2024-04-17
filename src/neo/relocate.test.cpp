#include "./relocate.hpp"
#include "neo/constexpr_union.hpp"

#include <memory>
#include <string_view>

#include <catch2/catch.hpp>
#include <vector>

static_assert(neo::trivially_relocatable<int>);
static_assert(neo::trivially_relocatable<const int>);
static_assert(neo::trivially_relocatable<std::string_view>);
static_assert(neo::trivially_relocatable<int&>);
static_assert(neo::trivially_relocatable<const std::string_view&>);
static_assert(neo::trivially_relocatable<std::unique_ptr<int>>);
static_assert(neo::trivially_relocatable<std::allocator<int>>);
static_assert(neo::trivially_relocatable<std::vector<int>>);

TEST_CASE("Relocate some objects") {
    int  a = 31;
    auto b = neo::relocate(&a);
    CHECK(b == 31);
}

TEST_CASE("Relocate a large object") {
    {
        neo::constexpr_union<std::vector<int>> onion;
        onion.construct<0>() = {1, 2};
        auto b               = neo::relocate(&onion._0);
        CHECK(b.size() == 2);
    }
    {
        neo::constexpr_union<std::array<int, 32>> onion;
        onion.construct<0>() = {1, 2, 3};
        auto dup             = neo::relocate(&onion._0);
        CHECK(dup[0] == 1);
        CHECK(dup[1] == 2);
        CHECK(dup[2] == 3);
    }
}
