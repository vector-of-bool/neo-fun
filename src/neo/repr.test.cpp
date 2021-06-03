#include "./repr.hpp"

#include <map>
#include <string_view>

#include <catch2/catch.hpp>

static_assert(neo::reprable<std::string>);

TEST_CASE("repr() some integers") {
    auto v = neo::repr(12);
    CHECK(v == "12.i32");

    std::string_view str = "I am a string";
    CHECK(neo::repr(str) == "\"I am a string\"sv");

    std::vector vec = {1, 2, 3};
    CHECK(neo::repr(vec) == "vector<i32>{1.i32, 2.i32, 3.i32}");
    CHECK(neo::repr(true) == "true");

    int array[] = {1, 2, 3, 4};
    CHECK(neo::repr(array) == "i32[]{1.i32, 2.i32, 3.i32, 4.i32}");

    std::array<int, 5> arr;
    CHECK(neo::repr(arr) == "array<i32>{0.i32, 0.i32, 0.i32, 0.i32, 0.i32}");

    auto pair = std::pair{1, str};
    CHECK(neo::repr(pair) == "pair{1.i32, \"I am a string\"sv}");
}
