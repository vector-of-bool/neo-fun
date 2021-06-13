#include "./zstring_view.hpp"

#include <neo/repr.hpp>

#include <catch2/catch.hpp>

template <typename T>
concept can_remove_suffix = requires(T value) {
    value.remove_suffix(4);
};

static_assert(can_remove_suffix<std::string_view>);
static_assert(!can_remove_suffix<neo::zstring_view>);

static_assert(std::convertible_to<std::string, neo::zstring_view>);
static_assert(!std::convertible_to<std::string_view, neo::zstring_view>);
static_assert(std::convertible_to<neo::zstring_view, std::string_view>);

TEST_CASE("Create a zstring") {
    std::string_view  str = "Hello!";
    neo::zstring_view s1{str};
    CHECK(s1 == "Hello!");

    neo::zstring_view s2 = s1;
    CHECK(s2 == s1);

    CHECK(neo::repr(s2).string() == "neo::zstring_view{\"Hello!\"}");

    std::string       s  = "I am an allocated string";
    neo::zstring_view zs = s;
    CHECK(zs == s);

    s1 = s;
    CHECK(s1 == s);

    CHECK(s1.front() == s.front());
    CHECK(s1.back() == s.back());
    CHECK(s1.data() == s.data());
}
