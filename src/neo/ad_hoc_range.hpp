#pragma once

#include "./version.hpp"

#ifdef __cpp_lib_concepts
#include <concepts>
#include <iterator>
#endif

namespace neo {

/**
 * @brief Generate an ad-hoc range object from an iterator and a sentinel
 *
 * This is also built to work with CTAD - If given a single iterator, the end
 * iterator will be a default-initialized iterator of the same type. If given
 * two parameters, the first will be the iterator and the second will be the
 * end sentinel.
 *
 * @tparam Iter The type of the iterator
 * @tparam Sentinel The type of the sentinel, or another iterator
 */

#if __cpp_lib_concepts
template <std::input_or_output_iterator Iter, std::sentinel_for<Iter> Sentinel>
#else
template <typename Iter, typename Sentinel>
#endif
class ad_hoc_range {
    [[no_unique_address]] Iter     _iter;
    [[no_unique_address]] Sentinel _end;

public:
    constexpr ad_hoc_range() = default;
    constexpr ad_hoc_range(Iter it, Sentinel s)
        : _iter(it)
        , _end(s) {}

    constexpr explicit ad_hoc_range(Iter it)
        : _iter(it)
        , _end() {}

    constexpr Iter     begin() const noexcept { return _iter; }
    constexpr Sentinel end() const noexcept { return _end; }
};

#if __cpp_lib_concepts

template <std::input_or_output_iterator Iter, std::sentinel_for<Iter> Sentinel>
explicit ad_hoc_range(Iter, Sentinel) -> ad_hoc_range<Iter, Sentinel>;

// clang-format off
template <std::input_or_output_iterator Iter>
    requires requires {
        typename Iter::sentinel_type;
        requires std::sentinel_for<Iter, typename Iter::sentinel_type>;
    }
explicit ad_hoc_range(Iter) -> ad_hoc_range<Iter, typename Iter::sentinel_type>;
// clang-format on

#else

template <typename Iter, typename Sentinel>
explicit ad_hoc_range(Iter, Sentinel) -> ad_hoc_range<Iter, Sentinel>;

template <typename Iter, typename Sentinel = typename Iter::sentinel_type>
explicit ad_hoc_range(Iter) -> ad_hoc_range<Iter, Sentinel>;

#endif

}  // namespace neo
