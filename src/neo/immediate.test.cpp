#include "./immediate.hpp"

#include "./await.hpp"

#include <catch2/catch.hpp>

neo::immediate<int> immediate_int() { return neo::immediate{12}; }

neo::immediate<void> coro_void(bool& set_true) {
    // Try to await on a nested immediate
    assert(co_await immediate_int() == 12);
    set_true = true;
}

neo::immediate<int> coro_int() {
    auto a = co_await immediate_int();
    auto b = co_await immediate_int();
    co_return a + b;
}

neo::immediate<int> await_void() {
    bool b = false;
    co_await coro_void(b);
    CHECK(b);
    co_return 33;
}

TEST_CASE("Simple immediate value") {
    auto i = immediate_int();
    CHECK(*i == 12);
}

TEST_CASE("Use immediate<void>") {
    bool did_run = false;
    coro_void(did_run);
    CHECK(did_run);
}

TEST_CASE("Use immediate<int>") {
    int v = *coro_int();
    CHECK(v == 24);
}

TEST_CASE("Create an immediate directly") {
    auto imm = neo::immediate{12};
    CHECK(*imm == 12);
}

TEST_CASE("Create an immediate of void") { auto imm = neo::immediate{}; }

neo::immediate<std::unique_ptr<int>> get_uptr() {
    auto p = std::make_unique<int>(3312);
    co_return p;
}

neo::immediate<void> await_uptr() {
    auto p = co_await get_uptr();
    CHECK(*p == 3312);
}

TEST_CASE("Await on a move-only") { await_uptr(); }

static_assert(neo::awaitable<neo::immediate<int>>);
static_assert(std::same_as<int&&, neo::await_result_t<neo::immediate<int>>>);
static_assert(std::same_as<int&, neo::await_result_t<neo::immediate<int>&>>);
static_assert(std::same_as<const int&, neo::await_result_t<neo::immediate<int> const&>>);
static_assert(std::same_as<void, neo::await_result_t<neo::immediate<void>>>);

using rebound = neo::rebind_awaitable_t<neo::immediate<int>, double>;
static_assert(std::same_as<rebound, neo::immediate<double>>);
