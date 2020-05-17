#include "./returns.hpp"

#include <catch2/catch.hpp>

template <typename T>
auto func(T t) NEO_RETURNS(t* t);

TEST_CASE("Simple function") { CHECK(func(2) == 4); }

TEST_CASE("Lambda") {
    auto l = []() NEO_RETURNS_L(3);
    CHECK(l() == 3);
}
