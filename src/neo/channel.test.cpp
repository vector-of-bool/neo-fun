#include "./channel.hpp"

#include <catch2/catch.hpp>

using namespace neo;

channel<int, int> int_chan() {
    decltype(auto) r = co_yield 21;
    CHECK(r == 42);
    co_return;
}

TEST_CASE("Simple channel") {
    auto ch = int_chan();
    CHECK(ch.current() == 21);
    int i = 42;
    ch.send(i);
    CHECK(ch.done());
    ch.return_value();
}

channel<void, void, int> ret_int_chan() {
    co_yield 0;
    // Should not compile:
    // auto x = co_yield 0;
    co_return 42;
}

TEST_CASE("Non-void return") {
    auto ch = ret_int_chan();
    REQUIRE_FALSE(ch.done());
    ch.send();
    CHECK(ch.done());
    CHECK(ch.return_value() == 42);
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
    REQUIRE_FALSE(ch.done());
    ch.send();
    CHECK(ch.done());
    CHECK(ch.return_value() == 42);
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
    REQUIRE_FALSE(ch.done());
    CHECK(ch.current() == 71);
    ch.send();
    CHECK(ch.done());
    CHECK(ch.return_value() == 42);
}

channel<int> inner() { co_yield 31; }
channel<int> outer() {
    co_yield global_int;
    co_yield from_channel(inner());
}

TEST_CASE("Nested yield") {
    auto ch = outer();
    REQUIRE_FALSE(ch.done());
    global_int = 94;
    CHECK(ch.current() == 94);
    ch.send();
    REQUIRE_FALSE(ch.done());
    CHECK(ch.current() == 31);
    ch.send();
    CHECK(ch.done());
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
    REQUIRE_FALSE(ch.done());
    global_int = 94;
    CHECK(ch.current() == 94);
    ch.send();
    REQUIRE_FALSE(ch.done());
    CHECK(ch.current() == 31);
    REQUIRE_FALSE(ch.done());
    ch.send();
    CHECK(ch.current() == 5);
    REQUIRE_FALSE(ch.done());
    ch.send();
    CHECK(ch.done());
}
