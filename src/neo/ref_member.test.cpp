#include "./ref_member.hpp"

#include "./unref.hpp"

#include <catch2/catch.hpp>

struct my_thing {
    neo::wrap_ref_member_t<int&> v;
};

TEST_CASE("Simple usage") {
    int      value = 132;
    my_thing m{value};
    neo::unref(m.v)        = 13;
    neo::ref_member<int> r = m.v;
    CHECK(value == 13);
    neo::unref(r) = 4;
    CHECK(value == 4);
}
