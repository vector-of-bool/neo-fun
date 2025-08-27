#include "./visit.hpp"
#include "neo/testing.hpp"
#include "neo/variant.hpp"

#include <catch2/catch.hpp>

#include <neo/testing.hpp>
#include <string>
#include <variant>

#include <neo/overload.hpp>

TEST_CASE("Basic visit") {
    std::variant<int, std::string> s2;
    auto                           r = neo::visit(s2, [](auto) { return 12; });
    CHECK(r == 12);
}

neo::testing::cx_test_case NeoVariantVisit = [](auto check) consteval {
    neo::variant<int, std::string> var;
    auto                           res = neo::visit(var, [](auto) { return 42; });
    check(res == 42);
};

neo::testing::cx_test_case VisitVariantWithVoid = [](auto check) consteval {
    neo::variant<int, void> var;
    auto                    r = neo::visit(var, [](auto) { return 1729; });
    check(r == 1729);
};

#ifndef _MSC_VER  // MSVC constexpr is broken for unions

struct base {};

struct derived : base {};

neo::testing::cx_test_case CommonReference = [](auto) consteval {
    neo::variant<base, derived>        var;
    std::same_as<base&> decltype(auto) b
        = neo::visit(var, [](auto&& x) -> decltype(auto) { return x; });
    (void)b;
};

struct immobile {
    immobile()                = default;
    immobile(const immobile&) = delete;
};

neo::testing::cx_test_case VisitWithImmobileresult = [](auto) consteval {
    neo::variant<int, int> var;
    immobile               im = neo::visit(var, [](auto) -> immobile { return {}; });
    (void)im;
};

#endif
