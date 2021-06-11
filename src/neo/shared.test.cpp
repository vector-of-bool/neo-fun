#include "./shared.hpp"

#include <catch2/catch.hpp>

using neo::repr_ostream_operator::operator<<;

struct rectangle_data {
    int width  = 0;
    int height = 0;
};

struct shared_rectangle : neo::shared_state<shared_rectangle, rectangle_data> {
    using shared_state::shared_state;
};

TEST_CASE("Create a shared state object") {
    shared_rectangle rect;
    rect->width  = 33;
    rect->height = 4;

    auto copy = rect;
    CHECK(copy->width == 33);
    rect->width = 4;
    CHECK(copy->width == 4);
    copy = rect.clone();
    CHECK(copy->width == 4);
    rect->width = 8;
    CHECK(copy->width == 4);
    CHECK(rect->width == 8);

    auto weak = rect.weak_ref();
    CHECK(weak.lock().has_value());
    rect.unshare();  // Detatch ownership
    CHECK_FALSE(weak.lock().has_value());

    auto cref = rect.const_ref();
    // cref->width = 3;  // Will not compile

    const shared_rectangle& r = cref;
    // We cannot copy using a ref-to-const
    // shared_rectangle r2 = r;  // Will not compile
    // r->width                  = 8;
    (void)r;

    auto const_weak = std::as_const(rect).weak_ref();
    auto const_ref  = *const_weak.lock();
    // const_ref->width        = 22;  // Will not compile
    auto clone_from_const   = const_ref.clone();
    clone_from_const->width = 5;
    CHECK(rect->width == 8);
    CHECK(clone_from_const->width == 5);

    neo::ref_to_const<shared_rectangle> cr = rect;

    const shared_rectangle& lang_cr = cr;
    (void)lang_cr;
}
