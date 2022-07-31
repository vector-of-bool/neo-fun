#include "./any_range.hpp"

#include "./iterator_facade.hpp"
#include "./range_archetypes.hpp"
#include "./range_concepts.hpp"
#include "./ranges.hpp"
#include "./test_concept.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <ranges>

NEO_TEST_CONCEPT(neo::ranges::range<neo::any_input_range<int>>);
NEO_TEST_CONCEPT(neo::ranges::input_range<neo::any_input_range<int>>);

static_assert(!std::copyable<neo::any_input_range<int>>);
static_assert(std::movable<neo::any_input_range<int>>);

struct with_any_range {
    neo::any_input_range<int> r;
};

static_assert(std::movable<with_any_range>);

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

TEST_CASE("A transform_view as an input_range") {
    auto                              arr = std::array{1, 21, 37, 4};
    neo::any_input_range<std::string> as_strings
        = arr | std::views::transform([](auto i) { return std::to_string(i); });
    auto tr2 = as_strings | std::views::transform([](auto s) { return s.length(); });
    neo::any_input_iterator<std::string::size_type> iv(tr2.begin());
    neo::any_input_range<std::string::size_type>    as_lengths = tr2;

    auto vec = neo::to_vector(as_lengths);
    CHECK(vec == std::vector<std::string::size_type>({1, 2, 2, 1}));
}

TEMPLATE_TEST_CASE("Various combinations",
                   "",
                   std::vector<int>,
                   std::vector<double>,
                   std::deque<int>,
                   std::deque<double>,
                   std::list<int>,
                   std::list<double>,
                   std::forward_list<int>,
                   std::forward_list<double>,
                   neo::arch::input_range<neo::arch::pathological&>,
                   (std::ranges::transform_view<
                       std::ranges::ref_view<neo::arch::input_range<neo::arch::pathological&>>,
                       std::function<double(neo::arch::pathological&)>>)) {
    using container = TestType;

    using container_tag = neo::iter_concept_t<std::ranges::iterator_t<container>>;
    using ref_type      = std::ranges::range_reference_t<container&>;

    static_assert(std::convertible_to<container, neo::any_range<ref_type, container_tag>>);

    if constexpr (std::ranges::random_access_range<container>) {
        static_assert(std::convertible_to<container, neo::any_random_access_range<ref_type>>);
    } else {
        static_assert(!std::convertible_to<container, neo::any_random_access_range<ref_type>>);
    }

    if constexpr (std::ranges::bidirectional_range<container>) {
        static_assert(std::convertible_to<container, neo::any_bidirectional_range<ref_type>>);
    } else {
        static_assert(!std::convertible_to<container, neo::any_bidirectional_range<ref_type>>);
    }

    if constexpr (std::ranges::forward_range<container>) {
        static_assert(std::convertible_to<container, neo::any_forward_range<ref_type>>);
        static_assert(std::convertible_to<container, neo::any_forward_range<ref_type>>);
    } else {
        static_assert(!std::convertible_to<container, neo::any_forward_range<ref_type>>);
    }

    if constexpr (std::ranges::input_range<container>) {
        static_assert(std::convertible_to<container, neo::any_input_range<ref_type>>);
    } else {
        static_assert(!std::convertible_to<container, neo::any_input_range<ref_type>>);
    }
}
