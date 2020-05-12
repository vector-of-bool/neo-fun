#include <neo/assert.hpp>

#include <catch2/catch.hpp>

bool false_bool = false;
bool true_bool  = true;

int global_n = 40;

TEST_CASE("Test some assertions") {
    int n = global_n;
    neo_assert(n != 2, "n was two?");
    neo_assert_always(n == 40, "ehg", n);
    neo_assert_audit(n + 7 == 47, "ehg", n + 7);
}

TEST_CASE("Audits are never executed") {
    auto check = [] {
        FAIL("This should not execute");
        return true;
    };
    neo_assert_assume(check(), "ye?");
}
