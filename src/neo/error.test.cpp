#include <neo/error.hpp>

#include <catch2/catch.hpp>

void set_err(std::error_code& out, std::error_code ec) { out = ec; }

TEST_CASE("Throw errors") {
    neo::error_code_thrower err;
    set_err(err, {});
    CHECK_NOTHROW(err("test"));
    set_err(err, make_error_code(std::errc::invalid_argument));
    CHECK_THROWS_AS(err("test"), std::system_error);

    using namespace neo::error_literals;
    CHECK_NOTHROW(set_err("No error"_ec_throw, {}));
    CHECK_THROWS_AS(set_err("No error"_ec_throw, make_error_code(std::errc::timed_out)),
                    std::system_error);
}
