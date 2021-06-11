#include <neo/out.hpp>
#include <neo/repr.hpp>

#include <catch2/catch.hpp>

void put_five(neo::output<int> o) { o.put(5); }
void put_opt_seven(neo::optional_output<int> o) { o.put(7); }

TEST_CASE("output") {
    int val;
    put_five(neo::into(val));
    CHECK(val == 5);
    put_opt_seven(neo::into(val));
    CHECK(val == 7);

    put_opt_seven(std::nullopt);  // Nothing happens

    auto test_out = neo::into(val);
    CHECK(neo::repr(test_out).string() == "neo::output{->7:int32}");

    neo::optional_output<int> test_opt_out = std::nullopt;
    CHECK(neo::repr(test_opt_out).string() == "neo::optional_output<int32>{nullopt}");
    test_opt_out = neo::into(test_out);
    CHECK(neo::repr(test_opt_out).string() == "neo::optional_output<int32>{->7}");
}
