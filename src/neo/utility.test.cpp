#include <neo/utility.hpp>

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Take an object") {
    auto ptr = std::make_unique<int>(6);
    auto t   = neo::take(ptr);
    CHECK(!ptr);
    CHECK(t);
}

TEST_CASE("Comparisons") {
    auto val = 12;
    CHECK((val == neo::oper::any_of(1, 4, 12, 77)));
    CHECK_FALSE((val == neo::oper::any_of(1, 4, 62, 77)));
    CHECK((val != neo::oper::any_of(3, 1, 67, 9)));
    CHECK_FALSE((val != neo::oper::any_of(5, 12, 6, 1)));

    CHECK((val == neo::oper::none_of(1, 33, 5, 1)));
    CHECK_FALSE((val == neo::oper::none_of(1, 33, 5, 12)));
}
