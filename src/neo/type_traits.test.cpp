#include "./type_traits.hpp"

#include "./concepts.hpp"

#include <string>
#include <utility>

int main() {}

static_assert(neo_is_void(void));
static_assert(neo_is_void(void const));
static_assert(neo_is_void(void const volatile));
static_assert(neo_is_void(void volatile));

static_assert(neo_is_void(void));
static_assert(neo_is_const(const void));
static_assert(not neo_is_const(void));

static_assert(not neo_is_integral(void));
static_assert(not neo_is_floating_point(const void));
static_assert(neo_is_floating_point(double));
static_assert(neo_is_floating_point(double const));

static_assert(neo_is_null_pointer(std::nullptr_t));

static_assert(neo_array_rank(int[][2][1]) == 3);

struct base {};

struct derived : base {};

static_assert(neo_is_base_of(base, derived));
static_assert(not neo_is_base_of(derived, base));

static_assert(neo::same_as<neo::common_type_t<double>, double>);
static_assert(not neo::has_common_type<>);
static_assert(neo::has_common_type<int>);
static_assert(neo::has_common_type<float, double>);
static_assert(neo::same_as<neo::common_type_t<float, double>, double>);
static_assert(neo::same_as<neo::common_type_t<float, double, double>, double>);
static_assert(not neo::has_common_type<float, std::string>);
static_assert(not neo::has_common_type<float, std::string, float>);
static_assert(not neo::has_common_type<float, double, std::string, float>);

static_assert(neo::same_as<neo::common_reference_t<int>, int>);
static_assert(neo::same_as<neo::common_reference_t<int&>, int&>);

static_assert(neo::same_as<neo::common_reference_t<int&, int&>, int&>);
static_assert(neo::same_as<neo::common_reference_t<int&, int&, int&>, int&>);
static_assert(neo::same_as<neo::common_reference_t<int&, const int&>, const int&>);
static_assert(neo::same_as<neo::common_reference_t<const int&, int&>, const int&>);
static_assert(neo::same_as<neo::common_reference_t<const int&&, int&&>, const int&&>);
static_assert(neo::same_as<neo::common_reference_t<const int&&, const int&&>, const int&&>);
static_assert(
    neo::same_as<neo::common_reference_t<const int&&, const volatile int&&>, const volatile int&&>);

static_assert(not neo::has_common_reference<std::string&, int&>);

static_assert(neo::same_as<neo::common_reference_t<int&, int>, std::common_reference_t<int&, int>>);
static_assert(
    neo::same_as<neo::common_reference_t<int&, double&>, std::common_reference_t<int&, double&>>);
static_assert(neo::same_as<neo::common_reference_t<int&, double&, float&>, double>);
