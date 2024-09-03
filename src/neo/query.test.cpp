#include "./query.hpp"
#include <vector>

struct get_allocator : neo::query_interface<get_allocator> {
    static constexpr auto exec(auto const& obj)
        requires requires { obj.get_allocator(); }
    {
        return obj.get_allocator();
    }
};

static_assert(neo::valid_query_for<get_allocator, std::vector<int>>);
static_assert(not neo::valid_query_for<get_allocator, std::array<int, 4>>);
