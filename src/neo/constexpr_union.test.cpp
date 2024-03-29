#include <neo/constexpr_union.hpp>

#include <catch2/catch.hpp>
#include <neo/testing.hpp>

#include <memory>

template <std::size_t N, typename... Ts, typename... Args>
constexpr auto make_union(Args&&... args) {
    using onion_type = neo::constexpr_union<Ts...>;
    onion_type onion;
    std::construct_at(&onion.template get<N>(), args...);
    onion.template construct<N>(args...);
    using T  = typename onion_type::template nth_type<N>;
    auto dup = onion.template get<N>();
    onion.template get<N>().~T();
    return dup;
}

TEST_CASE("Create a simple union") {
    neo::constexpr_union<int> a;
    a.get<0>() = 31;
    CHECK(a.get<0>() == 31);

    auto q = make_union<1, double, int, int>(31);
    CHECK(q == 31);

    constexpr auto Q = make_union<1, double, int, int>(31);
    STATIC_REQUIRE(Q == 31);
}

TEST_CASE("Large") {
    // clang-format off
    constexpr auto x = make_union<31, int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int,
                                      int, int, int, int, int, int, int, int, int>(42);
    // clang-format on
    STATIC_REQUIRE(x == 42);
}

struct nontrivial {
    constexpr nontrivial() {}
    constexpr ~nontrivial() {}
    int value = 31;
};

TEST_CASE("Non-trivial") {
    neo::constexpr_union<std::string, int> a;
    a.get<1>() = 31;
    CHECK(a.get<1>() == 31);
    std::construct_at(&a.get<0>(), "hey");
    CHECK(a.get<0>() == "hey");

    {
        constexpr auto n = make_union<1, int, nontrivial, std::string>();
        STATIC_REQUIRE(n.value == 31);
    }
}

neo::testing::cx_test_case ChangingActiveMember = [](auto) consteval {
    neo::constexpr_union<neo::unit, neo::unit, neo::unit> onion;
    onion.construct<1>();  // Should be constexpr-okay
};
