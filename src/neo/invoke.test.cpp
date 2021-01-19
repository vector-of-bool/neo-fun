#include "./invoke.hpp"

#include <catch2/catch.hpp>

auto twice(int n) { return n * 2; }

TEST_CASE("Invoke functions") { CHECK(neo::invoke(twice, 12) == 24); }

struct thing {
    int value;

    auto const_method(int, bool) const { return 77; }
    auto const_noexcept_method(int, bool) const noexcept { return 11; }
    auto noexcept_method(int, std::string) noexcept { return 7621; }
    auto noexcept_lref_method(int, std::string) & noexcept { return 7621; }
    auto noexcept_rref_method(int, std::string) && { return 7621; }
};

TEST_CASE("Invoke members") {
    auto  valptr = &thing::value;
    thing t;
    t.value = 44;
    CHECK(neo::invoke(valptr, t) == 44);

    auto cm_ptr = &thing::const_method;
    CHECK(neo::invoke(cm_ptr, t, 23, true) == 77);

    auto cnm_method = &thing::const_noexcept_method;
    CHECK(neo::invoke(cnm_method, t, 8, false) == 11);

    auto nl_method = &thing::noexcept_lref_method;
    CHECK(neo::invoke(nl_method, t, 8, "hi"));

    auto nr_method = &thing::noexcept_rref_method;
    CHECK(neo::invoke(nr_method, std::move(t), 8, "hi"));
}
