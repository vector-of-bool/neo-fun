#include "./storage.hpp"

#include "./archetypes.hpp"

#include <catch2/catch.hpp>

namespace arch = neo::arch;

struct trivial {
    int i;
};

struct trivial_default_constructible {
    int i;
};

struct trivial_destructible {
    int i = 0;
};

struct nontrivial {
    int i = 0;
    ~nontrivial() {}
};

struct not_default_constructible {
    int a;
    constexpr explicit not_default_constructible(int a_)
        : a(a_) {}
};

// The storage for an empty type is itself an empty type
static_assert(neo::empty_type<neo::storage_for<neo::unit>>);
static_assert(neo::empty_type<neo::storage_for<void>>);

// Storage traits:
static_assert(not neo::empty_type<neo::storage_for<trivial>>);
static_assert(not neo::trivial_type<neo::storage_for<trivial_default_constructible>>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible>&&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible>&&>);
static_assert(neo::trivially_copyable<neo::storage_for<not_default_constructible>>);

static_assert(neo::trivially_copyable<neo::storage_for<trivial_default_constructible>>);
static_assert(not neo::copyable<neo::storage_for<arch::nontrivial_copy_constructor>>);
static_assert(not neo::copyable<neo::storage_for<arch::nontrivial_copy_assignment>>);
static_assert(not neo::copyable<neo::storage_for<arch::nontrivial_move_constructor>>);
static_assert(not neo::copyable<neo::storage_for<arch::nontrivial_move_assignment>>);

static_assert(neo::trivially_copyable<neo::storage_for<trivial_default_constructible&>>);
static_assert(neo::copyable<neo::storage_for<arch::nontrivial_copy_constructor&>>);
static_assert(neo::copyable<neo::storage_for<arch::nontrivial_copy_assignment&>>);
static_assert(neo::copyable<neo::storage_for<arch::nontrivial_move_constructor&>>);
static_assert(neo::copyable<neo::storage_for<arch::nontrivial_move_assignment&>>);

static_assert(neo::trivially_movable<neo::storage_for<arch::trivially_movable>>);
static_assert(neo::trivially_movable<neo::storage_for<arch::trivially_movable>>);
static_assert(not neo::movable<neo::storage_for<arch::nontrivial_move_constructor>>);
static_assert(not neo::movable<neo::storage_for<arch::nontrivial_move_assignment>>);

static_assert(neo::trivially_movable<neo::storage_for<arch::trivially_movable&>>);
static_assert(neo::trivially_movable<neo::storage_for<arch::trivially_movable&>>);
static_assert(neo::movable<neo::storage_for<arch::nontrivial_move_constructor&>>);
static_assert(neo::movable<neo::storage_for<arch::nontrivial_move_assignment&>>);

static_assert(not neo::trivial_type<neo::storage_for<trivial_default_constructible>>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::storage_for<trivial_default_constructible>&,
                                        neo::storage_for<trivial_default_constructible>&&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::storage_for<not_default_constructible>&,
                                        neo::storage_for<not_default_constructible>&&>);
static_assert(neo::trivially_copyable<neo::storage_for<not_default_constructible>>);

constexpr auto get_some_storage(bool do_construct) {
    auto x4 = neo::storage_for<not_default_constructible>();
    if (do_construct) {
        x4.construct(42);
    }
    return x4;
}

TEST_CASE("Constexpr storage") {  //
    constexpr auto x0 [[maybe_unused]] = neo::storage_for<void>();
    constexpr auto x1 [[maybe_unused]] = neo::storage_for<int>();
    constexpr auto x2 [[maybe_unused]] = neo::storage_for<neo::unit>();
    constexpr auto x3 [[maybe_unused]] = neo::storage_for<trivial>();
    // constexpr auto x4 [[maybe_unused]] = neo::storage_for<trivial_default_constructible>();
    constexpr auto x5 [[maybe_unused]] = get_some_storage(false);
    constexpr auto x6 [[maybe_unused]] = get_some_storage(true);
    static_assert(x6.get().a == 42);
}

// static_assert(neo::empty_type<neo::nano_opt_storage<void>>);
// static_assert(neo::trivially_default_constructible<neo::nano_opt_storage<void>>);
