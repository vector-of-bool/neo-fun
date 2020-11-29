#include <neo/function_traits.hpp>

void foo(int) noexcept;

static_assert(
    std::is_base_of_v<neo::signature_tag<void(int)>, neo::invocable_signature<decltype(foo)>>);

static_assert(
    std::is_base_of_v<neo::signature_tag<void(int)>, neo::invocable_signature<decltype(&foo)>>);

static_assert(neo::invocable_is_noexcept_v<decltype(foo)>);

struct callable {
    int  member;
    auto operator()() { return 12; }

    int do_thing(double) const&;
};

static_assert(std::is_same_v<int, neo::invocable_return_type_t<callable>>);
static_assert(std::is_same_v<neo::tag<>, neo::invocable_arg_types_t<callable>>);

static_assert(std::is_same_v<int&, neo::invocable_return_type_t<decltype(&callable::member)>>);

static_assert(std::is_same_v<int, neo::invocable_return_type_t<decltype(&callable::do_thing)>>);
static_assert(std::is_same_v<neo::tag<const callable&, double>,
                             neo::invocable_arg_types_t<decltype(&callable::do_thing)>>);

static_assert(std::is_same_v<neo::tag<double, double, char>,
                             neo::invocable_arg_types_t<int (&)(double, double, char)>>);

static_assert(std::is_same_v<neo::tag<double, double, char>,
                             neo::invocable_arg_types_t<int(&&)(double, double, char)>>);
