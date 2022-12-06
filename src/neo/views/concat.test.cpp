#include "./concat.hpp"

#include "../generator.hpp"
#include "../range_archetypes.hpp"
#include "../text_algo.hpp"

#include <catch2/catch.hpp>

namespace sr = std::ranges;
namespace sv = std::views;

using namespace std::literals;

using namespace neo::text_range_operators::equality;

TEST_CASE("Single range case") {
    std::string s;
    // A single viewable range is an sv::all_t of itself:
    std::same_as<sv::all_t<std::string&>> auto ref = neo::views::concat(s);
    (void)ref;
}

TEST_CASE("Two ranges") {
    std::string_view a = "foo";
    std::string_view b = "bar";

    std::same_as<neo::views::concat_view<std::string_view, std::string_view>> auto pair
        = neo::views::concat(a, b);
    CHECK(neo::copy_text(pair) == "foobar");
}

TEST_CASE("Empty subranges") {
    auto three = neo::views::concat("foo"sv, ""sv, "bar"sv);
    CHECK(neo::copy_text(three) == "foobar");

    auto trailing_empty = neo::views::concat("foo"sv, "bar"sv, ""sv);
    CHECK(neo::copy_text(trailing_empty) == "foobar"sv);

    auto leading_empty = neo::views::concat(""sv, "foo"sv, "bar"sv);
    CHECK(neo::copy_text(leading_empty) == "foobar"sv);

    auto consecutive_empty = neo::views::concat("foo"sv, ""sv, ""sv, "bar"sv);
    CHECK(neo::copy_text(consecutive_empty) == "foobar"sv);

    auto all_empty = neo::views::concat(""sv, ""sv, ""sv, ""sv);
    CHECK(neo::copy_text(all_empty) == ""sv);
    CHECK(std::ranges::empty(all_empty));
    CHECK(std::ranges::size(all_empty) == 0);
    CHECK(std::ranges::distance(all_empty) == 0);
}

TEST_CASE("Reversal") {
    auto simple = neo::views::concat("foo"sv, "bar"sv);
    CHECK(neo::copy_text(sv::reverse(simple)) == "raboof");

    auto three = neo::views::concat("foo"sv, ""sv, "bar"sv);
    CHECK(neo::copy_text(sv::reverse(three)) == "raboof");

    auto trailing_empty = neo::views::concat("foo"sv, "bar"sv, ""sv);
    CHECK(neo::copy_text(sv::reverse(trailing_empty)) == "raboof"sv);

    auto leading_empty = neo::views::concat(""sv, "foo"sv, "bar"sv);
    CHECK(neo::copy_text(sv::reverse(leading_empty)) == "raboof"sv);

    auto consecutive_empty = neo::views::concat("foo"sv, ""sv, ""sv, "bar"sv);
    CHECK(neo::copy_text(sv::reverse(consecutive_empty)) == "raboof"sv);

    auto all_empty = neo::views::concat(""sv, ""sv, ""sv, ""sv);
    CHECK(neo::copy_text(sv::reverse(all_empty)) == ""sv);
    CHECK(std::ranges::empty(sv::reverse(all_empty)));
    CHECK(std::ranges::size(sv::reverse(all_empty)) == 0);
    CHECK(std::ranges::distance(sv::reverse(all_empty)) == 0);
}

neo::generator<int> five() {
    co_yield 1;
    co_yield 2;
    co_yield 3;
    co_yield 4;
    co_yield 5;
}

TEST_CASE("Forward-only") {
    auto f1   = five();
    auto f2   = five();
    auto both = neo::views::concat(f1, f2);
    auto vec  = neo::to_vector(both);
    CHECK(vec == std::vector{1, 2, 3, 4, 5, 1, 2, 3, 4, 5});
}

static_assert(sr::input_range<neo::views::concat_t<sv::all_t<neo::arch::input_range<>&>>>);
