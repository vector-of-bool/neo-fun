#include <neo/concepts.hpp>
#include <neo/ref.hpp>

#include <catch2/catch.hpp>
#include <type_traits>

using neo::cref_v;
using neo::mref_v;
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
static_assert(is_same_v<decltype(mref_v<int&&>), int&&>);
static_assert(is_same_v<decltype(mref_v<const int&&>), int&&>);
static_assert(is_same_v<decltype(mref_v<int&>), int&>);
static_assert(is_same_v<decltype(mref_v<const int&>), int&>);

static_assert(neo::equality_comparable<neo::reference_object<int&>>);
static_assert(neo::totally_ordered<neo::reference_object<int&>>);
static_assert(neo::copyable<neo::reference_object<int&>>);
static_assert(neo::explicit_convertible_to<neo::reference_object<int&>, int&>);
static_assert(neo::explicit_convertible_to<neo::reference_object<int&>, const int&>);

// This is required in order to link in some configurations MSVC
TEST_CASE("nil") {}
