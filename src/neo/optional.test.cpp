#include "./optional.hpp"

#include "./repr.hpp"
#include "./test_concept.hpp"

#include <catch2/catch.hpp>

struct trivial {
    int i;
};

struct trivial_default_constructible {
    int i;
    ~trivial_default_constructible() {}
};

struct trivial_destructible {
    int i = 0;
};

struct nontrivial {
    int i = 0;
    ~nontrivial() {}
};

NEO_TEST_CONCEPT(std::is_trivially_destructible_v<neo::nano_opt_storage<trivial>>);
NEO_TEST_CONCEPT(std::is_trivially_default_constructible_v<neo::nano_opt_storage<trivial>>);

/// XXX: Bug in language/compilers sees a non-trivial destructor as invalidating
/// trivial_default_constructible. As such, these checks will fail:
// NEO_TEST_CONCEPT(std::is_trivially_default_constructible_v<trivial_default_constructible>);
// NEO_TEST_CONCEPT(std::is_trivially_default_constructible_v<
//                  neo::nano_opt_storage<trivial_default_constructible>>);
/// Refer: https://cplusplus.github.io/LWG/issue2116 and
/// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51452

NEO_TEST_CONCEPT(
    !std::is_trivially_destructible_v<neo::nano_opt_storage<trivial_default_constructible>>);

NEO_TEST_CONCEPT(!std::is_trivially_default_constructible_v<neo::nano_opt_storage<nontrivial>>);
NEO_TEST_CONCEPT(!std::is_trivially_destructible_v<neo::nano_opt_storage<nontrivial>>);

// Check that nano_opt is constexpr-able
constexpr neo::nano_opt<int> get_const_opt() {
    auto ret = neo::nano_opt<int>(std::in_place, 41);
    ++ret.get();
    return ret;
}

constexpr neo::nano_opt<int> get_const_opt_2() {
    neo::nano_opt<int> ret;
    ret = {33};
    return ret;
}

TEST_CASE("Constexpr nano_opt") {
    constexpr auto opt = get_const_opt();
    static_assert(opt.get() == 42);

    constexpr auto opt_2 = get_const_opt_2();
    CHECK(opt_2.get() == 33);

    CHECK(neo::repr(opt).string() == "[neo::nano_opt<int32> [42]]");
    CHECK(neo::repr(opt_2).string() == "[neo::nano_opt<int32> [33]]");

    neo::nano_opt<int> null;
    CHECK(!null.has_value());
    CHECK(neo::repr(null).string() == "[neo::nano_opt<int32> nullopt]");
}
