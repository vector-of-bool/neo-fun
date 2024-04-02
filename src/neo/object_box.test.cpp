#include "./object_box.hpp"

#include "./archetypes.hpp"
#include "./repr.hpp"
#include "neo/concepts.hpp"

#include <catch2/catch.hpp>

#include <memory>

static_assert(std::constructible_from<neo::object_box<int>, int>);
static_assert(std::assignable_from<neo::object_box<int>&, neo::object_box<int>>);
static_assert(std::assignable_from<neo::object_box<int&>&, neo::object_box<int&>>);

struct nontrivial_destructor {
    ~nontrivial_destructor() {}
};
static_assert(neo::trivial_type<neo::object_box<int>>);
static_assert(neo::trivially_default_constructible<neo::object_box<int>>);
static_assert(neo::trivially_copyable<neo::object_box<int>>);
static_assert(neo::trivially_movable<neo::object_box<int>>);
static_assert(neo::trivially_copyable<neo::object_box<int&>>);
static_assert(neo::trivially_copyable<neo::object_box<const int&>>);
static_assert(neo::trivially_copyable<neo::object_box<void>>);
static_assert(neo::trivially_copyable<neo::object_box<int[4]>>);

static_assert(std::copyable<neo::object_box<int>>);
static_assert(std::semiregular<neo::object_box<int>>);
static_assert(std::copyable<neo::object_box<int&>>);
static_assert(std::copyable<neo::object_box<const int&>>);
static_assert(not std::copyable<neo::object_box<const int>>);
static_assert(std::movable<neo::object_box<std::unique_ptr<int>>>);
static_assert(std::movable<neo::object_box<std::unique_ptr<int>&>>);

static_assert(not neo::copy_constructible<neo::object_box<neo::arch::nontrivial_move_constructor>>);
static_assert(not neo::copy_assignable<neo::object_box<neo::arch::nontrivial_move_constructor>>);

static_assert(not std::default_initializable<neo::object_box<int&>>);
static_assert(std::default_initializable<neo::object_box<int>>);

TEST_CASE("Construct some boxes") {
    neo::object_box b1{7};
    static_assert(std::same_as<decltype(b1), neo::object_box<int>>);
    int             value = 9;
    neo::object_box b2{value};
    static_assert(std::same_as<decltype(b2), neo::object_box<int&>>);

    CAPTURE(neo::repr(b1));
    CAPTURE(neo::repr(b2));
}

TEST_CASE("Void box") {
    static_assert(neo::semiregular<neo::object_box<void>>);
    static_assert(neo::semiregular<neo::object_box<const void>>);
    static_assert(neo::semiregular<neo::object_box<volatile void>>);
    static_assert(neo::semiregular<neo::object_box<const volatile void>>);
}

TEST_CASE("Array box") {
    static_assert(neo::object_type<int[12]>);
    static_assert(neo::semiregular<neo::object_box<int[12]>>);
    int  carr[3] = {1, 2, 3};
    auto arr     = neo::object_box{std::as_const(carr)};
    CHECK(arr.get()[0] == 1);
    CHECK(arr.get()[1] == 2);
    CHECK(arr.get()[2] == 3);

    neo::object_box box{carr};
    static_assert(std::same_as<decltype(box), neo::object_box<int(&)[3]>>);
    CHECK(box.get()[0] == 1);
    CHECK(box.get()[1] == 2);
    CHECK(box.get()[2] == 3);

    neo::object_box box2{NEO_MOVE(carr)};
    static_assert(std::same_as<decltype(box2), neo::object_box<int[3]>>);
    CHECK(box2.get()[0] == 1);
    CHECK(box2.get()[1] == 2);
    CHECK(box2.get()[2] == 3);

    neo::object_box box3{std::as_const(arr)};
    static_assert(std::same_as<decltype(box3), neo::object_box<int const(&)[3]>>);
    CHECK(box3.get()[0] == 1);
    CHECK(box3.get()[1] == 2);
    CHECK(box3.get()[2] == 3);

    neo::object_box<int[3]> box_copy_const{std::as_const(carr)};
    static_assert(std::same_as<decltype(box_copy_const), neo::object_box<int[3]>>);
    CHECK(box_copy_const.get()[0] == 1);
    CHECK(box_copy_const.get()[1] == 2);
    CHECK(box_copy_const.get()[2] == 3);

    neo::object_box<int[3]> box_copy_rval{NEO_MOVE(carr)};
    static_assert(std::same_as<decltype(box_copy_rval), neo::object_box<int[3]>>);
    CHECK(box_copy_rval.get()[0] == 1);
    CHECK(box_copy_rval.get()[1] == 2);
    CHECK(box_copy_rval.get()[2] == 3);

    neo::object_box<int[3]> box_in_place_construct{std::in_place, carr};
    CHECK(box_in_place_construct.get()[0] == 1);
    CHECK(box_in_place_construct.get()[1] == 2);
    CHECK(box_in_place_construct.get()[2] == 3);
}

TEST_CASE("Emplacement construct") {
    neo::object_box<int> i{std::in_place, 21};
    CHECK(i.get() == 21);
}

TEST_CASE("Equality+Ordering") {
    SECTION("regular") {
        constexpr neo::object_box<int> a{31};
        CHECK(a == a);
        CHECK_FALSE(a < a);
    }
    SECTION("Arrays") {
        constexpr neo::object_box<int[3]> a{std::in_place, 1, 3, 4};
        STATIC_REQUIRE(a == a);
        STATIC_REQUIRE_FALSE(a < a);
        constexpr neo::object_box<int[3]> b{std::in_place, 3, 1, 9};
        STATIC_REQUIRE_FALSE(a == b);
        STATIC_REQUIRE(a < b);
    }
    SECTION("void") {
        constexpr neo::object_box<void> a;
        CHECK(a == a);
        CHECK_FALSE(a < a);
    }
}

static_assert(neo::regular<neo::object_box<int>>);
static_assert(neo::regular<neo::object_box<int[12]>>);
static_assert(neo::totally_ordered<neo::object_box<int>>);
static_assert(neo::totally_ordered<neo::object_box<int[12]>>);
static_assert(neo::totally_ordered<neo::object_box<int&>>);
static_assert(neo::totally_ordered<neo::object_box<void>>);
static_assert(neo::regular<neo::object_box<void>>);
