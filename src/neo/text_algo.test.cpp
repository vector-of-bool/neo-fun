#include "./text_algo.hpp"

#include "./zstring_view.hpp"

#include <catch2/catch.hpp>
#include <neo/ranges.hpp>

#include <sstream>

using namespace std::literals;

using namespace neo::text_range_operators;

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
    CHECK(empty == "");
    CHECK(empty == ""s);
    CHECK(empty == ""sv);

    CHECK(std::string(empty) == "");
    CHECK(std::wstring(empty) == L"");
    CHECK(std::u32string(empty) == U"");

    std::stringstream strm;
    strm << "foo" << empty << "bar";
    CHECK(strm.str() == "foobar");
}

static_assert(std::ranges::borrowed_range<neo::str_concat_t<>>);
auto n = std::ranges::begin(neo::str_concat());
static_assert(std::ranges::forward_range<neo::str_concat_t<>>);

TEST_CASE("Concatenating strings") {
    SECTION("Empty") {
        auto s = neo::str_concat();
        CAPTURE(s);
        CHECK(s == "");
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
        CHECK(s == "foo");
        CHECK(s[1] == 'o');
        CHECK(s.begin()[1] == 'o');

        CHECK(s < "zzz"sv);
        CHECK(s > "aaa");
    }

    SECTION("Two") {
        constexpr auto s = neo::str_concat("foo", "bar");
        static_assert(s == "foobar");
        CAPTURE(s);
        CHECK(s == "foobar");
        CHECK(std::ranges::size(s) == 6);
    }

    SECTION("Empty inside") {
        constexpr auto s = neo::str_concat("foo", "", "bar");
        CAPTURE(s);
        CHECK(s == "foobar");
        static_assert(s == "foobar");
    }

    SECTION("Concat differing types") {
        auto s2 = neo::str_concat("foo"sv, "bar"s, "baz", " ", neo::zstring_view{"meow"});
        CHECK(s2 == "foobarbaz meow");
        static_assert(neo::text_range<decltype(s2)>);
        CHECK(s2[9] == ' ');

        auto stdstr = neo::to_std_string(s2);
        CHECK(stdstr == "foobarbaz meow");
    }
}
