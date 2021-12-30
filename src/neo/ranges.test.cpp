#include <catch2/catch.hpp>

#include <neo/version.hpp>

#include "./ranges.hpp"

#include <array>
#include <list>

static_assert(
    std::ranges::random_access_range<neo::views::enumerate_view<std::vector<std::string>>>);
static_assert(
    std::ranges::random_access_range<neo::views::enumerate_view<const std::vector<std::string>&>>);

TEST_CASE("A range to a vector") {
    std::list<int> list;
    list.push_back(2);
    list.push_back(4);
    list.push_back(1);
    list.push_back(55);

    auto vec1 = list | neo::to_vector;
    static_assert(std::same_as<decltype(vec1), std::vector<int>>);
    CHECK(vec1 == std::vector<int>({2, 4, 1, 55}));
}

TEST_CASE("Enumerate a range") {
    std::list<int> list;
    list.push_back(0);
    list.push_back(2);
    list.push_back(4);
    list.push_back(6);

    auto enumer = neo::views::enumerate(list);
    for (const auto [idx, ref] : enumer) {
        CHECK(static_cast<int>(idx * 2) == ref);
    }

    auto io = std::views::iota(0) | neo::views::enumerate;
    for (const auto [idx, val] : io) {
        CHECK(static_cast<int>(idx) == val);
        if (val == 100) {
            break;
        }
    }
}

TEST_CASE("each() over a range") {
    std::array arr       = {1, 2, 3, 4};
    int        last_seen = 0;
    arr | neo::ranges::each([&](int i) {
        CHECK(i > 0);
        CHECK(i < 5);
        last_seen = i;
    });
    CHECK(last_seen == 4);
}

TEST_CASE("Distribute integers") {
    std::vector<int> evens;
    std::vector<int> odds;
    std::views::iota(0, 20)
        | neo::ranges::distribute(  //
            NEO_TL(_1 % 2),
            neo::ranges::write_into{std::back_inserter(evens)},
            neo::ranges::write_into{std::back_inserter(odds)});
    CHECK(evens == std::vector({0, 2, 4, 6, 8, 10, 12, 14, 16, 18}));
    CHECK(odds == std::vector({1, 3, 5, 7, 9, 11, 13, 15, 17, 19}));
}
