#include "./archetypes.hpp"
#include "./concepts.hpp"

#include <catch2/catch.hpp>

#include <string>

NEO_MSVC_PRAGMA(warning(disable : 4624));

// This is required in order to link in some configurations MSVC
TEST_CASE("nil") {}

using namespace neo::arch;

static_assert(!neo::destructible<indestructible>);
static_assert(!neo::constructible_from<unconstructible>);

static_assert(!neo::movable<immovable>);
static_assert(!neo::copyable<uncopyable>);
static_assert(neo::movable<uncopyable>);

static_assert(!neo::semiregular<irregular>);

static_assert(!neo::destructible<irregular> &&           //
              !neo::default_initializable<irregular> &&  //
              !neo::copyable<irregular> &&               //
              !neo::movable<irregular>);

template <typename T>
concept can_amp_addressof = requires(T& arg) { &arg; };
class derived_pathological : pathological {};

static_assert(!can_amp_addressof<pathological>);
static_assert(!can_amp_addressof<derived_pathological>);

template <typename T>
concept can_comma_operate = requires(T& arg) { (arg, 0); };
static_assert(!can_comma_operate<pathological>);
static_assert(!can_comma_operate<derived_pathological>);

static_assert(neo::destructible<destructible>);
static_assert(!neo::semiregular<destructible>);

static_assert(neo::constructible_from<constructible_from<int, char, void*>, int, char, void*>);
static_assert(neo::default_initializable<default_initializable>);
static_assert(!neo::semiregular<default_initializable>);

static_assert(neo::move_constructible<move_constructible>);
static_assert(neo::copy_constructible<copy_constructible>);
static_assert(!neo::semiregular<copy_constructible>);

static_assert(neo::convertible_to<convertible_to<int>, int>);
static_assert(!neo::convertible_to<convertible_to<char>, int>);

static_assert(neo::movable<movable>);
static_assert(neo::movable<copyable>);
static_assert(neo::copyable<copyable>);
static_assert(!neo::semiregular<copyable>);

static_assert(neo::semiregular<semiregular>);

static_assert(neo::equality_comparable<equality_comparable>);
static_assert(neo::semiregular<regular>);
static_assert(neo::regular<regular>);

static_assert(neo::assignable_from<assignable_from<int>&, int>);
static_assert(!neo::assignable_from<assignable_from<int>&, std::string>);

static_assert(neo::totally_ordered<totally_ordered>);

struct semiregular_2 : semiregular {};
struct regular_2 : regular {};

static_assert(!neo::regular<semiregular>);
static_assert(!neo::regular<semiregular_2>);
static_assert(neo::regular<regular_2>);

static_assert(neo::invocable<invocable<int, char, int*>, int, char, int*>);

static_assert(neo::trivially_default_constructible<trivially_default_constructible>);
static_assert(not neo::trivially_constructible<trivially_default_constructible,
                                               trivially_default_constructible const&>);
static_assert(not neo::trivially_constructible<trivially_default_constructible,
                                               trivially_default_constructible&&>);
static_assert(not neo::trivially_copyable<trivially_default_constructible>);
static_assert(not neo::trivially_assignable<trivially_default_constructible&,
                                            const trivially_default_constructible&>);
static_assert(not neo::trivially_assignable<trivially_default_constructible&,
                                            trivially_default_constructible&&>);
// XXX: https://cplusplus.github.io/LWG/issue2116
// static_assert(not neo::trivially_destructible<trivially_default_constructible>);

static_assert(not neo::default_initializable<trivially_copy_constructible>);
static_assert(neo::trivially_constructible<trivially_copy_constructible,
                                           trivially_copy_constructible const&>);
static_assert(not neo::trivially_assignable<trivially_copy_constructible&,
                                            trivially_copy_constructible const&>);
static_assert(
    not neo::trivially_assignable<trivially_copy_constructible&, trivially_copy_constructible&&>);
static_assert(
    not neo::trivially_constructible<trivially_copy_constructible, trivially_copy_constructible&&>);

static_assert(not neo::default_initializable<trivially_copy_assignable>);
static_assert(
    not neo::trivially_constructible<trivially_copy_assignable, trivially_copy_assignable const&>);
static_assert(
    neo::trivially_assignable<trivially_copy_assignable&, trivially_copy_assignable const&>);
static_assert(
    not neo::trivially_assignable<trivially_copy_assignable&, trivially_copy_assignable&&>);
static_assert(
    not neo::trivially_constructible<trivially_copy_assignable, trivially_copy_assignable&&>);

static_assert(neo::trivially_copyable<trivially_copyable>);
static_assert(neo::trivially_movable<trivially_copyable>);
static_assert(neo::trivially_movable<trivially_movable>);
static_assert(not neo::copyable<trivially_movable>);

static_assert(neo::trivially_destructible<trivially_destructible>);
static_assert(not neo::trivially_default_constructible<trivially_destructible>);
static_assert(not neo::trivially_copyable<trivially_destructible>);
static_assert(not neo::trivially_movable<trivially_destructible>);
