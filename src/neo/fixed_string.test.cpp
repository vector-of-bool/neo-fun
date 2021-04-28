
#if __cpp_nontype_template_args < 201911L && (__GNUC__ < 10)
// No class-type non-type template parameter suppoet

#else

#include <neo/fixed_string.hpp>

#include <catch2/catch.hpp>

template <neo::basic_fixed_string String>
struct test_nttp_string {};

TEST_CASE("Create a basic fixed string") {
    neo::basic_fixed_string fstr = "I am another fixed string";
    CHECK(fstr == fstr);

    constexpr neo::basic_fixed_string a   = "foo";
    constexpr neo::basic_fixed_string b   = "bar";
    constexpr auto                    two = a + b;
    static_assert(two == "foobar");
    CHECK(two == "foobar");
}

TEST_CASE("Take a substring") {
    neo::basic_fixed_string f    = "Hello, world!";
    auto                    part = f.substr(neo::val_v<0>, neo::val_v<5>);
    CHECK(part == "Hello");
    auto part2 = f.substr(neo::val_v<7>, neo::val_v<6>);
    CHECK(part2 == "world!");
}

TEST_CASE("Create a string view") {
    constexpr neo::basic_fixed_string f    = "I am a string";
    auto                              view = f.view();
    CHECK(view == "I am a string");
    CHECK(view != "lolnope");
    CHECK_FALSE(view.empty());
}

#endif
