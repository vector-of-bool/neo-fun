#include <neo/string.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Convert a string") {
    std::string_view sv = "Hello, string!";
    auto             s  = neo::make_string(sv);
    static_assert(std::is_same_v<decltype(s), std::string>);
    CHECK(s == sv);

    std::wstring_view wsv = L"Hello, wide string!";
    auto              ws  = neo::make_string(wsv);
    static_assert(std::is_same_v<decltype(ws), std::wstring>);
    CHECK(ws == wsv);

    auto empty_ws = neo::make_empty_string_from(wsv);
    static_assert(std::is_same_v<decltype(empty_ws), std::wstring>);
    CHECK(empty_ws == L"");
}
