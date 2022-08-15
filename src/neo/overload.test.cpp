#include "./overload.hpp"

#include <catch2/catch.hpp>

using namespace std::literals;

TEST_CASE("Create an overload") {
    auto f = neo::overload([] { return 12; });
    CHECK(f() == 12);

    auto f2 = neo::overload([] { return 12; },  //
                            [](int) { return "string"s; });
    CHECK(f2(1) == "string");
    CHECK(f2() == 12);
}

TEST_CASE("Create an ordered overload") {
    auto nullary = neo::ordered_overload([]() { return 33; });
    CHECK(nullary() == 33);

    auto math = neo::ordered_overload{
        [](int n) { return n * 2; },
        [](double d) { return d / 2; },
    };

    // Normally ambiguous overload, but this chooses the first viable invocable
    CHECK(math(4u) == 8);
}

TEST_CASE("Empty overload") {
    neo::overload o{};
    std::ignore = o;
}