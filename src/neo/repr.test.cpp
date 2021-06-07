#include "./repr.hpp"

#include <filesystem>
#include <map>
#include <optional>
#include <string_view>

#include <catch2/catch.hpp>

static_assert(neo::reprable<std::int32_t>);
static_assert(neo::reprable<std::string>);

struct unknown_thing {};

static_assert(!neo::reprable<unknown_thing>);

using i32 = std::int32_t;

TEST_CASE("repr() some integers") {
    auto v = neo::repr(12);
    CHECK(v == "12:int32");

    CHECK(neo::repr_type<i32*>() == "int32*");
    CHECK(neo::repr_type<const i32*>() == "int32 const*");
    CHECK(neo::repr(true) == "true");
}

TEST_CASE("repr() some vectors/arrays") {
    std::vector vec = {1, 2, 3};
    CHECK(neo::repr(vec) == "vector<int32>{1, 2, 3}");

    i32 array[] = {1, 2, 3, 4};
    CHECK(neo::repr(array) == "int32[]{1, 2, 3, 4}");

    std::array<i32, 5> arr{};
    CHECK(neo::repr(arr) == "array<int32>{0, 0, 0, 0, 0}");
}

TEST_CASE("Repr a tuple and a pair") {
    auto pair = std::pair{1, std::string_view("I am a string")};
    CHECK(neo::repr(pair) == "pair{1:int32, \"I am a string\"sv}");
}

TEST_CASE("Repr some strings") {
    std::string_view str = "I am a string";
    CHECK(neo::repr(str) == "\"I am a string\"sv");

    str = "String with \" a quote in it";
    CHECK(neo::repr(str) == "\"String with \\\" a quote in it\"sv");
}

static_assert(neo::reprable<std::map<std::string, i32>::value_type>);

TEST_CASE("Repr a map") {
    std::map<std::string, i32> map;
    map.emplace("the answer", 42);
    map.emplace("the question", 1729);
    auto rep = neo::repr(map);

    CHECK(rep == "map<std::string, int32>{[\"the answer\" => 42], [\"the question\" => 1729]}");

    CHECK(neo::repr_type<i32*>() == "int32*");
    CHECK(neo::repr_type<const i32*>() == "int32 const*");
    CHECK(neo::repr_type<std::pair<i32, const i32*>>() == "pair<int32, int32 const*>");
}

TEST_CASE("repr() some pointers") {
    i32  i  = 4;
    auto p  = &i;
    auto p1 = &p;
    CHECK(neo::repr(p) == "[int32* [4]]");
    CHECK(neo::repr(p1) == "[int32** [[4]]]");

    unknown_thing thing;
    CHECK(neo::repr(&thing).starts_with("[unknown-pointer 0x"));
}

TEST_CASE("repr() an optional") {
    std::optional<int> opt = 332;
    CHECK(neo::repr(opt) == "[optional<int32> [332]]");
    opt.reset();
    CHECK(neo::repr(opt) == "[optional<int32> nullopt]");
}

static_assert(neo::repr_detail::detect_path<std::filesystem::path>);
TEST_CASE("repr() a filesystem path") {
    auto cwd    = std::filesystem::current_path();
    auto rep    = neo::repr(cwd);
    auto expect = neo::ufmt("[path {}]", neo::repr_value(cwd.string()));
    CHECK(rep == expect);
}
