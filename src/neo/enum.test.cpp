#include "./enum.hpp"

#include <catch2/catch.hpp>

namespace test {

enum class mine : int {
    a = 0b001,
    b = 0b010,
    c = 0b100,
};

NEO_DECL_ENUM_BITOPS(mine);

}  // namespace test

constexpr auto val_a = test::mine::a;
constexpr auto val_b = test::mine::b;
constexpr auto val_c = test::mine::c;

constexpr auto val_ab = val_a | val_b;
static_assert(val_ab != val_a);
static_assert(val_ab != val_b);
static_assert(neo::test_flags(val_ab, val_a));
static_assert(neo::test_flags(val_ab, val_b));
static_assert(!neo::test_flags(val_ab, val_c));

static_assert(neo::is_bitset_enum_v<test::mine>);
static_assert(!neo::is_bitset_enum_v<test::mine&>);
static_assert(neo::bitset_enum<test::mine>);
static_assert(neo::bitset_enum<test::mine&>);  // Remove-cvref
static_assert(!neo::bitset_enum<int>);

TEST_CASE("Bit ops") {
    auto val = val_ab;
    CHECK(val == (test::mine::a | test::mine::b));
    val |= test::mine::c;
    CHECK(neo::test_flags(val, test::mine::c));

    val = test::mine::a | test::mine::c;
    val = neo::toggle_flags(val, test::mine::b);
    CHECK(val == (test::mine::a | test::mine::b | test::mine::c));
    val = neo::toggle_flags(val, test::mine::b);
    CHECK(val == (test::mine::a | test::mine::c));
}
