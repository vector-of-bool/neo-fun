#include <neo/ufmt.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Format a simple string") {
    CHECK(neo::ufmt("Just a string") == "Just a string");
    CHECK(neo::ufmt("Number is {}", 34) == "Number is 34");
    CHECK(neo::ufmt("{} + {} = {}", 1, 2, 3) == "1 + 2 = 3");

    CHECK(neo::ufmt("Hello, {}!", std::string_view("world")) == "Hello, world!");
    CHECK(neo::ufmt("Hello, {}!", std::string("user")) == "Hello, user!");
    CHECK(neo::ufmt("2 + 2 = 5 ? {}", false) == "2 + 2 = 5 ? false");
    CHECK(neo::ufmt("2 + 2 = 4 ? {}", true) == "2 + 2 = 4 ? true");
    CHECK(neo::ufmt("'Y' is {}", 'Y') == "'Y' is Y");
}

struct my_item {
    int i = 1;

    friend std::string to_string(my_item item) noexcept { return neo::to_string(item.i); }
};

TEST_CASE("Custom format my_item") {
    my_item item;
    item.i = 52;
    CHECK(neo::ufmt("{}", item) == "52");
}
