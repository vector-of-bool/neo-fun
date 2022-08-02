#include "./tokenize.hpp"

#include "./ranges.hpp"

#include "./tl.hpp"
#include <catch2/catch.hpp>

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
    CHECK(it->view == "a");
    ++it;
    CHECK(it->view == "b");
    ++it;
    CHECK(it->view == "c");
}

TEST_CASE("Tokenize an owned string") {
    neo::tokenizer toks{std::string{"foo bar baz"}, neo::whitespace_splitter{}};
    static_assert(std::ranges::forward_range<decltype(toks)>);

    auto it = toks.begin();
    CHECK(it->view == "foo");
    ++it;
    CHECK(it->view == "bar");
}
