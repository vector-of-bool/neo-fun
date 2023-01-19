#include "./repr.hpp"

#include <array>
#include <filesystem>
#include <map>
#include <optional>
#include <string_view>

#include <catch2/catch.hpp>

static_assert(neo::reprable<std::int32_t>);
static_assert(neo::reprable<std::string>);
static_assert(neo::reprable<std::string_view>);
static_assert(neo::reprable<std::wstring>);
static_assert(neo::reprable<std::wstring_view>);

#if _WIN32
#include <windows.h>
static_assert(neo::reprable<DWORD>);
#endif

struct unknown_thing {};

/// Fails:
// static_assert(neo::reprable<unknown_thing>);
// ^^ This static assert should be uncommented to guage the readability of error
// messages involving repr() on un-reprable types

static_assert(!neo::reprable<unknown_thing>);

using i32 = std::int32_t;

TEST_CASE("repr() some numbers") {
    auto v = neo::repr(12).string();
    CHECK(v == "12:int32");

    CHECK(neo::repr_type<i32*>().string() == "int32*");
    CHECK(neo::repr_type<const i32*>().string() == "int32 const*");
    CHECK(neo::repr(true).string() == "true");

    double d = 33.02;
    CHECK_THAT(neo::repr(d).string(), Catch::Matches("33\\.02.*"));
    float f = 4.1f;
    CHECK_THAT(neo::repr(f).string(), Catch::Matches("4\\.1.*f"));
}

TEST_CASE("repr() some vectors/arrays") {
    std::vector vec = {1, 2, 3};
    CHECK(neo::repr(vec).string() == "vector<int32>{1, 2, 3}");

    i32 array[] = {1, 2, 3, 4};
    CHECK(neo::repr(array).string() == "int32[]{1, 2, 3, 4}");

    std::array<i32, 5> arr{};
    CHECK(neo::repr(arr).string() == "array<int32>{0, 0, 0, 0, 0}");

    std::array<unknown_thing, 4> unk;
    CHECK(neo::repr(unk).string() == "array<?>{?, ?, ?, ?}");
}

TEST_CASE("Repr a tuple and a pair") {
    auto pair = std::pair{1, std::string_view("I am a string")};
    CHECK(neo::repr(pair).string() == "pair{1:int32, \"I am a string\"sv}");

    auto tup = std::tuple{1, 3, std::string{"eggs"}};
    CHECK(neo::repr(tup).string() == "tuple{1:int32, 3:int32, \"eggs\"s}");
    CHECK(neo::repr_type<decltype(tup)>().string() == "tuple<int32, int32, std::string>");
    CHECK(neo::repr_value(tup).string() == "{1, 3, \"eggs\"}");

    auto unk = std::tuple{std::string{"hello"}, unknown_thing{}, 42};
    CHECK(neo::repr(unk).string() == "tuple{\"hello\"s, ?, 42:int32}");
}

TEST_CASE("Repr some strings") {
    std::string_view str = "I am a string";
    CHECK(neo::repr(str).string() == "\"I am a string\"sv");

    str = "String with \" a quote in it";
    CHECK(neo::repr(str).string() == "\"String with \\\" a quote in it\"sv");
}

static_assert(neo::reprable<std::map<std::string, i32>::value_type>);

TEST_CASE("Repr a map") {
    std::map<std::string, i32> map;
    map.emplace("the answer", 42);
    map.emplace("the question", 1729);
    auto rep = neo::repr(map).string();

    CHECK(rep == "map<std::string, int32>{[\"the answer\" => 42], [\"the question\" => 1729]}");

    CHECK(neo::repr_type<i32*>().string() == "int32*");
    CHECK(neo::repr_type<const i32*>().string() == "int32 const*");
    CHECK(neo::repr_type<std::pair<i32, const i32*>>().string() == "pair<int32, int32 const*>");
}

TEST_CASE("repr() some pointers") {
    i32  i  = 4;
    auto p  = &i;
    auto p1 = &p;
    CHECK_THAT(neo::repr(p).string(), Catch::StartsWith("[int32* (0x"));
    CHECK_THAT(neo::repr(p1).string(), Catch::StartsWith("[int32** (0x"));

    void*       vp  = &i;
    const void* cvp = &i;
    CHECK_THAT(neo::repr(vp).string(), Catch::StartsWith("[void* 0x"));
    CHECK_THAT(neo::repr(cvp).string(), Catch::StartsWith("[void const* 0x"));

    unknown_thing thing;
    CHECK_THAT(neo::repr(&thing).string(), Catch::StartsWith("[unknown-type* 0x"));
}

TEST_CASE("repr() an optional") {
    std::optional<int> opt = 332;
    CHECK(neo::repr(opt).string() == "optional<int32>{->332}");
    opt.reset();
    CHECK(neo::repr(opt).string() == "optional<int32>{nullopt}");

    std::optional<unknown_thing> unk = unknown_thing{};
    CHECK(neo::repr(unk).string() == "optional<?>{->?}");
}

static_assert(neo::repr_detail::detect_path<std::filesystem::path>);
TEST_CASE("repr() a filesystem path") {
    auto cwd    = std::filesystem::current_path();
    auto rep    = neo::repr(cwd).string();
    auto expect = neo::ufmt("path{{}}", neo::repr_value(cwd.string()));
    CHECK(rep == expect);
}
