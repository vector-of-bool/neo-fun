#pragma once

#include "./assert.hpp"
#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./iterator_facade.hpp"
#include "./returns.hpp"
#include "./tl.hpp"
#include "./version.hpp"

#include <concepts>
#include <functional>
#include <ranges>
#include <vector>

namespace neo::views {}

namespace neo::ranges {

namespace views = neo::views;

using std::ranges::begin;
using std::ranges::end;
using std::ranges::iterator_t;
using std::ranges::range;
using std::ranges::range_difference_t;
using std::ranges::range_reference_t;
using std::ranges::range_size_t;
using std::ranges::range_value_t;
using std::ranges::sentinel_t;
using std::ranges::view;

template <range T>
struct range_index {
    using type = std::make_unsigned_t<range_difference_t<T>>;
};

template <std::ranges::sized_range T>
struct range_index<T> {
    using type = range_size_t<T>;
};

template <std::ranges::range T>
using range_index_t = typename range_index<T>::type;

// clang-format off
/**
 * @brief A view whose iterator type does not vary by the const-ness of the range
 */
template <typename Range>
concept simple_view =
    view<Range> &&
    range<const Range> &&
    std::same_as<iterator_t<Range>,
                 iterator_t<const Range>> &&
    std::same_as<sentinel_t<Range>,
                 sentinel_t<const Range>>;

template <typename Range>
concept nothrow_range =
    range<Range> &&
    requires (Range& r, std::ranges::iterator_t<Range> iter) {
        { std::ranges::begin(r) } noexcept;
        { std::ranges::end(r) } noexcept;
        { ++iter } noexcept;
        { *iter } noexcept;
    };
// clang-format on

struct pipable {
    template <typename Left, neo::invocable2<Left> Right>
    constexpr friend decltype(auto)
    operator|(Left&& left, Right&& right) noexcept(nothrow_invocable<Right, Left>) {
        return NEO_INVOKE(NEO_FWD(right), NEO_FWD(left));
    }
};

struct to_vector_fn : pipable {
    template <std::ranges::range R>
    auto operator()(R&& range) const noexcept(nothrow_range<R>) {
        using vec_type = std::vector<range_value_t<R>>;
        if constexpr (std::ranges::common_range<R>) {
            return vec_type{std::ranges::begin(range), std::ranges::end(range)};
        } else {
            vec_type ret;
            if constexpr (std::ranges::sized_range<R>) {
                ret.reserve(std::ranges::size(range));
            }
            for (auto&& elem : range) {
                ret.push_back(NEO_FWD(elem));
            }
            return ret;
        }
    }
};

/**
 * @brief Convert a range into a vector of its contents
 */
inline constexpr to_vector_fn to_vector;

struct exhaust_fn : pipable {
    template <std::ranges::input_range R>
    constexpr void operator()(R&& range) {
        for (auto&& el : range) {
            (void)el;
        }
    }
};

/**
 * @brief Simply exhaust the given range by advancing its iterators all the way
 * through.
 */
inline constexpr exhaust_fn exhaust;

template <typename Func>
class each_closure : public pipable {
    Func _fn;

public:
    constexpr each_closure(Func fn)
        : _fn(NEO_FWD(fn)) {}

    template <std::ranges::range Range>
    requires neo::invocable2<Func&, range_reference_t<Range>>  //
    constexpr void
    operator()(Range&& range) noexcept(nothrow_invocable<Func&, range_reference_t<Range>>) {
        for (auto&& elem : range) {
            NEO_INVOKE(_fn, NEO_FWD(elem));
        }
    }
};

struct each_fn {
    template <typename Func>
    constexpr auto operator()(Func&& fn) const noexcept {
        return each_closure<Func>(NEO_FWD(fn));
    }
};

/**
 * @brief Apply an invocable to each element of the given range. Results are discarded.
 */
inline constexpr each_fn each;

template <typename R, typename T>
concept range_of = range<R> && std::same_as<range_value_t<R>, T>;

template <typename R, typename T>
concept input_range_of = range_of<R, T> && std::ranges::input_range<R>;

template <typename R, typename T>
concept forward_range_of = input_range_of<R, T> && std::ranges::input_range<R>;

template <typename R, typename T>
concept bidirectional_range_of = forward_range_of<R, T> && std::ranges::bidirectional_range<R>;

template <typename R, typename T>
concept random_access_range_of
    = bidirectional_range_of<R, T> && std::ranges::random_access_range<R>;

template <typename R, typename T>
concept contiguous_range_of = random_access_range_of<R, T> && std::ranges::contiguous_range<R>;

template <typename Dest>
struct write_into {
    Dest _dest;

    template <typename Arg>
    void operator()(Arg&& arg) requires requires {
        *_dest = NEO_FWD(arg);
    }
    { *_dest = NEO_FWD(arg); }
};

template <typename D>
explicit write_into(D&&) -> write_into<D>;

template <typename Selector, typename... Handlers>
class distribute : public pipable {
private:
    Selector                _select;
    std::tuple<Handlers...> _handlers;

    template <std::size_t... Is>
    void _dist(auto&& elem, std::integral auto select, std::index_sequence<Is...>) {
        bool did_select
            = ((static_cast<std::size_t>(select) == Is
                && (static_cast<void>(NEO_INVOKE(std::get<Is>(_handlers), NEO_FWD(elem))), 1))
               || ...);
        neo_assert(expects,
                   did_select,
                   "Invalid returned integral select index in neo::ranges::distribute()",
                   select,
                   sizeof...(Handlers));
    }

public:
    distribute(Selector&& sel, Handlers&&... hs)
        : _select(NEO_FWD(sel))
        , _handlers(NEO_FWD(hs)...) {}

    // clang-format off
    template <std::ranges::input_range R>
    requires
            neo::invocable2<Selector, std::ranges::range_reference_t<R>>
        && (neo::invocable2<Handlers, std::ranges::range_reference_t<R>> && ...)
    constexpr void operator()(R&& r) noexcept(nothrow_range<R>) {
        // clang-format on
        for (auto&& elem : r) {
            std::integral auto idx = _select(std::as_const(elem));
            _dist(NEO_FWD(elem), idx, std::index_sequence_for<Handlers...>{});
        }
    }
};

}  // namespace neo::ranges

namespace neo {

using neo::ranges::to_vector;

}  // namespace neo

#include "./views/enumerate.hpp"
