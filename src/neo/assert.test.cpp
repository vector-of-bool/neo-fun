#include <neo/assert.hpp>

#include <catch2/catch.hpp>

#include <iostream>

bool false_bool = false;
bool true_bool  = true;

int global_n = 40;

TEST_CASE("Test some assertions") {
    int n = global_n;
    neo_assert(invariant, n != 2, "n was two?", n);
    neo_assert_always(invariant, n == 40, "ehg", n);
    neo_assert_audit(invariant, n + 7 == 47, "ehg", n + 7);
    neo_assert(invariant, 4 == 4, "test", std::cin.rdstate());
}

TEST_CASE("Audits are never executed") {
    auto check = [] {
        FAIL("This should not execute");
        return true;
    };
    neo_assert_assume(check());
}
