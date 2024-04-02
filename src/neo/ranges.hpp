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

namespace neo {

namespace _sr = std::ranges;


}  // namespace neo

namespace neo::views {}

namespace neo::ranges {

namespace views = neo::views;

using _sr::begin;
using _sr::end;
using _sr::iterator_t;
using _sr::range;
using _sr::range_difference_t;
using _sr::range_reference_t;
using _sr::range_size_t;
using _sr::range_value_t;
using _sr::sentinel_t;
using _sr::view;

template <range T>
struct range_index {
    using type = std::make_unsigned_t<range_difference_t<T>>;
};

template <_sr::sized_range T>
struct range_index<T> {
    using type = range_size_t<T>;
};

template <_sr::range T>
using range_index_t = typename range_index<T>::type;

// clang-format off
/**
 * @brief A view whose iterator type does not vary by the const-ness of the range
 */
template <typename Range>
concept simple_view =
    view<Range> &&
    range<const Range> &&
    same_as<iterator_t<Range>,
            iterator_t<const Range>> &&
    same_as<sentinel_t<Range>,
            sentinel_t<const Range>>;

template <typename Range>
concept nothrow_range =
    range<Range> &&
    requires (Range& r, _sr::iterator_t<Range> iter) {
        { _sr::begin(r) } noexcept;
        { _sr::end(r) } noexcept;
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
    template <_sr::range R>
    auto operator()(R&& range) const noexcept(nothrow_range<R>) {
        using vec_type = std::vector<range_value_t<R>>;
        if constexpr (_sr::common_range<R>) {
            return vec_type{_sr::begin(range), _sr::end(range)};
        } else {
            vec_type ret;
            if constexpr (_sr::sized_range<R>) {
                ret.reserve(_sr::size(range));
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
    template <_sr::input_range R>
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

    template <_sr::range Range>
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
concept range_of = range<R> && same_as<range_value_t<R>, T>;

template <typename R, typename T>
concept input_range_of = range_of<R, T> && _sr::input_range<R>;

template <typename R, typename T>
concept forward_range_of = input_range_of<R, T> && _sr::input_range<R>;

template <typename R, typename T>
concept bidirectional_range_of = forward_range_of<R, T> && _sr::bidirectional_range<R>;

template <typename R, typename T>
concept random_access_range_of = bidirectional_range_of<R, T> && _sr::random_access_range<R>;

template <typename R, typename T>
concept contiguous_range_of = random_access_range_of<R, T> && _sr::contiguous_range<R>;

template <typename Dest>
struct write_into {
    Dest _dest;

    template <typename Arg>
    void operator()(Arg&& arg)
        requires requires { *_dest = NEO_FWD(arg); }
    {
        *_dest = NEO_FWD(arg);
    }
};

template <typename D>
explicit write_into(D&&) -> write_into<D>;

template <typename Selector, typename... Handlers>
class distribute : public pipable {
private:
    Selector                _select;
    std::tuple<Handlers...> _handlers;

    template <std::size_t... Is>
    void _dist(auto&& elem, integral auto select, std::index_sequence<Is...>) {
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
    template <_sr::input_range R>
    requires
            neo::invocable2<Selector, _sr::range_reference_t<R>>
        && (neo::invocable2<Handlers, _sr::range_reference_t<R>> && ...)
    constexpr void operator()(R&& r) noexcept(nothrow_range<R>) {
        // clang-format on
        for (auto&& elem : r) {
            integral auto idx = _select(std::as_const(elem));
            _dist(NEO_FWD(elem), idx, std::index_sequence_for<Handlers...>{});
        }
    }
};

inline constexpr struct udistance_fn {
    template <_sr::input_range R>
    [[nodiscard]] constexpr _sr::range_size_t<R> operator()(R&& rng) const
        noexcept(nothrow_range<R>) {
        const _sr::range_difference_t<R> dist = _sr::distance(rng);
        return static_cast<_sr::range_size_t<R>>(dist);
    }
} udistance;

}  // namespace neo::ranges

namespace neo {

using neo::ranges::to_vector;

}  // namespace neo

#include "./views/enumerate.hpp"
