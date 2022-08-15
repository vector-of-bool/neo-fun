#include "./assignable_box.hpp"

#include "./archetypes.hpp"
#include "./repr.hpp"

#include <catch2/catch.hpp>

#include <memory>

static_assert(std::constructible_from<neo::assignable_box<int>, int>);
static_assert(std::assignable_from<neo::assignable_box<int>&, neo::assignable_box<int>>);
static_assert(std::assignable_from<neo::assignable_box<int&>&, neo::assignable_box<int&>>);

static_assert(std::copyable<neo::assignable_box<int>>);
static_assert(std::copyable<neo::assignable_box<int&>>);
static_assert(std::copyable<neo::assignable_box<const int&>>);
static_assert(not std::copyable<neo::assignable_box<const int>>);
static_assert(std::movable<neo::assignable_box<std::unique_ptr<int>>>);
static_assert(std::movable<neo::assignable_box<std::unique_ptr<int>&>>);

static_assert(not std::default_initializable<neo::assignable_box<int&>>);
static_assert(std::default_initializable<neo::assignable_box<int>>);

TEST_CASE("Construct some boxes") {
    neo::assignable_box b1{7};
    static_assert(std::same_as<decltype(b1), neo::assignable_box<int>>);
    int                 value = 9;
    neo::assignable_box b2{value};
    static_assert(std::same_as<decltype(b2), neo::assignable_box<int&>>);

    CAPTURE(neo::repr(b1));
    CAPTURE(neo::repr(b2));
}
