#include "./ct_optional.hpp"
#include "neo/attrib.hpp"
#include "neo/type_traits.hpp"

#include <catch2/catch.hpp>

struct something {
    NEO_NO_UNIQUE_ADDRESS neo::ct_optional<int, false> nothing;
};

static_assert(neo::empty_type<something>);

TEST_CASE("Create an optional") {
    neo::ct_optional<int, true> v = 12;
    CHECK(v.has_value());
    CHECK(*v == 12);
    v = 84;
    CHECK(*v == 84);

    neo::ct_optional<std::string, true> s;
    s = "I am a string";
    CHECK(*s == "I am a string");

    // std::string s1 = *std::move(s);
    // CHECK(s->empty());

    neo::ct_optional<std::unique_ptr<int>, true> uptr = std::make_unique<int>(12);
    CHECK(**uptr == 12);
    auto taken = *std::move(uptr);
    static_assert(not std::copyable<neo::ct_optional<std::unique_ptr<int>, true>>);
    static_assert(not std::copyable<neo::ct_optional<std::unique_ptr<int>, false>>);
    static_assert(not std::copy_constructible<neo::ct_optional<std::unique_ptr<int>, false>>);
    static_assert(not std::is_copy_assignable_v<neo::ct_optional<std::unique_ptr<int>, false>>);
}
