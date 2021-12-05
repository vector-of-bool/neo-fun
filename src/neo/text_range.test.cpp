#include "./text_range.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test") {
    std::string s = "Hello!";
    static_assert(neo::text_range<std::string>);
    static_assert(neo::text_range<std::string_view>);
    static_assert(neo::mutable_text_range<std::string>);
    static_assert(!neo::mutable_text_range<const std::string>);
    static_assert(!neo::mutable_text_range<std::string_view>);
    static_assert(!neo::text_view<std::string>);
    static_assert(neo::text_view<const char(&)[67]>);
    static_assert(neo::text_range_size("Hello!") == 6);
    static_assert(std::ranges::size("Hello!") == 7);

    std::same_as<std::string> auto dup = neo::copy_text(s);
    CHECK(dup == s);

    std::same_as<std::string_view> auto view1 = neo::view_text(s);
    CHECK(view1 == "Hello!");
    std::same_as<std::string_view> auto view2 = neo::view_text(view1);
    CHECK(view1 == view2);

    std::same_as<std::wstring> auto      w     = neo::copy_text(L"Wide string");
    std::same_as<std::wstring_view> auto wview = neo::view_text(w);
    CHECK(wview == L"Wide string");
}
