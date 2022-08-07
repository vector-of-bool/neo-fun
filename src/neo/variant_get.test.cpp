#include "./variant_get.hpp"

#include <catch2/catch.hpp>

#include <variant>

static_assert(neo::supports_alternative<std::variant<int>, int>);
static_assert(!neo::supports_alternative<std::variant<int>, std::string>);

static_assert(not neo::supports_alternative<int, double>);

TEST_CASE("Get an alternative") {
    std::variant<int, std::string> s = 12;
    CHECK(neo::holds_alternative<int>(s));
}
