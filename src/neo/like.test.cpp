#include "./like.hpp"

#include <catch2/catch.hpp>

template <typename From, typename Basis, typename Expect>
void test() {
    static_assert(std::same_as<Expect, neo::forward_like_t<Basis, From>>);
}

void test_all() {
    test<int, double, int&&>();
    test<int, double&, int&>();
    test<int, double&&, int&&>();
    test<int, double const, int const&&>();
    test<int, double const&, int const&>();
    test<int, double const&&, int const&&>();
    test<int const, double, int const&&>();
    test<int const, double&, int const&>();
    test<int const, double&&, int const&&>();
    test<int const, double const, int const&&>();
    test<int const, double const&, int const&>();
    test<int const, double const&&, int const&&>();
    test<int&, double, int&&>();
    test<int&, double&, int&>();
    test<int&, double&&, int&&>();
    test<int&, double const, int const&&>();
    test<int&, double const&, int const&>();
    test<int&, double const&&, int const&&>();
    test<int&&, double, int&&>();
    test<int&&, double&, int&>();
    test<int&&, double&&, int&&>();
    test<int&&, double const, int const&&>();
    test<int&&, double const&, int const&>();
    test<int&&, double const&&, int const&&>();
    test<int const&, double, int const&&>();
    test<int const&, double&, int const&>();
    test<int const&, double&&, int const&&>();
    test<int const&, double const, int const&&>();
    test<int const&, double const&, int const&>();
    test<int const&, double const&&, int const&&>();
    test<int const&&, double, int const&&>();
    test<int const&&, double&, int const&>();
    test<int const&&, double&&, int const&&>();
    test<int const&&, double const, int const&&>();
    test<int const&&, double const&, int const&>();
    test<int const&&, double const&&, int const&&>();
}

// This is required in order to link in some configurations MSVC
TEST_CASE("nil") {}
