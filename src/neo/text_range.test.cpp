#include "./text_range.hpp"

#include "./text_algo.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test") {
    std::string s = "Hello!";
    static_assert(neo::text_range<std::string>);
    static_assert(neo::text_range<std::string_view>);
    static_assert(neo::mutable_text_range<std::string>);
    static_assert(!neo::mutable_text_range<const std::string>);
    static_assert(!neo::mutable_text_range<std::string_view>);
    static_assert(!neo::text_view<std::string>);
    static_assert(neo::text_range<const char(&)[67]>);
    static_assert(neo::text_array_ref<const char(&)[67]>);
    static_assert(neo::text_range_size("Hello!") == 6);
    static_assert(std::ranges::size("Hello!") == 7);

    static_assert(not neo::character_type<int>);

    static_assert(std::same_as<std::string_view, neo::view_text_t<std::string_view>>);
    static_assert(std::same_as<std::string_view, neo::view_text_t<std::string_view&>>);
    static_assert(std::same_as<std::string_view, neo::view_text_t<const std::string_view&>>);
    static_assert(std::same_as<std::string_view, neo::view_text_t<std::string_view&&>>);

    neo::text_view auto s1 = neo::view_text("hello!");
    CHECK(s1.size() == 6);
    CHECK(neo::text_range_equal_to{}(s1, "hello!"));
    auto s2 = neo::to_std_string(s1);

    auto s3 = neo::to_std_string("I am a string");
    CHECK(s3 == "I am a string");

    std::same_as<std::allocator<char>> auto _ = neo::text_allocator("I am a bare string literal");

    std::same_as<std::string> auto dup = neo::copy_text(s);
    CHECK(dup == s);

    auto view1 = neo::view_text(s);
    CHECK(neo::text_range_equal_to{}(view1, "Hello!"));
    auto view2 = neo::view_text(view1);
    CHECK(neo::text_range_equal_to{}(view1, view2));

    std::same_as<std::wstring> auto w     = neo::copy_text(L"Wide string");
    auto                            wview = neo::view_text(w);
    CHECK(neo::text_range_equal_to{}(wview, L"Wide string"));
}
