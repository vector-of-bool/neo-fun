#include "./text_algo.hpp"

#include "./ranges.hpp"
#include "./ufmt.hpp"
#include "./zstring_view.hpp"

#include <catch2/catch.hpp>

#include <sstream>

using namespace std::literals;

TEST_CASE("Trim a string") {
    std::string s  = " foo bar  ";
    auto        s2 = neo::trim(s);
    CHECK(s2 == "foo bar");
}

TEST_CASE("Join some strings") {
    auto strs = {"foo"sv, "bar"sv, "baz"sv};

    auto fin = neo::join_text(strs, "-");
    CHECK(fin == "foo-bar-baz");
}

TEST_CASE("The empty string") {
    auto empty = neo::str_concat();
    CHECK(neo::text_range_equal_to{}(empty, ""));
    CHECK(neo::text_range_equal_to{}(empty, ""s));
    CHECK(neo::text_range_equal_to{}(empty, ""sv));

    CHECK(std::string(empty) == "");
    CHECK(std::wstring(empty) == L"");
    CHECK(std::u32string(empty) == U"");

    std::stringstream strm;
    strm << "foo" << neo::text_range_ostream_inserter{empty} << "bar";
    CHECK(strm.str() == "foobar");
}

static_assert(std::ranges::borrowed_range<neo::str_concat_t<>>);
auto n = std::ranges::begin(neo::str_concat());
static_assert(std::ranges::forward_range<neo::str_concat_t<>>);

TEST_CASE("Concatenating strings") {
    SECTION("Empty") {
        auto s = neo::str_concat();
        CAPTURE(s);
        CHECK(neo::text_range_equal_to{}(s, ""));
        CHECK(std::ranges::distance(s) == 0);
    }

    SECTION("Single") {
        auto s = neo::str_concat("foo");
        static_assert(std::ranges::contiguous_range<decltype(s)>);
        auto it = s.begin();
        REQUIRE(it != s.end());
        CHECK(*it == 'f');
        ++it;
        REQUIRE(it != s.end());
        CHECK(*it == 'o');
        ++it;
        REQUIRE(it != s.end());
        CHECK(*it == 'o');
        ++it;
        CHECK(it == s.end());
        --it;
        CHECK(*it == 'o');
        --it;
        CHECK(*it == 'o');
        --it;
        CHECK(*it == 'f');
        CHECK(it == s.begin());
        CHECK((s.end() - it) == 3);
        CAPTURE(s);
        CHECK(neo::text_range_equal_to{}(s, "foo"));
        CHECK(s[1] == 'o');
        CHECK(s.begin()[1] == 'o');

        CHECK((neo::text_range_compare_3way{}(s, "zzz"sv) < 0));
        CHECK((neo::text_range_compare_3way{}(s, "aaa"sv) > 0));
    }

    SECTION("Two") {
        constexpr auto s = neo::str_concat("foo", "bar");
        static_assert(neo::text_range_equal_to{}(s, "foobar"));
        CAPTURE(s);
        CHECK(neo::text_range_equal_to{}(s, "foobar"));
        CHECK(std::ranges::size(s) == 6);
    }

    SECTION("Empty inside") {
        constexpr auto s = neo::str_concat("foo", "", "bar");
        CAPTURE(s);
        CHECK(neo::text_range_equal_to{}(s, "foobar"));
        static_assert(neo::text_range_equal_to{}(s, "foobar"));
    }

    SECTION("Concat differing types") {
        auto s2 = neo::str_concat("foo"sv, "bar"s, "baz", " ", neo::zstring_view{"meow"});
        CHECK(neo::text_range_equal_to{}(s2, "foobarbaz meow"));
        static_assert(neo::text_range<decltype(s2)>);
        CHECK(s2[9] == ' ');

        auto stdstr = neo::to_std_string(s2);
        CHECK(stdstr == "foobarbaz meow");

        auto s3 = neo::ufmt("foo {}", neo::text_range_formatter{s2});
        CHECK(s3 == "foo foobarbaz meow");
    }
}
