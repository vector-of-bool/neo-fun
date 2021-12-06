#include "./any_iterator.hpp"

#include "./range_archetypes.hpp"
#include "./test_concept.hpp"
#include "./utf8.hpp"

#include <deque>
#include <forward_list>
#include <list>

#include <catch2/catch.hpp>

NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int>>);
NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<int&>>);
NEO_TEST_CONCEPT(neo::input_iterator<neo::any_input_iterator<const int&>>);
NEO_TEST_CONCEPT(neo::sentinel_for<neo::any_sentinel, neo::any_input_iterator<int>>);

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

template <typename R>
auto make_any_iter(R&& r) {
    return neo::any_iterator{std::ranges::begin(r)};
}

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
    using iter_type = std::ranges::iterator_t<container&>;

    using container_tag = neo::iter_concept_t<iter_type>;
    using ref_type      = std::ranges::range_reference_t<container&>;

    static_assert(std::convertible_to<iter_type, neo::any_iterator<ref_type, container_tag>>);

    if constexpr (std::random_access_iterator<iter_type>) {
        static_assert(std::convertible_to<iter_type, neo::any_random_access_iterator<ref_type>>);
    } else {
        static_assert(!std::convertible_to<iter_type, neo::any_random_access_iterator<ref_type>>);
    }

    if constexpr (std::bidirectional_iterator<iter_type>) {
        static_assert(std::convertible_to<iter_type, neo::any_bidirectional_iterator<ref_type>>);
    } else {
        static_assert(!std::convertible_to<iter_type, neo::any_bidirectional_iterator<ref_type>>);
    }

    if constexpr (std::forward_iterator<iter_type>) {
        static_assert(std::convertible_to<iter_type, neo::any_forward_iterator<ref_type>>);
        static_assert(std::convertible_to<iter_type, neo::any_forward_iterator<ref_type>>);
    } else {
        static_assert(!std::convertible_to<iter_type, neo::any_forward_iterator<ref_type>>);
    }

    if constexpr (std::input_iterator<iter_type>) {
        static_assert(std::convertible_to<iter_type, neo::any_input_iterator<ref_type>>);
    } else {
        static_assert(!std::convertible_to<iter_type, neo::any_input_iterator<ref_type>>);
    }
}

// clang-format off
template <bool C, typename From, typename To>
concept iter_converts =
       C == std::convertible_to<neo::arch::input_iterator<From>,
                                neo::any_input_iterator<To>>
    && C == std::convertible_to<neo::arch::forward_iterator<From>,
                                neo::any_input_iterator<To>>
    && C == std::convertible_to<neo::arch::bidirectional_iterator<From>,
                                neo::any_input_iterator<To>>
    && C == std::convertible_to<neo::arch::random_access_iterator<From>,
                                neo::any_input_iterator<To>>
    && C == std::convertible_to<neo::arch::forward_iterator<From>,
                                neo::any_forward_iterator<To>>
    && C == std::convertible_to<neo::arch::bidirectional_iterator<From>,
                                neo::any_forward_iterator<To>>
    && C == std::convertible_to<neo::arch::random_access_iterator<From>,
                                neo::any_forward_iterator<To>>
    && C == std::convertible_to<neo::arch::bidirectional_iterator<From>,
                                neo::any_bidirectional_iterator<To>>
    && C == std::convertible_to<neo::arch::random_access_iterator<From>,
                                neo::any_bidirectional_iterator<To>>
    && C == std::convertible_to<neo::arch::random_access_iterator<From>,
                                neo::any_random_access_iterator<To>>;
// clang-format on

// Okay: Simple conversion
static_assert(iter_converts<true, int, int>);
static_assert(iter_converts<true, int, double>);
static_assert(iter_converts<true, double, double>);
static_assert(iter_converts<true, float, double>);
static_assert(iter_converts<true, const float, double>);

// bad: Binds reference to temp:
static_assert(iter_converts<false, int, int&>);
// bad: Binds reference to temp:
static_assert(iter_converts<false, int, const int&>);
// Okay: Adds 'const' to reference
static_assert(iter_converts<true, int&, const int&>);
// Okay: Same reference type
static_assert(iter_converts<true, const int&, const int&>);
// Bad: Requires unrelated conversion
static_assert(iter_converts<false, const int&, const double&>);
struct base {};
struct derived : base {};
// Okay: Derived-to-base
static_assert(iter_converts<true, derived, base>);
static_assert(iter_converts<true, derived&, base>);
static_assert(iter_converts<true, const derived&, base>);
static_assert(iter_converts<true, const derived&, const base&>);
static_assert(iter_converts<true, derived&, const base&>);
static_assert(iter_converts<true, derived&, base&>);
// Bad: Bind reference to temporary
static_assert(iter_converts<false, derived, const base&>);
// Bad: Drops 'const'
static_assert(iter_converts<false, const derived&, base&>);
