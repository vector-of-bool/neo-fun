#include "./tuple.hpp"
#include "neo/concepts.hpp"
#include "neo/get.hpp"
#include <neo/testing.hpp>

#include <catch2/catch.hpp>

#include <memory>
#include <string>
#include <tuple>

using neo::tuple;
using neo::tuple_get_t;

neo::testing::cx_test_case EmptyTuple = [](auto check) consteval {
    neo::tuple<> empty;
    check(empty == empty);
    check(empty <= empty);
    auto dup                   = empty;            // Copy-construct
    empty                      = dup;              // Copy-assign
    auto dup2 [[maybe_unused]] = NEO_MOVE(empty);  // Move-construct
    dup                        = NEO_MOVE(empty);  // Move-assign
    static_assert(neo::constructible_from<tuple<>>);
    static_assert(neo::constructible_from<tuple<>, tuple<>&>);
    static_assert(neo::constructible_from<tuple<>, tuple<> const&>);
    static_assert(neo::constructible_from<tuple<>, tuple<>&&>);
    static_assert(neo::constructible_from<tuple<>, tuple<> const&&>);
    static_assert(not neo::constructible_from<tuple<>, tuple<int> const&&>);
    // static_assert(neo::constructible_from<tuple<>, std::allocator_arg_t, std::allocator<void>>);
};

neo::testing::cx_test_case BasicTuple = [](auto check) consteval {
    tuple<int> a{21};
    check(a.get<0>() == 21);
    check(a == a);
    check(a == tuple{21});
    auto b = a;
    check(b == a);
    b.get<0>() = 412;
    check(b != a);
    check(a < b);
};

neo::testing::cx_test_case TupleOrdering = [](auto check) consteval {
    tuple<int, int> a{21, 41};
    tuple<int, int> b{2, 79};
    check(b < a);  // 21 < 2
    b.get<0>() = 22;
    check(b > a);  // 22 > 21
    b.get<0>() = 21;
    check(b > a);  // 79 > 41
};

neo::testing::cx_test_case TupleOfVoid = [](auto check) consteval {
    tuple<void>             v1;
    tuple<void, void>       v2;
    tuple<void, void, void> v3 [[maybe_unused]];
    auto                    dup = v2;
    (void)dup;
    dup = v2;
    check(v1 == v1);
    check(v2 == v2);

    tuple<neo::unit> u1;
    check(v1 == u1);
    u1 = v1;  // okay

    (void)tuple<void>(0);  // okay
    static_assert(std::regular<tuple<void>>);
    static_assert(std::regular<tuple<void, void>>);
};

neo::testing::cx_test_case TupleOfReferences = [](auto check) consteval {
    int         a = 0, b = 1, c = 2;
    tuple<int&> t1{a};
    tuple<int&> t2{b};
    tuple<int&> t3{c};
    check(t1 == t1);
    check(t2.get<0>() == 1);
    check(t1 != t2);
    a = 1;
    // Both no refer to equal values, but the references are not equal:
    check(t1.get<0>() == 1);
    check(t2.get<0>() == 1);
    check(t1 != t2);

    // Compare against a tuple that holds a value
    tuple<int> regular{1};
    // Both values are 1
    check(t1.get<0>() == 1);
    check(regular.get<0>() == 1);
};

static_assert(neo::explicit_convertible_to<tuple<int>, tuple<double>>);
static_assert(neo::convertible_to<tuple<int>, tuple<double>>);

template <typename... Ts>
static void semiregular_assertions() {
    using tuple = tuple<Ts...>;
    static_assert(std::semiregular<tuple>);
}

template <typename... Ts>
static void regular_assertions() {
    using tuple = tuple<Ts...>;
    semiregular_assertions<Ts...>();

    static_assert(std::regular<tuple>);
    static_assert(std::totally_ordered<tuple>);
}

TEST_CASE("Empty tuple") {
    tuple<> empty;
    auto    dup = empty;
    empty       = dup;
    regular_assertions<>();
}

TEST_CASE("Tuple with reference") {
    int         i       = 42;
    tuple<int&> t       = {i};
    int         i2      = 51;
    tuple<int&> another = {i2};
    auto        first   = neo::get_nth<0>(another);
    CHECK(first == i2);
    CHECK(another.get<0>() == 51);
    t = another;
    CHECK(i == 42);
}

struct not_equality_comparable {};

TEST_CASE("Equality and compare") {
    STATIC_REQUIRE(not neo::equality_comparable<tuple<not_equality_comparable>>);
    STATIC_REQUIRE(neo::equality_comparable<tuple<int>>);
    STATIC_REQUIRE(neo::equality_comparable<tuple<int, int>>);
    STATIC_REQUIRE(neo::equality_comparable<tuple<std::string, int>>);
    STATIC_REQUIRE(
        neo::equality_comparable_with<tuple<std::string_view, int>, tuple<const char*, int>>);
    STATIC_REQUIRE(
        neo::totally_ordered_with<tuple<std::string_view, int>, tuple<const char*, int>>);

    STATIC_REQUIRE(not neo::equality_comparable_with<tuple<std::string_view, int>,
                                                     tuple<const char*, not_equality_comparable>>);

    neo::tuple t1{1, 2};
    neo::tuple t2{2, 1};
    CHECK(t1 != t2);
    CHECK(t1 < t2);
    CHECK(t2 > t1);

    {
        neo::tuple<std::string_view, int> t1{"hey", 2};
        neo::tuple<const char*, long>     t2{"aaa", 5};
        CHECK(t1 > t2);
    }
}

TEST_CASE("Const-ness") {
    STATIC_REQUIRE(std::assignable_from<tuple_get_t<tuple<int>&, 0>, int>);
    STATIC_REQUIRE(not std::assignable_from<tuple_get_t<const tuple<int>&, 0>, int>);
    using moveonly = std::unique_ptr<int>;
    STATIC_REQUIRE(std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly>&&, 0>>);
    STATIC_REQUIRE(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly>&, 0>>);
    STATIC_REQUIRE(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly> const&, 0>>);
    STATIC_REQUIRE(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly> const&&, 0>>);
}

TEST_CASE("Simple usage") {
    int        a = 1;
    neo::tuple i{a};
}

TEST_CASE("Get<> on a std::tuple") {
    std::tuple<int, std::string>      t;
    std::same_as<int&> decltype(auto) n [[maybe_unused]] = neo::get_nth<0>(t);

    neo::tuple<int, std::string>                    t2;
    std::same_as<std::string&> decltype(auto)       s [[maybe_unused]] = neo::get_nth<1>(t);
    std::same_as<const std::string&> decltype(auto) s2 [[maybe_unused]]
    = neo::get_nth<1>(std::as_const(t2));
}

static_assert(
    neo::weak_same_as<neo::common_type_t<neo::tuple<double, int>, neo::tuple<int, double>>,
                      neo::tuple<double, double>>);

static_assert(not neo::has_common_type<neo::tuple<double, int>, neo::tuple<int, std::string>>);
static_assert(neo::has_common_type<neo::tuple<double, const char*>, neo::tuple<int, std::string>>);

static_assert(neo::weak_same_as<
              neo::common_reference_t<neo::tuple<int&, double>, neo::tuple<int const&, double>>,
              neo::tuple<int const&, double>>);
