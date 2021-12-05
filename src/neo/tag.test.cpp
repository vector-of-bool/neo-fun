#include <neo/tag.hpp>

#include <tuple>
#include <variant>

static_assert(neo::tag_size_v<neo::tag<int, int>> == 2);
static_assert(neo::tag_size_v<neo::tag<>> == 0);

static_assert(std::same_as<neo::tag<>, neo::template_args_tag_t<std::tuple<>>>);
static_assert(
    std::same_as<neo::tag<int, double>, neo::template_args_tag_t<std::tuple<int, double>>>);
static_assert(std::same_as<neo::tag<int, char>, neo::template_args_tag_t<std::variant<int, char>>>);
