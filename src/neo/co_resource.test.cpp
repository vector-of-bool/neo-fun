#if __cpp_impl_coroutine

#include "./co_resource.hpp"

#include <catch2/catch.hpp>

neo::co_resource<int> with_integer(int& out) {
    out = 42;
    co_yield 12;
    out = 1729;
}

neo::co_resource<int&> with_iref(int& out) {
    int inner = 64;
    out       = 42;
    co_yield inner;
    out = 1729;
    assert(inner == 7);
}

neo::co_resource<void> with_void(int& out) {
    out = 42;
    co_yield 0;
    out = 1729;
}

TEST_CASE("Do some stuff") {
    int v = 0;
    {
        auto s = with_integer(v);
        CHECK(v == 42);
    }
    CHECK(v == 1729);
}

TEST_CASE("Yield a reference") {
    int v = 0;
    {
        auto s = with_iref(v);
        CHECK(v == 42);
        CHECK(*s == 64);
        *s = 7;
    }
    CHECK(v == 1729);
}

TEST_CASE("Yield void") {
    int v = 0;
    {
        auto s = with_void(v);
        CHECK(v == 42);
    }
    CHECK(v == 1729);
}

neo::co_resource<void> with_throws() {
    throw std::runtime_error("oof");
    co_return;
}

TEST_CASE("Throws in setup") { CHECK_THROWS_AS(with_throws(), std::runtime_error); }

#endif
