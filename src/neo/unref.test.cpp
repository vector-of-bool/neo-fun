#include "./unref.hpp"
#include "./fwd.hpp"

#include "./ref_member.hpp"

#include <memory>

#include <catch2/catch.hpp>

template <typename T>
class with_member {
    neo::wrap_ref_member_t<T> _member;

public:
    with_member(T&& arg)
        : _member(NEO_FWD(arg)) {}

    NEO_DECL_UNREF_GETTER(member, _member);
};

template <typename T>
with_member(T&&) -> with_member<T>;

TEST_CASE("Wrap and unwrap some members") {
    with_member integer{89};
    CHECK(integer.member() == 89);
    integer.member() = 12;
    CHECK(integer.member() == 12);

    int         lval = 12;
    with_member integer2{lval};
    CHECK(integer2.member() == 12);
    integer2.member() = 42;
    CHECK(integer2.member() == 42);
    CHECK(lval == 42);

    auto        iptr = std::make_unique<int>(9);
    with_member i3{std::move(iptr)};
    CHECK(*i3.member() == 9);
    CHECK(iptr == nullptr);
    *i3.member() = 4;
    CHECK(*i3.member() == 4);

    // Does not compile: Tries to copy the unique_ptr:
    // auto copied = i3.member();
    // Okay: Moves-from the member
    auto moved = std::move(i3).member();
}
