#include "./platform.hpp"

NEO_GNU_LIKE_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations");

#include <neo/ad_hoc_range.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Create a simple ad-hoc range") {  //
    int arr[] = {1, 2, 3, 4};

    neo::ad_hoc_range<int*, int*> r  = neo::ad_hoc_range{arr, arr + 4};
    auto                          it = r.begin();
    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    ++it;
    CHECK(*it == 3);
    ++it;
    CHECK(*it == 4);
    ++it;

    CHECK(it == r.end());
    for (auto v : r) {
        CHECK(v > 0);
        CHECK(v < 5);
    }
}