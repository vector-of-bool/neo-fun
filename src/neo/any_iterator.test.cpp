#include "./any_iterator.hpp"

#include "./test_concept.hpp"
#include "./utf8.hpp"

#include <catch2/catch.hpp>

NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int>>);
NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int&>>);
NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<const int&>>);
NEO_TEST_CONCEPT(neo::sentinel_for<neo::any_sentinel, neo::any_input_iterator<int>>);

TEST_CASE("Create an erased input_iterator") {
    std::vector<int>          vec = {1, 2, 3, 4};
    neo::erase_input_iterator it{vec.cbegin()};
    neo::erase_input_iterator end{vec.cend()};

    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    CHECK(it != end);
    CHECK(it == it);
    auto it2 = it;
    CHECK(it == it2);
    ++it;
    CHECK(it != it2);

    neo::any_input_iterator any_it = it;
}

NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int>>);

TEST_CASE("Erased iterator with sentinel") {
    std::string     s = "foo";
    neo::utf8_range chars{s};

    neo::any_input_iterator<int>   it2 = s.cbegin();
    neo::any_input_iterator<int>   it3 = it2;
    neo::any_input_iterator<char&> c   = s.begin();

    // Convert from an iter of one type to another:
    neo::any_input_iterator<int> i4 = c;

    static_assert(std::input_iterator<neo::any_input_iterator<int>>);
}

TEST_CASE("Created an any_input_iterator") {
    std::vector<int> vec = {1, 2, 3, 4};

    neo::any_input_iterator it{vec.cbegin()};
    neo::any_input_iterator end{vec.cend()};

    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    CHECK(it != end);
    CHECK(it == it);
    auto it2 = it;
    CHECK(it == it2);
    ++it;
    CHECK(it != it2);
    CHECK_FALSE(it == vec.cend());
    CHECK(it != end);
}
