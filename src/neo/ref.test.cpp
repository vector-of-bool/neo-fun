#include <neo/ref.hpp>

#include <type_traits>

using neo::cref_v;
using neo::ref_v;
using std::is_same_v;

static_assert(is_same_v<decltype(ref_v<int>), int&&>);
static_assert(is_same_v<decltype(ref_v<int&&>), int&&>);
static_assert(is_same_v<decltype(ref_v<int&>), int&>);
static_assert(is_same_v<decltype(ref_v<const int&>), const int&>);
static_assert(is_same_v<decltype(ref_v<const int>), const int&&>);
static_assert(is_same_v<decltype(cref_v<int>), const int&&>);
static_assert(is_same_v<decltype(cref_v<int&>), const int&>);
static_assert(is_same_v<decltype(cref_v<int&&>), const int&&>);
static_assert(is_same_v<decltype(cref_v<const int>), const int&&>);
static_assert(is_same_v<decltype(cref_v<const int&>), const int&>);
static_assert(is_same_v<decltype(cref_v<const int&&>), const int&&>);
