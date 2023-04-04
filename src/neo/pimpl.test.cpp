#include "./pimpl.hpp"

#include <catch2/catch.hpp>

class pimpl_haver {
    struct impl;

    neo::pimpl<impl> _impl;
};

pimpl_haver get_item();

TEST_CASE("Access a pimpl object") {
    if constexpr (false) {
        auto i = get_item();
    }
}

struct real_pimpl {
    struct impl {
        int value = 0;
    };

    neo::pimpl<impl> p;
};

TEST_CASE("Use a pimpl object") {
    real_pimpl p1;
    CHECK(p1.p->value == 0);
    p1 = p1;
    CHECK(p1.p->value == 0);
    real_pimpl p2;
    p2.p->value = 42;
    CHECK(p1.p->value == 0);
    p1 = p2;
    CHECK(p1.p->value == 42);
    CHECK(p2.p->value == 42);
    p1.p->value = 6;
    CHECK(p2.p->value == 42);

    p2 = real_pimpl();
    CHECK(p1.p->value == 6);

    p1 = std::move(p2);
    CHECK(p1.p->value == 0);
    p1.p->value = 9;
    p2 = p1;
    CHECK(p2.p->value == 9);
    p1 = std::move(p2);
    p2 = std::move(p1);
    CHECK(p2.p->value == 9);
}
