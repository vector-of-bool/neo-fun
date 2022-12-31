#pragma once

#include "./assert.hpp"
#include "./concepts.hpp"
#include "./declval.hpp"
#include "./reconstruct.hpp"
#include "./returns.hpp"
#include "./tag.hpp"

#include <concepts>
#include <ranges>

namespace neo {

namespace _sr = std::ranges;
namespace _sv = std::views;

template <typename R>
concept sliceable_range = _sr::input_range<R>      //
    and (reconstructible_range<remove_cvref_t<R>>  //
         or _sr::borrowed_range<remove_cvref_t<R>>);

namespace _slice_detail {

struct slice_fn_base {
    template <typename R>
    static constexpr auto _adjust(R&& rng, auto idx) {
        if (idx < 0) {
            neo_assert(expects,
                       _sr::sized_range<R> or _sr::forward_range<R>,
                       "Cannot use a negative slice indexing with an unsized input range",
                       idx);
            idx = _sr::distance(rng) + idx;
            if (idx < 0) {
                idx = 0;
            }
        }
        return idx;
    }

    template <typename R>
    constexpr static bool _is_nothrow = ranges::nothrow_range<R>  //
        or is_nothrow_reconstructible_range_v<remove_cvref_t<R>>;

    template <sliceable_range R,
              typename It = _sr::iterator_t<R>,
              sentinel_for<It> Stop,
              typename R_ = remove_cvref_t<R>>
    constexpr sliceable_range auto  // sole_type_t<decltype(_slice_detail::slice_type<R_>())>
    operator()(R&& rng, std::type_identity_t<It> first, Stop stop) const noexcept(_is_nothrow<R>) {
        if constexpr (reconstructible_range<R_>) {
            // Create a copy of the range with the given iterator pair
            return reconstruct_range(NEO_FWD(rng), first, stop);
        } else if constexpr (_sr::borrowed_range<R_>) {
            // We are safe to return a subrange that views the range
            return _sr::subrange{first, stop};
        }
    }

    template <_sr::forward_range R>
        requires sliceable_range<R>
    constexpr auto operator()(R&&                        rng,
                              _sr::range_difference_t<R> start_pos,
                              _sr::range_difference_t<R> end_pos) const
        noexcept(_is_nothrow<R>and ranges::nothrow_range<R>) {
        auto start_pos_ = _adjust(rng, start_pos);
        auto end_pos_   = _adjust(rng, end_pos);
        neo_assert(expects,
                   start_pos_ <= end_pos_,
                   "Slice start index must be less than the end index",
                   start_pos,
                   end_pos,
                   start_pos_,
                   end_pos_);
        auto it  = _sr::begin(rng);
        auto end = _sr::end(rng);
        _sr::advance(it, start_pos_, end);
        auto diff    = end_pos_ - start_pos_;
        auto new_end = _sr::next(it, diff, end);
        return (*this)(NEO_FWD(rng), it, new_end);
    }

    template <sliceable_range R>
    constexpr auto operator()(R&& rng, _sr::range_difference_t<R> start_pos) const
        noexcept(_is_nothrow<R>and ranges::nothrow_range<R>) {
        start_pos = _adjust(rng, start_pos);
        auto it   = _sr::begin(rng);
        auto end  = _sr::end(rng);
        _sr::advance(it, start_pos, end);
        return (*this)(NEO_FWD(rng), it, end);
    }

    template <sliceable_range R>
    constexpr auto operator()(R&& rng) const
        NEO_RETURNS((*this)(NEO_FWD(rng), _sr::begin(rng), std::ranges::end(rng)));
};

}  // namespace _slice_detail

/**
 * @brief Obtain a segment of a text_range with characters removed from the beginning or the end
 */
inline constexpr struct slice_fn {
    template <sliceable_range R>
    constexpr auto operator()(R&& rng, auto&&... args) const
        noexcept(_slice_detail::slice_fn_base::_is_nothrow<R>) {
        return _slice_detail::slice_fn_base{}(NEO_FWD(rng), NEO_FWD(args)...);
    }

    constexpr static inline struct view_fn {
        template <_sr::viewable_range R>
            requires sliceable_range<_sv::all_t<R>>
        constexpr auto operator()(R&& rng, auto&&... args) const
            noexcept(_slice_detail::slice_fn_base::_is_nothrow<_sv::all_t<R>>) {
            return _slice_detail::slice_fn_base{}(_sv::all(NEO_FWD(rng)), NEO_FWD(args)...);
        }
    } view = {};
} slice;

template <sliceable_range R>
using slice_t = decltype(slice(NEO_DECLVAL(R &&)));

template <_sr::viewable_range R>
using view_slice_t = decltype(slice.view(NEO_DECLVAL(R &&)));

}  // namespace neo
