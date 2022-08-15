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
