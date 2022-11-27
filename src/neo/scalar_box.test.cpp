#include "./scalar_box.hpp"

#include "./archetypes.hpp"
#include "./repr.hpp"

#include <catch2/catch.hpp>

#include <memory>

static_assert(std::constructible_from<neo::scalar_box<int>, int>);
static_assert(std::assignable_from<neo::scalar_box<int>&, neo::scalar_box<int>>);
static_assert(std::assignable_from<neo::scalar_box<int&>&, neo::scalar_box<int&>>);

static_assert(std::copyable<neo::scalar_box<int>>);
static_assert(std::copyable<neo::scalar_box<int&>>);
static_assert(std::copyable<neo::scalar_box<const int&>>);
static_assert(not std::copyable<neo::scalar_box<const int>>);
static_assert(std::movable<neo::scalar_box<std::unique_ptr<int>>>);
static_assert(std::movable<neo::scalar_box<std::unique_ptr<int>&>>);

static_assert(not std::default_initializable<neo::scalar_box<int&>>);
static_assert(std::default_initializable<neo::scalar_box<int>>);

TEST_CASE("Construct some boxes") {
    neo::scalar_box b1{7};
    static_assert(std::same_as<decltype(b1), neo::scalar_box<int>>);
    int             value = 9;
    neo::scalar_box b2{value};
    static_assert(std::same_as<decltype(b2), neo::scalar_box<int&>>);

    CAPTURE(neo::repr(b1));
    CAPTURE(neo::repr(b2));
}

TEST_CASE("Void box") {
    static_assert(std::semiregular<neo::scalar_box<void>>);
    static_assert(std::semiregular<neo::scalar_box<const void>>);
    static_assert(std::semiregular<neo::scalar_box<volatile void>>);
    static_assert(std::semiregular<neo::scalar_box<const volatile void>>);
}

TEST_CASE("Array box") {
    static_assert(std::semiregular<neo::scalar_box<int[12]>>);
    int  carr[3] = {1, 2, 3};
    auto arr     = neo::scalar_box{std::as_const(carr)};
    CHECK(arr.get()[0] == 1);
    CHECK(arr.get()[1] == 2);
    CHECK(arr.get()[2] == 3);

    neo::scalar_box box{carr};
    static_assert(std::same_as<decltype(box), neo::scalar_box<int(&)[3]>>);
    CHECK(box.get()[0] == 1);
    CHECK(box.get()[1] == 2);
    CHECK(box.get()[2] == 3);

    neo::scalar_box box2{NEO_MOVE(carr)};
    static_assert(std::same_as<decltype(box2), neo::scalar_box<int[3]>>);
    CHECK(box2.get()[0] == 1);
    CHECK(box2.get()[1] == 2);
    CHECK(box2.get()[2] == 3);
}
