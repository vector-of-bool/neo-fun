#include "./pick_type.hpp"

#include "./tag.hpp"

#include <cstddef>
#include <utility>

template <std::size_t>
struct item {};

template <typename Seq>
struct p;

template <std::size_t... I>
struct p<std::index_sequence<I...>> {
    using tag = neo::tag<item<I>...>;
};

#ifndef __INTELLISENSE__

item<1>    f1 = neo::pick_type_of_t<1, p<std::make_index_sequence<4050>>::tag>{};
item<0>    f2 = neo::pick_type_of_t<0, p<std::make_index_sequence<4050>>::tag>{};
item<94>   f3 = neo::pick_type_of_t<94, p<std::make_index_sequence<4050>>::tag>{};
item<940>  f4 = neo::pick_type_of_t<940, p<std::make_index_sequence<4050>>::tag>{};
item<4047> f5 = neo::pick_type_of_t<4047, p<std::make_index_sequence<4050>>::tag>{};
item<9845> f6 = neo::pick_type_of_t<9845, p<std::make_index_sequence<10'000>>::tag>{};
// nth f     = 9;

#endif

// Required for executable linkage
int main() {}
