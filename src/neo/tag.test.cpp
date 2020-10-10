#include <neo/tag.hpp>

static_assert(neo::tag_size_v<neo::tag<int, int>> == 2);
static_assert(neo::tag_size_v<neo::tag<>> == 0);
