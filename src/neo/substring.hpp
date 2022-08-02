#pragma once

#include "./reconstruct.hpp"
#include "./text_range.hpp"

namespace neo {

inline constexpr struct substring_fn {
    template <text_range R, typename Iter = std::ranges::iterator_t<R>>
    constexpr text_range auto operator()(R&&                        text,
                                         std::ranges::iterator_t<R> iter,
                                         std::ranges::iterator_t<R> end) const noexcept {
        if constexpr (neo::reconstructible_range<R>) {
            return neo::reconstruct_range(NEO_FWD(text), iter, end);
        } else {
            return std::ranges::subrange(iter, end);
        }
    }

    template <text_range R>
    constexpr text_range auto operator()(R&&                          text,
                                         std::ranges::range_size_t<R> off,
                                         std::ranges::range_size_t<R> len) const noexcept {
        auto end     = std::ranges::end(text);
        auto it      = std::ranges::next(std::ranges::begin(text), off, end);
        auto new_end = std::ranges::next(it, len, end);
        return (*this)(NEO_FWD(text), it, new_end);
    }

    template <text_range R>
    constexpr text_range auto operator()(R&&                          text,
                                         std::ranges::range_size_t<R> off) const noexcept {
        auto len = neo::text_range_size(text);
        return (*this)(NEO_FWD(text), off, len);
    }
} substring;

template <text_range R>
using substring_t = decltype(substring(NEO_DECLVAL(R), 0));

}  // namespace neo
