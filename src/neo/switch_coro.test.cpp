#include <neo/switch_coro.hpp>

#include <catch2/catch.hpp>

namespace {
struct my_coro {
    int _coro = 0;
    int operator()() {
        NEO_CORO_BEGIN(_coro);
        NEO_CORO_YIELD(3);
        NEO_CORO_END;
        return 4;
    }
};
}  // namespace

TEST_CASE("Simple coroutine") {
    auto co = my_coro();
    auto v  = co();
    CHECK(v == 3);
    CHECK(co() == 4);
}