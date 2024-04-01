#pragma once

#include "./meta.hpp"

namespace neo {

using meta::find_type_v;
using meta::find_type_in;

template <std::size_t N, typename L>
using pick_type_of_t [[deprecated("Use neo::meta::at")]] = meta::at<L, N>;

} // namespace neo
