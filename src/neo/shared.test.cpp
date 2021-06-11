#include "./shared.hpp"

#include <catch2/catch.hpp>

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

    {
        neo::weak_ref     wr  = rect;
        neo::ref_to_const cr  = rect;
        neo::weak_ref     cwr = cr;
        static_assert(std::same_as<decltype(wr), neo::weak_ref<shared_rectangle>>);
        static_assert(std::same_as<decltype(cr), neo::ref_to_const<shared_rectangle>>);
        static_assert(std::same_as<decltype(cwr), neo::weak_ref<const shared_rectangle>>);
    }

    auto weak = neo::weak_ref(rect);
    CHECK(weak.lock().has_value());
    rect.unshare();  // Detatch ownership
    CHECK_FALSE(weak.lock().has_value());

    auto cref = neo::ref_to_const(rect);
    // cref->width = 3;  // Will not compile

    // const shared_rectangle& r = cref;
    // We cannot copy using a ref-to-const
    // shared_rectangle r2 = r;  // Will not compile
    // r->width                  = 8;

    auto const_weak = neo::weak_ref(neo::ref_to_const(rect));
    auto const_ref  = *const_weak.lock();
    CHECK(const_ref.use_count() == 3);
    const_ref.unshare();
    CHECK(const_ref.use_count() == 1);
    // const_ref->width        = 22;  // Will not compile
    auto clone_from_const   = const_ref.clone();
    clone_from_const->width = 5;
    CHECK(rect->width == 8);
    CHECK(clone_from_const->width == 5);

    /// We can bind a reference from the shared-state
    static_assert(std::convertible_to<shared_rectangle, const rectangle_data&>);
    static_assert(std::convertible_to<shared_rectangle, rectangle_data&>);
    static_assert(std::convertible_to<shared_rectangle, rectangle_data>);
    static_assert(std::convertible_to<neo::ref_to_const<shared_rectangle>, const rectangle_data&>);
    static_assert(!std::convertible_to<neo::ref_to_const<shared_rectangle>, rectangle_data&>);
    static_assert(std::convertible_to<neo::ref_to_const<shared_rectangle>,
                                      neo::weak_ref<const shared_rectangle>>);
    static_assert(std::convertible_to<shared_rectangle, neo::weak_ref<const shared_rectangle>>);
    static_assert(
        !std::convertible_to<neo::ref_to_const<shared_rectangle>, neo::weak_ref<shared_rectangle>>);
}
