#include "./range_concepts.hpp"

#include "./test_concept.hpp"

#include <string>
#include <vector>

NEO_TEST_CONCEPT(neo::ranges::input_range<std::vector<int>>);
NEO_TEST_CONCEPT(neo::ranges::input_range<std::string>);
NEO_TEST_CONCEPT(neo::ranges::random_access_range<std::string>);

#include <catch2/catch.hpp>

// This is required in order to link in some configurations MSVC
TEST_CASE("nil") {}
