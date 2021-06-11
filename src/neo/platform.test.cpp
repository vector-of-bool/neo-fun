#include <neo/platform.hpp>
#include <neo/repr.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Check platform info ID representations") {
    CHECK(neo::repr(neo::compiler_id_t::gnu).string() == "neo::compiler_id_t::gnu");
    CHECK(neo::repr_value(neo::compiler_id_t::gnu).string() == "gnu");
    CHECK(neo::repr_value(neo::operating_system_t::windows).string() == "windows");
}
