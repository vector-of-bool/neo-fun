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
