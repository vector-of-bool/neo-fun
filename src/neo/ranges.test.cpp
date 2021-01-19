#include <catch2/catch.hpp>

#include <neo/version.hpp>

#if __cpp_lib_ranges
#include "./ranges.hpp"

#include <array>
#include <list>

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
        CHECK((idx * 2) == ref);
    }

    auto io = std::views::iota(0) | neo::views::enumerate;
    for (const auto [idx, val] : io) {
        CHECK(idx == val);
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

#endif