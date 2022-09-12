#include "./invoke.hpp"

#include <functional>

#include <catch2/catch.hpp>

auto twice(int n) { return n * 2; }

TEST_CASE("Invoke functions") { CHECK(NEO_INVOKE(twice, 12) == 24); }

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
    CHECK(NEO_INVOKE(valptr, t) == 44);
    const auto vp2 = valptr;
    CHECK(NEO_INVOKE(vp2, t) == 44);
    CHECK(NEO_INVOKE(&thing::value, t) == 44);

    auto cm_ptr = &thing::const_method;
    CHECK(NEO_INVOKE(cm_ptr, t, 23, true) == 77);
    static_assert(!noexcept(NEO_INVOKE(cm_ptr, t, 8, false)));

    auto cnm_method = &thing::const_noexcept_method;
    CHECK(NEO_INVOKE(cnm_method, t, 8, false) == 11);
    static_assert(noexcept(NEO_INVOKE(cnm_method, t, 8, false)));

    auto nl_method = &thing::noexcept_lref_method;
    CHECK(NEO_INVOKE(nl_method, t, 8, "hi"));

    auto nr_method = &thing::noexcept_rref_method;
    CHECK(NEO_INVOKE(nr_method, std::move(t), 8, "hi"));
}

TEST_CASE("Invoke refs") {
    auto lambda = [](int a, int b) { return a + b; };

    auto lref = std::ref(lambda);
    CHECK(NEO_INVOKE(lref, 4, 1) == 5);
    CHECK(NEO_INVOKE(std::move(lref), 4, 1) == 5);
    CHECK(NEO_INVOKE(std::as_const(lref), 4, 1) == 5);

    std::unique_ptr<int> p{new int{91}};
    auto                 lambda_returning_ref = [&p]() -> std::unique_ptr<int>& { return p; };
    CHECK(*NEO_INVOKE(lambda_returning_ref) == 91);
}
