#include "./slice.hpp"

#include "./generator.hpp"
#include "./range_archetypes.hpp"

#include <istream>

#include <catch2/catch.hpp>

TEST_CASE("Get substrings") {
    static_assert(std::same_as<neo::slice_t<std::string>, std::string>);
    static_assert(std::same_as<neo::slice_t<std::string&>, std::string>);
    static_assert(std::same_as<neo::slice_t<std::string&&>, std::string>);
    static_assert(std::same_as<neo::slice_t<std::string_view>, std::string_view>);
    std::string str    = "I am a string";
    auto        substr = neo::slice(str, 2);
    CHECK(substr == "am a string");
    substr = neo::slice(str, 0, -7);
    CHECK(substr == "I am a");
    substr = neo::slice(str, -6);
    CHECK(substr == "string");
    substr = neo::slice(str, -600);
    CHECK(substr == "I am a string");

    substr = neo::slice(str, 500);
    CHECK(substr == "");

    substr = neo::slice(str, 0, 400);
    CHECK(substr == "I am a string");

    substr = neo::slice(str, 0, -400);
    CHECK(substr == "");
}

TEST_CASE("Slice a vector") {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8};
    // Passing a param copies by default
    std::same_as<std::vector<int>> auto copy1 = neo::slice(nums);
    CHECK(nums == copy1);

    std::same_as<std::ranges::subrange<std::vector<int>::iterator>> auto view1
        = neo::slice.view(nums, 2, 6);
    CHECKED_IF(std::ranges::size(view1) == 4) {
        CHECK(view1[0] == 3);
        CHECK(view1[1] == 4);
        CHECK(view1[2] == 5);
        CHECK(view1[3] == 6);
    }
}

// static_assert(std::same_as<neo::view_slice_t<neo::arch::view<>>, neo::arch::view<>>);
static_assert(not neo::sliceable_range<neo::generator<int>>);
static_assert(not neo::sliceable_range<neo::generator<int>&>);
static_assert(not neo::sliceable_range<neo::generator<int> const&>);
// We can slice a ref_view of a generator:
static_assert(std::same_as<neo::view_slice_t<neo::generator<int>&>,
                           std::ranges::subrange<std::ranges::iterator_t<neo::generator<int>>,
                                                 std::ranges::sentinel_t<neo::generator<int>>>>);

static_assert(std::same_as<neo::slice_t<std::vector<int>>, std::vector<int>>);
static_assert(std::same_as<neo::slice_t<std::vector<int>&>, std::vector<int>>);
static_assert(std::same_as<neo::slice_t<std::vector<int> const&>, std::vector<int>>);
static_assert(std::same_as<neo::slice_t<std::vector<int>&&>, std::vector<int>>);
static_assert(
    std::same_as<neo::view_slice_t<std::string&>, std::ranges::subrange<std::string::iterator>>);
static_assert(std::same_as<neo::view_slice_t<std::string const&>,
                           std::ranges::subrange<std::string::const_iterator>>);

TEST_CASE("Slice an input range") {
    std::istream_iterator<char> first, second;
    auto                        sub = std::ranges::subrange(first, second);
    neo::slice(sub, 4);
}
