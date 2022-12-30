#include "./tuple.hpp"

#include <catch2/catch.hpp>

#include <memory>
#include <string>
#include <tuple>

using neo::tuple;
using neo::tuple_get_t;

static_assert(neo::explicit_convertible_to<tuple<int>, tuple<double>>);
static_assert(neo::convertible_to<tuple<int>, tuple<double>>);

template <typename... Ts>
static void semiregular_assertions() {
    using tuple = tuple<Ts...>;
    static_assert(std::semiregular<tuple>);
}

template <typename... Ts>
static void regular_assertions() {
    using tuple = tuple<Ts...>;
    semiregular_assertions<Ts...>();

    static_assert(std::regular<tuple>);
    static_assert(std::totally_ordered<tuple>);
}

TEST_CASE("Empty tuple") {
    tuple<> empty;
    auto    dup = empty;
    empty       = dup;
    regular_assertions<>();
}

struct not_equality_comparable {};

TEST_CASE("Equality and compare") {
    static_assert(not neo::equality_comparable<tuple<not_equality_comparable>>);
    static_assert(neo::equality_comparable<tuple<int>>);
    static_assert(neo::equality_comparable<tuple<int, int>>);
    static_assert(neo::equality_comparable<tuple<std::string, int>>);
    static_assert(
        neo::equality_comparable_with<tuple<std::string_view, int>, tuple<const char*, int>>);

    static_assert(not neo::equality_comparable_with<tuple<std::string_view, int>,
                                                    tuple<const char*, not_equality_comparable>>);

    neo::tuple t1{1, 2};
    neo::tuple t2{2, 1};
    CHECK(t1 != t2);
    CHECK(t1 < t2);
    CHECK(t2 > t1);
}

TEST_CASE("Const-ness") {
    static_assert(std::assignable_from<tuple_get_t<tuple<int>&, 0>, int>);
    static_assert(not std::assignable_from<tuple_get_t<const tuple<int>&, 0>, int>);
    using moveonly = std::unique_ptr<int>;
    static_assert(std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly>&&, 0>>);
    static_assert(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly>&, 0>>);
    static_assert(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly> const&, 0>>);
    static_assert(not std::assignable_from<moveonly&, tuple_get_t<tuple<moveonly> const&&, 0>>);
}

TEST_CASE("Simple usage") {
    int        a = 1;
    neo::tuple i{a};
}

TEST_CASE("Get<> on a std::tuple") {
    std::tuple<int, std::string>      t;
    std::same_as<int&> decltype(auto) n [[maybe_unused]] = neo::get_nth<0>(t);

    neo::tuple<int, std::string>                    t2;
    std::same_as<std::string&> decltype(auto)       s [[maybe_unused]] = neo::get_nth<1>(t);
    std::same_as<const std::string&> decltype(auto) s2 [[maybe_unused]]
    = neo::get_nth<1>(std::as_const(t2));
}

static_assert(
    neo::weak_same_as<neo::common_type_t<neo::tuple<double, int>, neo::tuple<int, double>>,
                      neo::tuple<double, double>>);

static_assert(not neo::has_common_type<neo::tuple<double, int>, neo::tuple<int, std::string>>);
static_assert(neo::has_common_type<neo::tuple<double, const char*>, neo::tuple<int, std::string>>);

static_assert(neo::weak_same_as<
              neo::common_reference_t<neo::tuple<int&, double>, neo::tuple<int const&, double>>,
              neo::tuple<int const&, double>>);
