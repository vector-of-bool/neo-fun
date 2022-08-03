#pragma once

#include "./reconstruct.hpp"
#include "./text_range.hpp"

namespace neo {

inline constexpr struct substring_fn {
    template <text_range R>
    constexpr text_subrange<R> operator()(text_subrange<R>,
                                          std::ranges::iterator_t<R> iter,
                                          std::ranges::iterator_t<R> end) const noexcept {
        return {iter, end};
    }

    template <text_range R>
    requires std::ranges::borrowed_range<R> and(not is_text_subrange<std::remove_cvref_t<R>>)  //
        constexpr text_subrange<std::remove_cvref_t<R>>
        operator()(R&&,
                   std::ranges::iterator_t<R> iter,
                   std::ranges::iterator_t<R> end) const noexcept {
        return {iter, end};
    }

    template <text_range R>
    requires std::ranges::borrowed_range<R>
    constexpr text_range auto operator()(R&&                          text,
                                         std::ranges::range_size_t<R> off,
                                         std::ranges::range_size_t<R> len) const noexcept {
        auto end     = std::ranges::begin(text) + neo::text_range_size(text);
        auto it      = std::ranges::next(std::ranges::begin(text), off, end);
        auto new_end = std::ranges::next(it, len, end);
        return (*this)(NEO_FWD(text), it, new_end);
    }

    template <text_range R>
    requires std::ranges::borrowed_range<R>
    constexpr text_range auto operator()(R&& text) const noexcept {
        auto it = std::ranges::begin(text);
        return (*this)(NEO_FWD(text), it, it + neo::text_range_size(text));
    }

    template <text_range R>
    requires std::ranges::borrowed_range<R>
    constexpr text_range auto operator()(R&&                          text,
                                         std::ranges::range_size_t<R> off) const noexcept {
        auto len = neo::text_range_size(text);
        return (*this)(NEO_FWD(text), off, len);
    }
} substring;

template <text_range R>
using substring_t = decltype(substring(NEO_DECLVAL(R &&)));

}  // namespace neo
