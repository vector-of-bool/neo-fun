#include "./repr.hpp"

#if __cpp_nontype_template_args < 201911L && (__GNUC__ < 10) && (__clang_major__ < 11)             \
    && (_MSC_VER < 1926)

// No class-type non-type template parameter suppoet

#else

#include <neo/fixed_string.hpp>

#include <catch2/catch.hpp>

template <neo::basic_fixed_string S>
struct inner;

template <neo::basic_fixed_string String>
struct test_nttp_string {
    using b = inner<String>;
};

template <>
struct test_nttp_string<"howdy do"> {};

TEST_CASE("Create a basic fixed string") {
    neo::basic_fixed_string fstr = "I am another fixed string";
    CHECK(fstr == fstr);
    auto r = neo::repr(fstr).string();
    CHECK(r == R"(neo::fixed_string{"I am another fixed string"})");

    constexpr neo::basic_fixed_string a   = "foo";
    constexpr neo::basic_fixed_string b   = "bar";
    constexpr auto                    two = a + b;
    CHECK(a != "false");
    static_assert(two == "foobar");
    CHECK(two == "foobar");
    CHECK((two + "baz") == "foobarbaz");
    CHECK(("baz" + two) == "bazfoobar");

    neo::basic_fixed_string wide_fstr = L"I am a wide fixed string";
    CHECK(wide_fstr == wide_fstr);
    r = neo::repr(wide_fstr).string();
    CHECK(r == R"(neo::basic_fixed_string{L"I am a wide fixed string"})");

    neo::basic_fixed_string u8_fstr = u8"I am a UTF-8 fixed string";
    CHECK(u8_fstr == u8_fstr);
    r = neo::repr(u8_fstr).string();
    CHECK(r == R"(neo::basic_fixed_string{u8"I am a UTF-8 fixed string"})");
}

TEST_CASE("Take a substring") {
    neo::basic_fixed_string f    = "Hello, world!";
    auto                    part = f.substr<0, 5>();
    CHECK(part == "Hello");
    auto part2 = f.substr<7, 6>();
    CHECK(part2 == "world!");
}

TEST_CASE("Create a string view") {
    constexpr neo::basic_fixed_string f    = "I am a string";
    auto                              view = f.string_view();
    CHECK(view == "I am a string");
    CHECK(view != "lolnope");
    CHECK_FALSE(view.empty());
}

TEST_CASE("Use a string as a template parameter") {
    test_nttp_string<"egg salad"> v [[maybe_unused]];

    neo::tstring<"I am a string"> t;

    CHECK(neo::repr(t).string() == "neo::tstring<\"I am a string\">");

    // Check comparisons
    using ts = neo::tstring<"I am a string">;
    // Create a view from an alias
    auto view1 [[maybe_unused]] = neo::make_tstring_view<ts>;

    const auto       view = neo::tstring_view_v<"Hello!">;
    std::string_view sv   = view;
    CHECK(sv == "Hello!");
    CHECK(sv == view);

    CHECK(view == "Hello!");
    CHECK(view != "Goodbye!");
    CHECK("Hello!" == view);
    CHECK("Goodbye!" != view);
    CHECK(view == view);
    CHECK(view > "A");
    CHECK(view < "Z");
    CHECK_FALSE(view < view);
}

#endif
