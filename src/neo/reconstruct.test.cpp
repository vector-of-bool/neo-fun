#include "./reconstruct.hpp"

#include <catch2/catch.hpp>

static_assert(neo::range_detail::detect_string<std::string>);

TEST_CASE("Reconstruct a string") {
    std::string s  = "Hello, world!";
    auto        s2 = neo::reconstruct_range(s, s.begin() + 3, s.end());
    CHECK(s2 == "lo, world!");
    s2 = neo::reconstruct_range(std::as_const(s), s.begin() + 5, s.end());
    CHECK(s2 == ", world!");

    std::string_view sv  = s;
    auto             sv2 = neo::reconstruct_range(sv, sv.begin() + 1, sv.end() - 3);
    CHECK(sv2 == "ello, wor");
}

TEST_CASE("Reconstruct a vector") {
    std::vector<int> r  = {1, 2, 3};
    auto             r2 = neo::reconstruct_range(r, r.begin() + 1, r.end());
    CHECK(r2 == std::vector<int>{2, 3});
}
