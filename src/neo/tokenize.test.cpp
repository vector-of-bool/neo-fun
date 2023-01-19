#include "./tokenize.hpp"

#include "./ranges.hpp"

#include "./tl.hpp"
#include <catch2/catch.hpp>

constexpr inline auto eq = neo::text_range_equal_to{};

TEST_CASE("Tokenize a simple string") {
    std::string    s = "a b c";
    neo::tokenizer toks{s, neo::whitespace_splitter{}};

    static_assert(std::forward_iterator<decltype(toks)::iterator>);
    static_assert(std::ranges::forward_range<decltype(toks)>);
    static_assert(std::ranges::borrowed_range<decltype(toks)>);
    auto n_toks = std::ranges::distance(toks);
    CHECK(n_toks == 3);

    auto it = toks.begin();
    REQUIRE(it != toks.end());
    CHECK(eq(*it, "a"));
    ++it;
    CHECK(eq(*it, "b"));
    ++it;
    CHECK(eq(*it, "c"));
}

TEST_CASE("Tokenize an owned string") {
    neo::tokenizer toks{std::string{"foo bar baz"}, neo::whitespace_splitter{}};
    static_assert(std::ranges::forward_range<decltype(toks)>);

    auto it = toks.begin();
    CHECK(eq(*it, "foo"));
    ++it;
    CHECK(eq(*it, "bar"));
}

TEST_CASE("Iterate some lines") {
    std::string s     = "foo\nbar\nbaz";
    auto        iter  = neo::iter_lines(s);
    auto        lines = neo::to_vector(iter);
    CHECKED_IF(lines.size() == 3) {
        CHECK(eq(lines[0], "foo"));
        CHECK(eq(lines[1], "bar"));
        CHECK(eq(lines[2], "baz"));
    }

    // Final newline causes another empty line:
    s     = "foo\n\nbar\nbaz\n";
    iter  = neo::iter_lines(s);
    lines = neo::to_vector(iter);
    CHECKED_IF(lines.size() == 5) {
        CHECK(eq(lines[0], "foo"));
        CHECK(eq(lines[1], ""));
        CHECK(eq(lines[2], "bar"));
        CHECK(eq(lines[3], "baz"));
        CHECK(eq(lines[4], ""));
    }
}
