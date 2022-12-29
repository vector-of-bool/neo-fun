#include "./meta.hpp"

#include <concepts>

namespace meta = neo::meta;
using meta::list;
using std::same_as;

static_assert(same_as<list<int, int, int>, meta::filled_list<int, 3>>);
static_assert(same_as<list<int>, meta::filled_list<int, 1>>);
static_assert(same_as<list<>, meta::filled_list<int, 0>>);

static_assert(meta::len_v<list<int, int, int>> == 3);

static_assert(same_as<meta::list<>, meta::clear<list<void, int, double>>>);

static_assert(same_as<int, meta::head<list<int, void, double>>>);
static_assert(same_as<list<void, double>, meta::tail<list<int, void, double>>>);

static_assert(same_as<int, meta::at<list<void, double, int, char>, 2>>);
static_assert(same_as<list<int, double>, meta::remove_prefix<list<char, int, double>, 1>>);
static_assert(same_as<list<int, double>, meta::remove_prefix<list<char, int, double>, 1>>);
static_assert(same_as<list<char, int>, meta::remove_suffix<list<char, int, double>, 1>>);

static_assert(same_as<list<int, double, char>, meta::slice<list<int, double, char, float>, 0, 3>>);

static_assert(same_as<list<int, int>, meta::remove_prefix<meta::filled_list<int, 500>, 498>>);

int main() {}
