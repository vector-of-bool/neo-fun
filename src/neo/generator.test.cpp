#include "./generator.hpp"

#include "./ranges.hpp"

#include <catch2/catch.hpp>

neo::generator<int> get_integers() {
    for (int i = 0;; ++i) {
        co_yield i;
    }
}

TEST_CASE("Generate some values") {
    auto gen = get_integers();
    (void)gen;

    auto it = gen.begin();
    REQUIRE(it != gen.end());
    CHECK(*it == 0);
    ++it;
    REQUIRE(it != gen.end());
    CHECK(*it == 1);

    static_assert(std::input_iterator<std::ranges::iterator_t<neo::generator<int>>>);
    static_assert(std::ranges::input_range<neo::generator<int>>);
}

neo::generator<std::string> get_strings() {
    co_yield "foo";
    co_yield "bar";
    co_yield "baz";
}

TEST_CASE("Generator some strings") {
    auto gen = get_strings();
    static_assert(std::ranges::input_range<neo::generator<std::string>>);
    auto vec = neo::to_vector(gen);
    CHECK(vec == std::vector<std::string>({"foo", "bar", "baz"}));
}

neo::generator<std::string&> yield_ref() {
    std::string s = "meow";
    co_yield s;
    CHECK(s == "bark");
}

TEST_CASE("Yield a reference") {
    auto gen = yield_ref();
    auto it  = gen.begin();
    CHECK(*it == "meow");
    *it = "bark";
    ++it;
    CHECK(it == gen.end());
}

neo::generator<int> throws_something() {
    co_yield 44;
    throw std::runtime_error("oof");
    co_yield 9;
}

TEST_CASE("Throw while generating") {
    auto g  = throws_something();
    auto it = g.begin();
    CHECK(*it == 44);
    CHECK_THROWS_AS(++it, std::runtime_error);
    CHECK(it == g.end());
}

neo::generator<std::unique_ptr<int>> gen_uptrs() {
    co_yield std::make_unique<int>(52);
    co_yield std::make_unique<int>(59);
    auto uptr = std::make_unique<int>(3412);

    // co_yield uptr;  // ! <- Must not compile
}

TEST_CASE("Yield move-only objects") {
    auto g   = gen_uptrs();
    auto it  = g.begin();
    auto ptr = *it;
    REQUIRE(ptr);
    CHECK(*ptr == 52);
    ++it;
    ptr = *it;
    CHECK(*ptr == 59);
    ++it;
    CHECK(it == g.end());
}
