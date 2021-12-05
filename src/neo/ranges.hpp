#pragma once

#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./iterator_facade.hpp"
#include "./returns.hpp"
#include "./version.hpp"

#include <concepts>
#include <functional>
#include <ranges>
#include <vector>

namespace neo::ranges {

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
// clang-format on

struct pipable {
    template <typename Left, neo::invocable<Left> Right>
    constexpr friend decltype(auto)
    operator|(Left&& left, Right&& right) noexcept(nothrow_invocable<Right, Left>) {
        return neo::invoke(NEO_FWD(right), NEO_FWD(left));
    }
};

struct to_vector_fn : pipable {
    template <std::ranges::range R>
    auto operator()(R&& range) const noexcept {
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

namespace views {

template <std::ranges::input_range R>
struct enumerator_reference {
    using count_type     = range_index_t<R>;
    using reference_type = range_reference_t<R>;
    count_type     index;
    reference_type value;
};

template <std::ranges::input_range R>
class enumerate_view : public std::ranges::view_interface<enumerate_view<R>> {
    R _range = {};

    template <bool Const>
    class _iter_t;

    template <bool Const>
    class _sentinel_t {
        using Base         = std::conditional_t<Const, const R, R>;
        using BaseSentinel = sentinel_t<Base>;

        BaseSentinel _base_sentinel = {};

    public:
        constexpr _sentinel_t() = default;
        constexpr explicit _sentinel_t(BaseSentinel s)
            : _base_sentinel(s) {}

        constexpr _sentinel_t(_sentinel_t<!Const> o)  //
            requires Const
            && std::convertible_to<sentinel_t<R>, BaseSentinel> : _base_sentinel(o.base()) {}

        constexpr auto base() const noexcept { return _base_sentinel; }

        friend constexpr bool operator==(const _iter_t<Const>& it, const _sentinel_t& self) {
            return it.base() == self.base();
        }
    };

    template <bool Const>
    class _iter_t : public neo::iterator_facade<_iter_t<Const>> {
        using Base       = std::conditional_t<Const, const R, R>;
        using count_type = range_index_t<Base>;
        using BaseIter   = iterator_t<Base>;

        BaseIter   _base_iter = {};
        count_type _pos       = 0;

    public:
        // using iterator_category = std::iterator_traits<BaseIter>::iterator_category;
        using sentinel_type = _sentinel_t<Const>;

        constexpr auto base() const noexcept requires std::copyable<BaseIter> { return _base_iter; }
        constexpr auto position() const noexcept { return _pos; }

        constexpr _iter_t() = default;
        constexpr _iter_t(BaseIter it, count_type pos)
            : _base_iter(it)
            , _pos(pos) {}

        constexpr _iter_t(_iter_t<!Const> o)                                //
            requires Const && std::convertible_to<iterator_t<R>, BaseIter>  //
            : _base_iter(o.base()), _pos(o.position()) {}

        using reference = enumerator_reference<Base>;

        using difference_type = range_difference_t<Base>;

        constexpr void
        advance(difference_type diff) requires std::ranges::random_access_range<Base> {
            _base_iter += diff;
            _pos += diff;
        }

        constexpr void increment() {
            ++_base_iter;
            ++_pos;
        }

        constexpr void decrement() requires std::ranges::bidirectional_range<Base> {
            --_base_iter;
            --_pos;
        }

        constexpr auto dereference() const noexcept { return reference{_pos, *_base_iter}; }

        constexpr bool operator==(const _iter_t& other) const noexcept  //
            requires std::equality_comparable<BaseIter> {
            return _base_iter == other._base_iter;
        }

        constexpr auto distance_to(const _iter_t& other) const noexcept  //
            requires std::random_access_iterator<BaseIter> {
            return _base_iter - other._base_iter;
        }
    };

public:
    constexpr enumerate_view() = default;
    constexpr enumerate_view(R&& r)
        : _range(NEO_FWD(r)) {}

    // Non-const begin for non-simple views use non-const-iterators
    constexpr auto begin() requires(!simple_view<R>) {
        return _iter_t<false>(std::ranges::begin(_range), 0);
    }

    // Begin for simple views are always const-iterators
    constexpr auto begin() const requires simple_view<R> {
        return _iter_t<true>(std::ranges::begin(_range), 0);
    }

    // Base case for end() uses a non-const sentinel:
    constexpr auto end() requires(!simple_view<R>) {
        return _sentinel_t<false>(std::ranges::end(_range));
    }

    // Case: A non-const common_range with a known size, uses non-const iterator as its end type
    constexpr auto end() requires std::ranges::common_range<R> && std::ranges::sized_range<R> {
        return _iter_t<false>{std::ranges::end(_range), static_cast<range_index_t<R>>(size())};
    }

    // Case: Const with a const range uses a const-sentinel
    constexpr auto end() const requires range<const R> {
        return _sentinel_t<true>{std::ranges::end(_range)};
    }

    // Case: Const common_range with a known size uses a non-const iterator
    constexpr auto
    end() const requires std::ranges::common_range<const R> && std::ranges::sized_range<R>  //
    {
        return _iter_t<true>{std::ranges::end(_range), static_cast<range_index_t<R>>(size())};
    }

    constexpr auto size() requires std::ranges::sized_range<R> { return std::ranges::size(_range); }
    // Obtain the underlying range:
    constexpr R base() const& requires std::copy_constructible<R> { return _range; }
    constexpr R base() && { return NEO_MOVE(_range); }
};

template <typename R>
enumerate_view(R&&) -> enumerate_view<std::views::all_t<R>>;

struct enumerate_fn : pipable {
    /**
     * @brief View a range as a range of {index, value} pairs
     */
    template <std::ranges::input_range R>
    auto operator()(R&& range) const noexcept {
        return enumerate_view(NEO_FWD(range));
    }
};

/**
 * @brief View a range as a range of {index, value} pairs
 */
inline constexpr enumerate_fn enumerate;

}  // namespace views

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
            neo::invoke(_fn, NEO_FWD(elem));
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

}  // namespace neo::ranges

namespace neo {

namespace views = neo::ranges::views;

using neo::ranges::to_vector;

}  // namespace neo
