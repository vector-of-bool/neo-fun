#include <neo/utility.hpp>

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Take an object") {
    auto ptr = std::make_unique<int>(6);
    auto t   = neo::take(ptr);
    CHECK(!ptr);
    CHECK(t);
}
