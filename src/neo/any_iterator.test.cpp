#include "./any_iterator.hpp"

#include "./test_concept.hpp"
#include "./utf8.hpp"

#include <forward_list>

#include <catch2/catch.hpp>

// NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int>>);
// NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int&>>);
// NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<const int&>>);
// NEO_TEST_CONCEPT(neo::sentinel_for<neo::any_sentinel, neo::any_input_iterator<int>>);

TEST_CASE("Create an erased input_iterator") {
    std::forward_list<int> fl = {1, 2, 3, 4};

    neo::any_iterator fit{fl.cbegin()};
    CHECK(*fit == 1);
    (void)fit;

    std::vector<int>  vec = {1, 2, 3, 4};
    neo::any_iterator it{vec.cbegin()};
    CHECK(it == it);
    neo::any_sentinel end{vec.cend()};
    CHECK(it != end);
    auto it2 = it;
    CHECK(it2 == it);
    CHECK(it2 != end);
    ++it2;
    CHECK(it != it2);
    CHECK((it2 - it) == 1);

    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    CHECK(it != end);
    ++it;
    ++it;
    ++it;
    CHECK(it == end);

    static_assert(neo::detail::sized_sentinel_of<decltype(it), decltype(it)>);
}

NEO_TEST_CONCEPT(neo::input_iterator<neo::any_random_access_iterator<int>>);
NEO_TEST_CONCEPT(neo::forward_iterator<neo::any_random_access_iterator<int>>);
NEO_TEST_CONCEPT(neo::bidirectional_iterator<neo::any_random_access_iterator<int>>);
NEO_TEST_CONCEPT(neo::random_access_iterator<neo::any_random_access_iterator<int>>);

static_assert(neo::detail::iter_is_forward<neo::any_forward_iterator<int>>);
NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int>>);
NEO_TEST_CONCEPT(neo::forward_iterator<neo::any_forward_iterator<int>>);
NEO_TEST_CONCEPT(neo::bidirectional_iterator<neo::any_bidirectional_iterator<int>>);
NEO_TEST_CONCEPT(neo::random_access_iterator<neo::any_random_access_iterator<int>>);

static_assert(
    std::convertible_to<std::vector<int>::iterator, neo::any_random_access_iterator<int&>>);

static_assert(
    std::convertible_to<std::vector<int>::iterator, neo::any_random_access_iterator<const int&>>);

TEST_CASE("Erased iterator with sentinel") {
    std::string     s = "foo";
    neo::utf8_range chars{s};

    neo::any_input_iterator<int>   it2 = s.cbegin();
    neo::any_input_iterator<int>   it3 = it2;
    neo::any_input_iterator<char&> c   = s.begin();

    // Convert from an iter of one type to another:
    neo::any_input_iterator<int> i4 = it2;

    static_assert(std::input_iterator<neo::any_input_iterator<int>>);
}

TEST_CASE("Created an any_input_iterator") {
    std::vector<int> vec = {1, 2, 3, 4};

    neo::any_iterator it{vec.cbegin()};
    neo::any_sentinel end{vec.cend()};

    neo::any_bidirectional_iterator<int>        rit  = it;
    neo::any_bidirectional_iterator<const int&> rit2 = it;
    neo::any_forward_iterator<const int&>       rit3 = it;
    neo::any_forward_iterator<const int>        rit4 = it;
    neo::any_forward_iterator<int>              rit5 = it;

    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    CHECK(it != end);
    ++it;
    ++it;
    CHECK(it != end);
    ++it;
    CHECK(it == end);

    neo::any_sentinel end2 = end;
    CHECK(it == end2);
    CHECK(it == it);
}
