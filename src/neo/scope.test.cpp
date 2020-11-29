#include "./scope.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Create a scope guard") {
    int value = 0;
    {
        neo_defer {
            // Set the value
            value = 44;
        };
        neo_defer {
            // This one is not visible, because it executes before the other
            value = 5;
        };
    }
    CHECK(value == 44);
}
