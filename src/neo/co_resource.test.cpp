#if __cpp_impl_coroutine

#include "./co_resource.hpp"
#include "./coroutine.hpp"

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

#if !NEO_IsEnabled(NEO_GCC_Broken_Coroutines)
enum rc_state {
    init,
    in_coro,
    about_to_convert,
    converted,
    constructed,
    used_resource,
    destroyed,
    coro_done,
};

struct rc_type {
    rc_state& st;

    rc_type(rc_state& s)
        : st{s} {
        assert(st == converted);
        st = constructed;
    }

    ~rc_type() {
        assert(st == used_resource);
        st = destroyed;
    }
};

struct rc_convert {
    rc_state& st;

    explicit rc_convert(rc_state& s)
        : st{s} {
        assert(st == in_coro);
        st = about_to_convert;
    }

    operator rc_type() {
        assert(st == about_to_convert);
        st = converted;
        return rc_type{st};
    }
};

neo::co_resource<rc_type> get_resource(rc_state& st) {
    assert(st == init);
    st = in_coro;
    co_yield rc_convert{st};
    assert(st == destroyed);
    st = coro_done;
}

TEST_CASE("co_rc lifetimes") {
    rc_state state = init;
    {
        auto res = get_resource(state);
        assert(state == constructed);
        state = used_resource;
    }
    assert(state == coro_done);
}

struct stuff {
    std::string a;
    std::string b;
};

neo::co_resource<stuff> get_stuff() { co_yield {"foo", "bar"}; }

TEST_CASE("Use some stuff") {
    auto s = get_stuff();
    CHECK(s->a == "foo");
    CHECK(s->b == "bar");
}
#endif

#endif
