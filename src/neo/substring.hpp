#pragma once

#include "./assert.hpp"
#include "./reconstruct.hpp"
#include "./slice.hpp"
#include "./text_range.hpp"

namespace neo {

using std_string_subrange      = std::ranges::subrange<std::string::iterator>;
using std_string_view_subrange = std::ranges::subrange<std::string_view::iterator>;

/**
 * @brief Obtain a segment of a text_range with characters removed from the beginning or the end
 */
inline constexpr struct substring_fn {
    template <text_range R>
    requires sliceable_range<R>
    constexpr auto operator()(R&& rng, auto&&... args) const
        NEO_RETURNS(neo::slice(NEO_FWD(rng), args...));

    template <text_array_ref R>
    constexpr auto operator()(R&& rng, auto&&... args) const
        NEO_RETURNS(neo::slice(neo::view_text(rng), args...));
} substring;

template <text_range R>
using substring_t = decltype(substring(NEO_DECLVAL(R &&)));

}  // namespace neo
