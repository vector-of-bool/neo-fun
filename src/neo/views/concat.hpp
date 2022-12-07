#pragma once

#include "../attrib.hpp"
#include "../declval.hpp"
#include "../fwd.hpp"
#include "../iterator.hpp"
#include "../iterator_facade.hpp"
#include "../ranges.hpp"
#include "../scalar_box.hpp"

#include <compare>
#include <tuple>
#include <variant>

#define SR std::ranges
#define SV std::views

namespace neo::views {

template <SR::view... Rs>
class concat_view;

namespace _concat_detail {

struct sentinel {
    constexpr bool operator==(sentinel) const noexcept { return true; }
};

template <typename... Rs>
class iterator {
public:
    using difference_type = std::common_type_t<SR::range_difference_t<Rs>...>;
    using value_type      = std::common_type_t<SR::range_value_t<Rs>...>;
    using reference_type  = std::common_reference_t<SR::range_reference_t<Rs>...>;

private:
    using view_type = concat_view<Rs...>;

    const view_type* _view;

    constexpr static inline bool _is_random_access
        = (std::random_access_iterator<SR::iterator_t<Rs>> and ...);
    constexpr static inline bool _is_nothrow = (ranges::nothrow_range<Rs> and ...);
    constexpr static inline bool _is_nothrow_advancing
        = (nothrow_advancing_iterator<SR::iterator_t<Rs>> and ...);

    using var_type = std::variant<SR::iterator_t<Rs>..., sentinel>;

    var_type _var{std::in_place_index<sizeof...(Rs)>, sentinel{}};

    template <std::size_t N>
    constexpr auto& _nth_iter() const noexcept {
        return *std::get_if<N>(&_var);
    }

    template <std::size_t N>
    constexpr auto& _nth_iter() noexcept {
        return *std::get_if<N>(&_var);
    }

    template <std::size_t N>
    constexpr auto& _nth_range() const noexcept {
        return std::get<N>(_view->_tuple);
    }

    template <std::size_t N>
    constexpr var_type _seek_fwd(auto it, difference_type off) noexcept(_is_nothrow) {
        auto& rng  = _nth_range<N>();
        auto  stop = SR::end(rng);
        if (it == stop) {
            // We've hit the end of the subrange
            if constexpr (N + 1 == sizeof...(Rs)) {
                neo_assert(expects, off == 0, "Seek past-the-end of a concat_view", off, N);
                return var_type(sentinel{});
            } else {
                auto& next   = _nth_range<N + 1>();
                auto  new_it = SR::begin(next);
                return _seek_fwd<N + 1>(new_it, off);
            }
        }
        if (off == 0) {
            // We have arrived
            return var_type(std::in_place_index<N>, it);
        } else {
            // We're seeking forwards within the current range
            off -= (off - SR::advance(it, off, stop));
            _adjust_forward<N>();
            return _seek_fwd<N>(it, off);
        }
    }

    template <std::size_t N>
    constexpr var_type _seek_bwd(auto it, difference_type off) noexcept(_is_nothrow) {
        auto  rit  = std::make_reverse_iterator(it);
        auto& rng  = _nth_range<N>();
        auto  stop = SR::rend(rng);
        if (rit == stop) {
            // We've hit the beginning of the range, and we need to roll back to a previous range
            if constexpr (N == 0) {
                neo_assert(expects, off == 0, "Seek before-the-begin of a concat_view", off, N);
                neo::unreachable();
            } else {
                auto& prev_range = _nth_range<N - 1>();
                auto  prev_it    = SR::end(prev_range);
                return _seek_bwd<N - 1>(prev_it, off);
            }
        }
        off -= (off - SR::advance(rit, off, stop));
        if (off == 0) {
            // We are finished
            return var_type(std::in_place_index<N>, rit.base());
        }
        return _seek_bwd<N>(rit.base(), off);
    }

    template <std::size_t N>
    constexpr var_type _seek(difference_type off) noexcept(_is_nothrow) {
        // First: Find our current index
        if constexpr (N == sizeof...(Rs)) {
            neo_assert(expects, off <= 0, "Seek past-the-end of a concat_view", off, N);
            if (off == 0) {
                return var_type(sentinel{});
            }
            return _seek_bwd<N - 1>(SR::end(_nth_range<N - 1>()), -off);
        } else if (N == _var.index()) {
            // We're at the correct position. Now adjust the iterator.
            auto it = _nth_iter<N>();
            if (off >= 0) {
                return _seek_fwd<N>(it, off);
            } else {
                return _seek_bwd<N>(it, -off);
            }
        } else {
            return _seek<N + 1>(off);
        }
    }

    constexpr void _shift(difference_type off) noexcept(_is_nothrow) { _var = _seek<0>(off); }

    template <std::size_t N>
    constexpr void
    _adjust_forward() noexcept((nothrow_advancing_iterator<SR::iterator_t<Rs>> and ...)) {
        if constexpr (N == sizeof...(Rs)) {
            return;
        } else {
            if (_var.index() != N) {
                return _adjust_forward<N + 1>();
            }
            auto& it   = _nth_iter<N>();
            auto& rg   = _nth_range<N>();
            auto  stop = SR::end(rg);
            if (it != stop) {
                // We're okay: We're not at the end of the current subrange yet.
                return;
            }
            // We've hit the end of the subrange. Advance to the next one
            if constexpr (N + 1 == sizeof...(Rs)) {
                // We're done.
                _var.template emplace<N + 1>();
                return;
            } else {
                auto& next_rg = _nth_range<N + 1>();
                auto  new_it  = SR::begin(next_rg);
                _var.template emplace<N + 1>(new_it);
                // The next range might also be empty: Check that one too.
                return _adjust_forward<N + 1>();
            }
        }
    }

    template <std::size_t N>
    constexpr void
    _do_increment() noexcept((nothrow_advancing_iterator<SR::iterator_t<Rs>> and ...)) {
        if constexpr (N == sizeof...(Rs)) {
            neo_assert(expects, false, "Attempt to advance a past-the-end concat_view iterator");
        } else {
            if (_var.index() != N) {
                // Try the next one
                return _do_increment<N + 1>();
            }
            auto& it = _nth_iter<N>();
            ++it;
            return _adjust_forward<N>();
        }
    }

    constexpr void _increment() noexcept((nothrow_advancing_iterator<SR::iterator_t<Rs>> and ...)) {
        if constexpr ((SR::bidirectional_range<Rs> and ...)) {
            _shift(1);
        } else {
            _do_increment<0>();
        }
    }

    constexpr void _decrement() noexcept(noexcept((--NEO_DECLVAL(SR::iterator_t<Rs>&), ...))) {
        _shift(-1);
    }

    template <int N>
    constexpr auto _compare(const iterator& other) const noexcept {
        if constexpr (N == sizeof...(Rs)) {
            return std::strong_ordering::equal;
        } else {
            if (_var.index() != N) {
                // We're not this type
                return _compare<N + 1>(other);
            }
            neo_assert(invariant,
                       other._var.index() == N,
                       "Wrong state during concat_iterator compare",
                       N,
                       _var.index(),
                       other._var.index());
            auto& lhs = _nth_iter<N>();
            auto& rhs = other.template _nth_iter<N>();
            return std::compare_three_way{}(lhs, rhs);
        }
    }

    template <std::size_t N>
    constexpr reference_type _deref() const
        noexcept((noexcept(*NEO_DECLVAL(SR::iterator_t<Rs>)) and ...)) {
        if constexpr (N == sizeof...(Rs)) {
            neo_assert(expects,
                       false,
                       "Dereference of sentinel iterator in neo::views::concat_view",
                       N);
            neo::unreachable();
        } else {
            if (_var.index() == N) {
                return *_nth_iter<N>();
            } else {
                return _deref<N + 1>();
            }
        }
    }

    template <std::size_t N>
    constexpr difference_type _diff_within_subrange(const iterator& other
                                                    [[maybe_unused]]) const noexcept {
        if constexpr (N == sizeof...(Rs)) {
            return 0;
        } else if (_var.index() == N) {
            return _nth_iter<N>() - other._nth_iter<N>();
        } else {
            return _diff_within_subrange<N + 1>(other);
        }
    }

    template <std::size_t N>
    constexpr difference_type _diff_look_back(const iterator& other
                                              [[maybe_unused]]) const noexcept {
        if constexpr (N == SIZE_MAX) {
            neo::unreachable();
        } else {
            if (_var.index() < N) {
                // We're not at the current subrange yet
                return _diff_look_back<N - 1>(other);
            }
            auto& rng = _nth_range<N>();
            if (_var.index() == N) {
                // We've found our subrange
                auto from_begin = SR::distance(SR::begin(rng), _nth_iter<N>());
                return from_begin + _diff_look_back<N - 1>(other);
            }
            // At this point, we're now looking for the subrange containing 'other'
            if (other._var.index() != N) {
                // We're not there yet
                // We're in an intermediate range between the target and ourself, so add the
                // intermediate range size
                auto dist = SR::distance(rng);
                return dist + _diff_look_back<N - 1>(other);
            }
            // We've found the subrange
            auto& other_it = other.template _nth_iter<N>();
            auto  off      = SR::distance(other_it, SR::end(rng));
            return off;
        }
    }

    template <std::size_t N>
    constexpr difference_type _diff_look_ahead(const iterator& other
                                               [[maybe_unused]]) const noexcept {
        if constexpr (N == sizeof...(Rs)) {
            neo::unreachable();
        } else {
            if (_var.index() > N) {
                // We're not at the current subrange yet
                return _diff_look_ahead<N + 1>(other);
            }
            if (_var.index() == N) {
                // We've found our subrange
                auto& rng      = _nth_range<N>();
                auto  from_end = SR::distance(_nth_iter<N>(), SR::end(rng));
                return from_end + _diff_look_ahead<N + 1>(other);
            }
            // At this point, we're now looking for the subrange containing 'other'
            if (other._var.index() != N) {
                // We're not there yet
                // We're in an intermediate range between the target and ourself, so add the
                // intermediate range size
                return SR::distance(_nth_range<N>()) + _diff_look_ahead<N + 1>(other);
            }
            // We've found the subrange
            auto& rng      = _nth_range<N>();
            auto& other_it = other.template _nth_iter<N>();
            auto  off      = SR::distance(SR::begin(rng), other_it);
            return off;
        }
    }

    friend view_type;

    constexpr explicit iterator(view_type const& v) noexcept
        : _view(&v) {}

    template <std::size_t N>
    constexpr explicit iterator(view_type const& v, std::in_place_index_t<N> idx, auto it) noexcept
        : _view(&v)
        , _var(idx, it) {
        _adjust_forward<N>();
    }

public:
    constexpr iterator() = default;

    constexpr reference_type operator*() const
        noexcept((noexcept(*NEO_DECLVAL(Rs).begin()) and ...)) {
        return _deref<0>();
    }

    constexpr iterator& operator++() noexcept(noexcept((++NEO_DECLVAL(SR::iterator_t<Rs>&), ...))) {
        _increment();
        return *this;
    }

    constexpr iterator
    operator++(int) noexcept(noexcept((++NEO_DECLVAL(SR::iterator_t<Rs>&), ...))) {
        auto dup = *this;
        _increment();
        return dup;
    }

    constexpr iterator&
    operator--() noexcept(noexcept((--NEO_DECLVAL(SR::iterator_t<Rs>&), ...)))  //
        requires(SR::bidirectional_range<Rs>and...) {
        _decrement();
        return *this;
    }

    constexpr iterator
    operator--(int) noexcept(noexcept((--NEO_DECLVAL(SR::iterator_t<Rs>&), ...)))  //
        requires(SR::bidirectional_range<Rs>and...) {
        auto old = *this;
        _decrement();
        return old;
    }

    constexpr iterator& operator+=(difference_type offset)  //
        noexcept(_is_nothrow_advancing)                     //
        requires _is_random_access {
        _shift(offset);
        return *this;
    }

    constexpr iterator operator+(difference_type offset) const  //
        noexcept(_is_nothrow_advancing)                         //
        requires _is_random_access {
        auto dup = *this;
        dup += offset;
        return dup;
    }

    friend constexpr iterator operator+(difference_type l,
                                        const iterator& self)  //
        noexcept(_is_nothrow_advancing)                        //
        requires _is_random_access {
        return self + l;
    }

    constexpr iterator operator-(difference_type offset) const  //
        noexcept(_is_nothrow_advancing)                         //
        requires _is_random_access {
        auto dup = *this;
        dup += -offset;
        return dup;
    }

    constexpr iterator& operator-=(difference_type off)  //
        noexcept(_is_nothrow_advancing)                  //
        requires _is_random_access {
        return *this += -off;
    }

    constexpr difference_type operator-(const iterator& other) const
        noexcept(_is_nothrow_advancing)  //
        requires _is_random_access {
        auto idx_diff = _var.index() <=> other._var.index();
        if (idx_diff == 0) {
            // We refer to the same subrange
            return _diff_within_subrange<0>(other);
        } else if (idx_diff > 0) {
            // We are ahead of "other", so we need to look backwards
            return _diff_look_back<sizeof...(Rs) - 1>(other);
        } else {
            return -_diff_look_ahead<0>(other);
        }
    }

    constexpr reference_type operator[](difference_type idx) const
        noexcept((ranges::nothrow_range<Rs> and ...))  //
        requires _is_random_access                     //
    {
        auto d = *this;
        d += idx;
        return *d;
    }

    constexpr bool operator==(const iterator& other) const noexcept
        requires(std::equality_comparable<SR::iterator_t<Rs>>and...) {
        return _var == other._var;
    }

    constexpr bool operator==(sentinel) const noexcept { return _var.index() == sizeof...(Rs); }

    constexpr auto operator<=>(const iterator& other) const  //
        noexcept(_is_nothrow_advancing)                      //
        requires(_is_random_access) {
        auto idx_off = _var.index() <=> other._var.index();
        if (not std::is_eq(idx_off)) {
            return idx_off;
        } else {
            return *this - other <=> 0;
        }
    }
};

}  // namespace _concat_detail

template <SR::view... Rs>
class concat_view : public SR::view_interface<concat_view<Rs...>> {
    using tuple_type = std::tuple<Rs...>;
    NEO_NO_UNIQUE_ADDRESS tuple_type _tuple;

    using _iterator = _concat_detail::iterator<Rs...>;
    friend _iterator;

public:
    constexpr concat_view() = default;

    constexpr concat_view(Rs... args) noexcept(noexcept(tuple_type(Rs(NEO_FWD(args))...)))
        : _tuple(Rs(NEO_FWD(args))...) {}

    constexpr auto size() const noexcept requires(SR::sized_range<Rs>and...) {
        return std::apply(
            [&](auto&&... els) {
                return (static_cast<std::common_type_t<SR::range_size_t<Rs>...>>(SR::size(els))
                        + ...);
            },
            _tuple);
    }

    constexpr auto begin() const noexcept {
        return _iterator{*this, std::in_place_index<0>, SR::begin(std::get<0>(_tuple))};
    }

    constexpr auto end() const noexcept {
        if constexpr ((SR::common_range<Rs> and ...)) {
            return _iterator{*this, std::in_place_index<sizeof...(Rs)>, _concat_detail::sentinel{}};
        } else {
            return _concat_detail::sentinel{};
        }
    }
};

template <typename... Rs>
explicit concat_view(Rs&&...) -> concat_view<SV::all_t<Rs>...>;

/**
 * @brief Construct a concat_view from the given viewable ranges.
 *
 * The returned range presents itself as the concatenation of all the
 * given ranges.
 */
inline constexpr struct concat_fn {
    template <SR::viewable_range... Rs>
    requires requires { typename std::common_reference_t<SR::range_reference_t<Rs>...>; }
    constexpr auto operator()(Rs&&... ranges) const noexcept {
        if constexpr (sizeof...(ranges) == 1) {
            return SV::all(ranges...);
        } else {
            using concat_type = concat_view<SV::all_t<Rs>...>;
            return concat_type(SV::all(NEO_FWD(ranges))...);
        }
    }
} concat;

template <SR::viewable_range... R>
using concat_t = decltype(concat(NEO_DECLVAL(R)...));

}  // namespace neo::views

#undef SR
#undef SV