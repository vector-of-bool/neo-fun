#pragma once

#include "../ranges.hpp"

namespace neo::views {

template <std::ranges::input_range R>
struct enumerator_reference {
    using count_type     = ranges::range_index_t<R>;
    using reference_type = ranges::range_reference_t<R>;
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
        using BaseSentinel = ranges::sentinel_t<Base>;

        BaseSentinel _base_sentinel = {};

    public:
        constexpr _sentinel_t() = default;
        constexpr explicit _sentinel_t(BaseSentinel s)
            : _base_sentinel(s) {}

        constexpr _sentinel_t(_sentinel_t<!Const> o)  //
            requires Const && std::convertible_to<ranges::sentinel_t<R>, BaseSentinel>
            : _base_sentinel(o.base()) {}

        constexpr auto base() const noexcept { return _base_sentinel; }

        friend constexpr bool operator==(const _iter_t<Const>& it, const _sentinel_t& self) {
            return it.base() == self.base();
        }
    };

    template <bool Const>
    class _iter_t : public neo::iterator_facade<_iter_t<Const>> {
        using Base       = std::conditional_t<Const, const R, R>;
        using count_type = ranges::range_index_t<Base>;
        using BaseIter   = ranges::iterator_t<Base>;

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

        constexpr _iter_t(_iter_t<!Const> o)                                        //
            requires Const && std::convertible_to<ranges::iterator_t<R>, BaseIter>  //
            : _base_iter(o.base()), _pos(o.position()) {}

        using reference = enumerator_reference<Base>;

        using difference_type = ranges::range_difference_t<Base>;

        constexpr void
        advance(difference_type diff) requires std::ranges::random_access_range<Base> {
            _base_iter += diff;
            _pos = static_cast<count_type>(static_cast<difference_type>(_pos) + diff);
        }

        constexpr void increment() {
            ++_base_iter;
            ++_pos;
        }

        constexpr void decrement() requires std::ranges::bidirectional_range<Base> {
            --_base_iter;
            --_pos;
        }

        constexpr auto dereference() const noexcept(noexcept(*_base_iter)) {
            return reference{_pos, *_base_iter};
        }

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
    constexpr auto begin() noexcept(ranges::nothrow_range<R>) requires(!ranges::simple_view<R>) {
        return _iter_t<false>(std::ranges::begin(_range), 0);
    }

    // Begin for simple views are always const-iterators
    constexpr auto begin() const
        noexcept(ranges::nothrow_range<R>) requires ranges::simple_view<R> {
        return _iter_t<true>(std::ranges::begin(_range), 0);
    }

    constexpr auto end() noexcept {
        if constexpr (!ranges::simple_view<R>) {
            // Base case for end() uses a non-const sentinel:
            return _sentinel_t<false>(std::ranges::end(_range));
        } else if constexpr (std::ranges::common_range<R> && std::ranges::sized_range<R>) {
            // Case: A non-const common_range with a known size, uses non-const iterator as its end
            // type
            return _iter_t<false>{std::ranges::end(_range),
                                  static_cast<ranges::range_index_t<R>>(size())};
        } else {
            return std::as_const(*this).end();
        }
    }

    constexpr auto end() const noexcept {
        if constexpr (ranges::range<const R>) {
            // Case: Const with a const range uses a const-sentinel
            return _sentinel_t<true>{std::ranges::end(_range)};
        } else if constexpr (std::ranges::common_range<const R> && std::ranges::sized_range<R>) {
            // Case: Const common_range with a known size uses a non-const iterator
            return _iter_t<true>{std::ranges::end(_range),
                                 static_cast<ranges::range_index_t<R>>(size())};
        }
    }

    constexpr auto size() const noexcept requires std::ranges::sized_range<R> {
        return std::ranges::size(_range);
    }
    // Obtain the underlying range:
    constexpr R base() const& requires std::copy_constructible<R> { return _range; }
    constexpr R base() && { return NEO_MOVE(_range); }
};

template <typename R>
enumerate_view(R&&) -> enumerate_view<std::views::all_t<R>>;

struct enumerate_fn : ranges::pipable {
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

}  // namespace neo::views