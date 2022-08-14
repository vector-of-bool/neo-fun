#include "./range_archetypes.hpp"

#include <ranges>
#include <string>

using namespace neo::arch;
namespace sr = std::ranges;

static_assert(std::weakly_incrementable<weakly_incrementable>);
static_assert(std::weakly_incrementable<input_or_output_iterator<>>);
static_assert(std::input_or_output_iterator<input_or_output_iterator<>>);

static_assert(std::input_iterator<input_iterator<>>);
static_assert(std::sentinel_for<sentinel_for<input_iterator<>>, input_iterator<>>);
static_assert(std::forward_iterator<forward_iterator<>>);
static_assert(!std::forward_iterator<input_iterator<>>);
static_assert(std::bidirectional_iterator<bidirectional_iterator<>>);

static_assert(std::random_access_iterator<random_access_iterator<int&>>);
static_assert(std::contiguous_iterator<contiguous_iterator<int&>>);

static_assert(std::indirectly_writable<indirectly_writable<int>, int>);
static_assert(std::output_iterator<output_iterator<std::string>, std::string>);
static_assert(std::output_iterator<forward_iterator<std::string&>, std::string>);
static_assert(!std::output_iterator<input_iterator<std::string&>, std::string>);

static_assert(sr::range<range<>>);
static_assert(sr::borrowed_range<borrowed_range<>>);
static_assert(!sr::borrowed_range<range<>>);
static_assert(sr::sized_range<sized_range<>>);

static_assert(sr::input_range<input_range<>>);
static_assert(sr::forward_range<forward_range<>>);
static_assert(sr::bidirectional_range<bidirectional_range<>>);
static_assert(sr::random_access_range<random_access_range<>>);
static_assert(sr::contiguous_range<contiguous_range<>>);

#include <catch2/catch.hpp>

// This is required in order to link in some configurations MSVC
TEST_CASE("nil") {}
