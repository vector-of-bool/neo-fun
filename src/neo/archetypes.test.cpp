#include "./archetypes.hpp"

#include <string>

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
concept can_amp_addressof = requires(T& arg) {
    &arg;
};
class derived_pathological : pathological {};

static_assert(!can_amp_addressof<pathological>);
static_assert(!can_amp_addressof<derived_pathological>);

template <typename T>
concept can_comma_operate = requires(T& arg) {
    (arg, 0);
};
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
