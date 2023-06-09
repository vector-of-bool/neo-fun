#include "./channel.hpp"

#include <catch2/catch.hpp>

#include <neo/config-pp.hpp>
#include <neo/coroutine.hpp>

#include <ranges>
#include <stdexcept>

using namespace neo;

channel<int, int> int_chan() {
    decltype(auto) r = co_yield 21;
    CHECK(r == 42);
    co_return;
}

TEST_CASE("Simple channel") {
    auto ch = int_chan();
    auto io = ch.open();
    CHECK(io.current() == 21);
    int i = 42;
    io.send(i);
    CHECK(io.done());
    io.return_value();
}

channel<void, void, int> ret_int_chan() {
    co_yield 0;
    // Should not compile:
    // auto x = co_yield 0;
    co_return 42;
}

TEST_CASE("Non-void return") {
    auto ch = ret_int_chan();
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    io.send();
    CHECK(io.done());
    CHECK(io.return_value() == 42);
}

static int                global_int = 0;
channel<void, void, int&> ret_int2_chan() {
    co_yield 0;
    // Should not compile:
    // co_return 51;
    global_int = 42;
    co_return global_int;
}

TEST_CASE("Reference return") {
    auto ch = ret_int2_chan();
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    io.send();
    CHECK(io.done());
    CHECK(io.return_value() == 42);
}

channel<int&, void, int&> ret_int3_chan() {
    global_int = 71;
    co_yield global_int;
    // Should not compile:
    // co_yield 0;
    global_int = 42;
    co_return global_int;
}

TEST_CASE("Reference yield") {
    auto ch = ret_int3_chan();
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    CHECK(io.current() == 71);
    io.send();
    CHECK(io.done());
    CHECK(io.return_value() == 42);
}

channel<int> inner() { co_yield 31; }
channel<int> outer() {
    co_yield global_int;
    co_yield from_channel(inner());
}

TEST_CASE("Nested yield") {
    global_int = 94;
    auto ch    = outer();
    auto io    = ch.open();
    REQUIRE_FALSE(io.done());
    CHECK(io.current() == 94);
    io.send();
    REQUIRE_FALSE(io.done());
    CHECK(io.current() == 31);
    io.send();
    CHECK(io.done());
}

channel<int, void, std::string> inner_with_str() {
    co_yield 31;
    co_return "Howdy";
}
channel<int> outer2() {
    co_yield global_int;
    auto s = co_yield from_channel(inner_with_str());
    co_yield static_cast<int>(s.length());
}

TEST_CASE("Nested yield with return") {
    auto ch = outer2();
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    global_int = 94;
    CHECK(io.current() == 94);
    io.send();
    REQUIRE_FALSE(io.done());
    CHECK(io.current() == 31);
    REQUIRE_FALSE(io.done());
    io.send();
    CHECK(io.current() == 5);
    REQUIRE_FALSE(io.done());
    io.send();
    CHECK(io.done());
}

channel<std::string, std::string, std::string> my_chan() {
    decltype(auto) s = co_yield "Hello";
    static_assert(neo::same_as<decltype(s), std::string>);
    CHECK(s == "world!");
    co_return "the end";
}

TEST_CASE("Nontrivial objects") {
    auto ch = my_chan();
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    CHECK(io.current() == "Hello");
    CHECK(io.current() == "Hello");  // Check again to be sure it isn't moved-from
    io.send("world!");
    REQUIRE(io.done());
    CHECK(io.return_value() == "the end");
    CHECK(io.return_value() == "the end");
}

channel<std::string, std::string, std::string> throwing_channel(int) {
    const auto s = co_yield "Howdy!";
    throw std::runtime_error("fail");
    co_return "hi";
}

TEST_CASE("Throw an error") {
    auto ch = throwing_channel(3);
    auto io = ch.open();
    REQUIRE_FALSE(io.done());
    CHECK_THROWS_AS(io.send("boo"), std::runtime_error);
    CHECK(io.done());
}

channel<void, void, std::string> throwing_channel2(int) {
    throw std::runtime_error("fail");
    co_return "hi";
}

TEST_CASE("Throw an error 2") {
    auto ch = throwing_channel2(3);
    CHECK_THROWS_AS(ch.open(), std::runtime_error);
}

channel<> maybe_throw(bool do_throw) {
    if (do_throw) {
        throw std::runtime_error("no");
    }
    co_return;
}
channel<> inner_loop() {
    std::vector<int> vec(1024);
    co_yield ~maybe_throw(false);
    co_yield ~maybe_throw(true);
    co_yield ~maybe_throw(false);
}

channel<> outer_loop() {
    auto ch = inner_loop();
    auto io = ch.open();
    io.send();
    io.send();
    co_return;
}

TEST_CASE("Throw during suspend") {
    if (not NEO_IsEnabled(NEO_Clang_Broken_Coroutines)) {
        auto ch = outer_loop();
        CHECK_THROWS_AS(ch.open(), std::runtime_error);
    }
}

channel<int, int> deep_channel(int layers) {
    if (layers == 0) {
        int got = co_yield 42;
        CHECK(got == 1729);
    } else {
        co_yield layers;
        co_yield ~deep_channel(layers - 1);
        co_yield layers;
    }
}

TEST_CASE("Deep resume") {
    const int nlayers = 4096;
    auto      ch      = deep_channel(nlayers);
    auto      io      = ch.open();
    for (int n = nlayers; n > 0; --n) {
        REQUIRE_FALSE(io.done());
        CHECK(io.current() == n);
        io.send(-12);
    }
    CHECK(io.current() == 42);
    io.send(1729);
    for (int n = 1; n <= nlayers; ++n) {
        REQUIRE_FALSE(io.done());
        CHECK(io.current() == n);
        io.send(-2);
    }
    CHECK(io.done());
}

channel<void, void, int> deep_return(int n, int top) {
    if (n == 0) {
        co_return top;
    }
    co_return co_yield ~deep_return(n - 1, top);
}

TEST_CASE("Deep return") {
    auto ch = deep_return(2, 52);
    auto io = ch.open();
    CHECK(io.return_value() == 52);
}

neo::channel<int&> integer_mrefs() {
    int n = 0;
    co_yield n;
}

neo::channel<int, void, std::string> generate_fib() {
    int a = 0;
    int b = 1;
    for (;;) {
        co_yield a;
        int sum = a + b;
        a       = b;
        b       = sum;
    }
}

static_assert(std::input_iterator<std::ranges::iterator_t<neo::channel<int>>>);
static_assert(std::ranges::range<neo::channel<int>>);
static_assert(std::ranges::input_range<neo::channel<int>>);
static_assert(std::ranges::output_range<neo::channel<void, int>, int>);
static_assert(not std::ranges::input_range<neo::channel<int, int>>);

TEST_CASE("As range") {
    auto             ch = generate_fib();
    std::vector<int> nums;
    std::ranges::copy(std::views::take(ch, 40), std::back_inserter(nums));
    CHECK(nums[39] == 63245986);
}

template <typename T>
neo::channel<double, T&&> push_backer(std::vector<T>& into) {
    while (1) {
        auto t = co_yield 42.0;
        into.push_back(NEO_FWD(t));
    }
}

TEST_CASE("Output range") {
    std::vector<std::string> strings;
    auto                     out = push_backer(strings);
    auto                     num_strings
        = std::views::iota(0, 30) | std::views::transform([](auto n) { return std::to_string(n); });
    std::ranges::copy(num_strings, out.begin());
    CHECK(strings.size() == 30);
}

neo::channel<int> inner_throws() {
    throw std::runtime_error("fail");
    co_return;
}

neo::channel<int> outer_catches() {
    try {
        co_yield ~inner_throws();
        FAIL("Child did not throw, but we expected it");
    } catch (std::runtime_error const&) {
        co_return;
    }
}

neo::channel<int> outer_unaware() { co_yield ~outer_catches(); }

TEST_CASE("Nested throw") {
    auto ch = outer_unaware();
    CHECK_NOTHROW(ch.open());
}