#include "./any_range.hpp"

#include "./range_concepts.hpp"
#include "./test_concept.hpp"

#include <catch2/catch.hpp>

NEO_TEST_CONCEPT(neo::ranges::input_range<neo::any_input_range<int>>);

void foo(neo::erased_input_range<const int&>&& range) {
    int counter = 0;
    for (auto v : range) {
        CHECK(v > 0);
        CHECK(v < 5);
        ++counter;
    }
    CHECK(counter == 4);
}

TEST_CASE("Erase a range") {
    const std::vector<int> vec = {1, 2, 3, 4};
    foo(neo::erase_input_range(vec));
}

TEST_CASE("Create an any_range for vector<int>") {
    std::vector<int>          vec = {1, 2, 3, 4};
    neo::any_input_range<int> r{vec};

    int counter = 0;
    for (auto v : r) {
        CHECK(v > 0);
        CHECK(v < 5);
        ++counter;
    }
    CHECK(counter == 4);

    auto v = neo::erase_input_range(vec);

    neo::any_input_range v2 = v;

    // Convert the range of ints to a range of doubles
    neo::any_input_range<double> d_r = r;

    double d_counter = 0;
    for (auto d : d_r) {
        d_counter += d;
    }
    CHECK(d_counter == (1.0 + 2.0 + 3.0 + 4.0));
}
