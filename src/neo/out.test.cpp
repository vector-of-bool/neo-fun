#include <neo/out.hpp>

#include <catch2/catch.hpp>

void put_five(neo::output<int> o) { o.put(5); }
void put_opt_seven(neo::optional_output<int> o) { o.put(7); }

TEST_CASE("output") {
    int val;
    put_five(neo::into(val));
    CHECK(val == 5);
    put_opt_seven(neo::into(val));
    CHECK(val == 7);

    put_opt_seven(std::nullopt); // Nothing happens
}
