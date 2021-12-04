#include "./any_range.hpp"

#include "./iterator_facade.hpp"
#include "./range_concepts.hpp"
#include "./test_concept.hpp"

#include <catch2/catch.hpp>

#include <ranges>

NEO_TEST_CONCEPT(neo::ranges::range<neo::any_input_range<int>>);
NEO_TEST_CONCEPT(neo::ranges::input_range<neo::any_input_range<int>>);

void foo(neo::any_input_range<const int&>&& range) {
    int  counter = 0;
    auto it      = range.begin();
    auto end     = range.end();
    for (; it != end; ++it) {
        auto v = *it;
        CHECK(v > 0);
        CHECK(v < 5);
        ++counter;
    }
    CHECK(counter == 4);
}

TEST_CASE("Erase a range") {
    const std::vector<int> vec = {1, 2, 3, 4};
    foo(neo::any_range(vec));
}

TEST_CASE("Create an any_range for vector<int>") {
    std::vector<int>                  vec = {1, 2, 3, 4};
    neo::any_bidirectional_range<int> r{vec};

    int counter = 0;
    for (auto v : r) {
        CHECK(v > 0);
        CHECK(v < 5);
        ++counter;
    }
    CHECK(counter == 4);

    neo::any_forward_range<int> v2  = std::move(r);
    neo::any_input_range<int>   ref = std::ref(v2);

    // Convert the range of ints to a range of doubles
    neo::any_input_range<double> d_r = std::ref(v2);

    double d_counter = 0;
    for (auto d : d_r) {
        d_counter += d;
    }
    CHECK(d_counter == (1.0 + 2.0 + 3.0 + 4.0));
}

TEST_CASE("Create an any_range for an uncommon range") {
    struct until_7_iter : neo::iterator_facade<until_7_iter> {
        int value = 0;
        struct sentinel_type {};

        auto dereference() const noexcept { return value; }
        auto increment() noexcept { ++value; }

        auto distance_to(sentinel_type) const noexcept { return 7 - value; }
        bool operator==(sentinel_type s) const noexcept { return distance_to(s) == 0; }
    };

    struct seven_range {
        auto begin() { return until_7_iter(); }
        auto end() { return until_7_iter::sentinel_type(); }
    };

    seven_range    svn;
    neo::any_range any_svn = svn;

    auto plus_two = any_svn | std::views::transform([](auto i) { return i + 2; });
    auto it       = plus_two.begin();
    CHECK(*it == 2);
    std::ranges::advance(it, 5);
    CHECK(*it == 7);
    CHECK(it != plus_two.end());
    ++it;
    ++it;
    CHECK(it == plus_two.end());
}