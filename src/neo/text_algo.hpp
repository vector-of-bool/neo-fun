#pragma once

#include "./ranges.hpp"
#include "./reconstruct.hpp"
#include "./scalar_box.hpp"
#include "./string.hpp"
#include "./substring.hpp"
#include "./tag.hpp"
#include "./text_range.hpp"

#include <algorithm>
#include <compare>
#include <concepts>
#include <iosfwd>
#include <numeric>
#include <tuple>

namespace neo {

namespace text_algo_detail {

template <typename Into, typename... Args>
concept can_append_to = requires(Into& into, Args&&... args) { into.append(NEO_FWD(args)...); };

}  // namespace text_algo_detail

template <mutable_text_range Out, input_text_range R>
constexpr void text_append(Out& into, R&& from) noexcept(ranges::nothrow_range<R>) {
    if constexpr (text_algo_detail::can_append_to<Out, R&&>) {
        // We append R directly
        into.append(NEO_FWD(from));
        return;
    } else if constexpr (text_algo_detail::can_append_to<Out,
                                                         std::ranges::iterator_t<R>,
                                                         std::ranges::sentinel_t<R>>) {
        // We can append iterators from R directly
        into.append(std::ranges::begin(from), neo::text_range_end(from));
        return;
    } else if constexpr (std::ranges::sized_range<R> or std::ranges::random_access_range<R>) {
        // 'from' has a known size, so we can reserve space and then write it:
        auto prev_size = static_cast<std::size_t>(neo::text_range_distance(into));
        into.resize(prev_size + neo::text_range_size(from));
        auto out = std::ranges::next(std::ranges::begin(into), prev_size);
        std::ranges::copy(neo::view_text(from), out);
    } else {
        // Fallback, just copy characters individually
        std::ranges::copy(neo::view_text(from), std::back_inserter(into));
    }
}

inline constexpr struct trim_fn {
    template <text_range R, predicate<char32_t> Predicate>
    constexpr substring_t<R> operator()(R&& text, Predicate&& pred) const
        noexcept(ranges::nothrow_range<R>) {
        // Find the first non-matching
        auto new_begin = std::ranges::find_if_not(text, pred);
        // Find the last non-matching
        auto new_end = std::ranges::find_if_not(std::views::reverse(text), pred);
        // These are the new boundaries:
        return substring(text, new_begin, new_end.base());
    }

    template <text_range R>
    constexpr substring_t<R> operator()(R&& text) const noexcept(ranges::nothrow_range<R>) {
        return (*this)(text, [](char32_t c) {
            return c == ' ' or c == '\t' or c == '\r' or c == '\f' or c == '\n';
        });
    }
} trim;

namespace text_algo_detail {

namespace stdr = std::ranges;

template <text_range Joiner>
struct join_text_closure : neo::ranges::pipable {
    NEO_NO_UNIQUE_ADDRESS view_text_t<Joiner> _join;

    explicit join_text_closure(Joiner&& j) noexcept
        : _join(neo::view_text(NEO_FWD(j))) {}

    template <std::ranges::input_range R>
        requires input_text_range<std::ranges::range_reference_t<R>>
    constexpr auto operator()(R&& strings) const noexcept {
        auto acc = neo::make_empty_string_from(_join);
        if constexpr (stdr::forward_range<R>) {
            // We can reserve space for the final string without destroying the input range
            size_t strings_acc = 0;
            size_t n_strings   = 0;
            stdr::for_each(strings, [&](auto&& str) {
                strings_acc += neo::text_range_size(str);
                n_strings++;
            });
            if (n_strings) {
                strings_acc += ((n_strings - 1) * neo::text_range_size(_join));
            }
            acc.reserve(strings_acc);
        }
        auto       it   = std::ranges::begin(strings);
        const auto stop = std::ranges::end(strings);
        while (it != stop) {
            neo::text_append(acc, *it);
            ++it;
            if (it != stop) {
                neo::text_append(acc, _join);
            }
        }
        return acc;
    }
};

template <typename... Ts>
struct str_concat_types {
    using char_type = common_type_t<std::ranges::range_value_t<Ts>...>;
    using reference = common_reference_t<std::ranges::range_reference_t<Ts>...>;
};

template <>
struct str_concat_types<> {
    using char_type = char;
    using reference = const char&;
};

}  // namespace text_algo_detail

/**
 * @brief Eagerly join a range of text_ranges into a new std::string
 */
inline constexpr struct join_text_fn {
    template <text_range Joiner>
    constexpr auto operator()(Joiner&& j) const noexcept {
        return text_algo_detail::join_text_closure<Joiner>{NEO_FWD(j)};
    }

    template <std::ranges::input_range R, text_range Joiner>
        requires neo::text_range<std::ranges::range_reference_t<R>>
    constexpr auto operator()(R&& r, Joiner&& j) const noexcept {
        return (*this)(j)(r);
    }
} join_text;

namespace text_algo_detail {

template <text_range... Ts>
class str_concat_tuple {
    NEO_NO_UNIQUE_ADDRESS std::tuple<neo::scalar_box<Ts>...> _strs;

    template <std::size_t N, std::size_t Counter, typename Head, typename... Tail>
    constexpr auto const& _get_nth(neo::tag<Head, Tail...>) const noexcept {
        if constexpr (N == 0) {
            return static_cast<const Head&>(std::get<Counter>(_strs));
        } else {
            return _get_nth<N - 1, Counter + 1>(neo::tag_v<Tail...>);
        }
    }

    template <std::size_t N>
    constexpr auto const& _nth() const noexcept {
        return std::get<N>(_strs).get();
    }

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using char_type       = typename text_algo_detail::str_concat_types<Ts...>::char_type;
    using reference       = typename text_algo_detail::str_concat_types<Ts...>::reference;

public:
    constexpr str_concat_tuple()
        requires((default_initializable<Ts> and ...))
    = default;

    constexpr explicit str_concat_tuple(Ts&&... strings) noexcept
        : _strs(NEO_FWD(strings)...) {}

    constexpr size_type size() const noexcept
        requires(std::ranges::sized_range<Ts> and ...)
    {
        return std::
            apply([](auto&&... elems) { return (neo::text_range_size(elems.get()) + ... + 0ull); },
                  _strs);
    }

    constexpr auto data() const noexcept
        requires(sizeof...(Ts) == 1 and (std::ranges::contiguous_range<Ts> and ...))
    {
        return std::ranges::data(_nth<0>());
    }

    class iterator : public neo::iterator_facade<iterator> {
        str_concat_tuple const* _self      = nullptr;
        size_type               _index     = 0;
        size_type               _inner_idx = 0;

    public:
        constexpr iterator() = default;

    private:
        constexpr explicit iterator(str_concat_tuple const& self, size_type idx) noexcept
            : _self(&self)
            , _index{idx} {}

        friend str_concat_tuple;

        template <size_type N>
        constexpr const auto& _nth() const noexcept {
            return _self->template _nth<N>();
        }

        template <size_type N>
        constexpr size_type _nth_size(size_type idx) const noexcept {
            if constexpr (N == sizeof...(Ts)) {
                neo_assert(expects,
                           idx == N,
                           "Access beyond the end of str_concat_range",
                           idx,
                           N,
                           _index,
                           _inner_idx);
                return 0;
            } else {
                if (idx == N) {
                    return neo::text_range_size(_nth<N>());
                }
                return _nth_size<N + 1>(idx);
            }
        }

        constexpr auto _nth_size(size_type idx) const noexcept { return _nth_size<0>(idx); }

        constexpr auto _remaining_in_cur_subrange() const noexcept {
            const auto cur_size = _nth_size(_index);
            return cur_size - _inner_idx;
        }

        template <std::size_t N>
        constexpr reference _deref() const {
            if constexpr (N == sizeof...(Ts)) {
                std::terminate();
            } else {
                if (N == _index) {
                    return *(std::ranges::next(std::ranges::begin(_nth<N>()), _inner_idx));
                } else {
                    return _deref<N + 1>();
                }
            }
        }

        constexpr void _advance(std::size_t off) {
            while (off >= _remaining_in_cur_subrange() and _index < sizeof...(Ts)) {
                off -= _remaining_in_cur_subrange();
                ++_index;
                _inner_idx = 0;
            }
            neo_assert(expects,
                       _index < sizeof...(Ts) or off == 0,
                       "Advance past-the-end of str_concat_range",
                       off,
                       _index,
                       _inner_idx);
            _inner_idx += off;
        }

        constexpr void _rewind(std::size_t off) noexcept {
            while (off >= _inner_idx and _index > 0) {
                off -= _inner_idx;
                --_index;
                _inner_idx = _nth_size(_index);
            }
            neo_assert(expects,
                       _inner_idx > 0 or off == 0,
                       "Rewind before-the-begin of str_concat_range",
                       off,
                       _index,
                       _inner_idx);
            _inner_idx -= off;
        }

        constexpr std::ptrdiff_t _dist_fwd(const iterator& other) const {
            neo_assert(invariant, _index < other._index, "Invalid dist-fwd");
            auto           idx = _index;
            std::ptrdiff_t off = _remaining_in_cur_subrange();
            ++idx;
            while (idx < other._index) {
                off += _nth_size(idx);
                ++idx;
            }
            off += other._inner_idx;
            return off;
        }

        constexpr std::ptrdiff_t _dist_bwd(const iterator& other) const noexcept {
            neo_assert(invariant, _index > other._index, "Invalid dist-bwd");
            auto           idx = _index;
            std::ptrdiff_t off = _inner_idx;
            --idx;
            while (idx > other._index) {
                off += _nth_size(idx);
                --idx;
            }
            off += other._remaining_in_cur_subrange();
            return -off;
        }

    public:
        constexpr void advance(std::ptrdiff_t off) noexcept((ranges::nothrow_range<Ts> and ...)) {
            if (off > 0) {
                _advance(off);
            } else if (off < 0) {
                _rewind(static_cast<std::size_t>(-off));
            }
        }

        constexpr std::ptrdiff_t distance_to(iterator const& other) const
            noexcept((ranges::nothrow_range<Ts> and ...)) {
            if (_index > other._index) {
                return _dist_bwd(other);
            } else if (_index < other._index) {
                return _dist_fwd(other);
            } else {
                return other._inner_idx - _inner_idx;
            }
        }

        constexpr reference dereference() const noexcept((ranges::nothrow_range<Ts> and ...)) {
            return _deref<0>();
        }

        constexpr bool operator==(iterator other) const noexcept {
            return _index == other._index and _inner_idx == other._inner_idx;
        }

        constexpr auto to_address() const noexcept
            requires(sizeof...(Ts) == 1 and (std::ranges::contiguous_range<Ts> and ...))
        {
            return std::addressof(dereference());
        }
    };

    constexpr auto begin() const noexcept((ranges::nothrow_range<Ts> and ...)) {
        return iterator{*this, 0};
    }
    constexpr auto end() const noexcept((ranges::nothrow_range<Ts> and ...)) {
        return iterator{*this, sizeof...(Ts)};
    }

    constexpr reference operator[](size_type off) const
        noexcept((ranges::nothrow_range<Ts> and ...))
        requires(random_access_text_range<Ts> and ...)
    {
        return begin()[off];
    }

    template <mutable_text_range S>
    constexpr explicit operator S() const noexcept((ranges::nothrow_range<Ts> and ...)) {
        S ret;
        ret.resize(size());
        std::ranges::copy(*this, std::ranges::begin(ret));
        return ret;
    }
};

template <text_range... Ts>
explicit str_concat_tuple(Ts&&...) -> str_concat_tuple<Ts...>;

}  // namespace text_algo_detail

inline constexpr struct str_concat_fn {
    template <text_range... Strings>
    constexpr auto operator()(Strings&&... strings) const noexcept {
        return text_algo_detail::str_concat_tuple{NEO_FWD(strings)...};
    }
} str_concat;

template <text_range... Ts>
using str_concat_t = decltype(neo::str_concat(NEO_DECLVAL(Ts)...));

inline constexpr struct starts_with_fn {
    template <forward_iterator                                Left,
              forward_iterator                                Right,
              std::indirect_equivalence_relation<Left, Right> Compare = std::equal_to<>>
    constexpr bool operator()(Left                           left,
                              sentinel_for<Left> auto const  left_end,
                              Right                          right,
                              sentinel_for<Right> auto const right_end,
                              Compare&&                      compare = {}) const noexcept {
        for (; left != left_end and right != right_end; ++left, ++right) {
            if (not compare(*left, *right)) {
                return false;
            }
        }
        if (right == right_end) {
            // We read the entire needle and it compared equal
            return true;
        }
        return false;
    }

    template <std::ranges::forward_range                                  Left,
              std::ranges::forward_range                                  Right,
              equivalence_relation<std::ranges::range_reference_t<Left>,
                                   std::ranges::range_reference_t<Right>> Compare
              = std::equal_to<>>
    constexpr bool operator()(Left&& left, Right&& right, Compare compare = {}) const noexcept {
        auto lit  = std::ranges::begin(left);
        auto rit  = std::ranges::begin(right);
        auto lend = std::ranges::end(left);
        auto rend = std::ranges::end(right);
        return (*this)(lit, lend, rit, rend, compare);
    }

    template <std::ranges::forward_range Left, forward_iterator RightIter>
    constexpr bool operator()(Left&& left, RightIter it, sentinel_for<RightIter> auto const stop) {
        return (
            *this)(std::ranges::begin(left), std::ranges::end(left), it, stop, std::equal_to<>{});
    }

    template <std::forward_iterator LeftIter, std::ranges::forward_range Right>
    constexpr bool operator()(LeftIter                          left,
                              sentinel_for<LeftIter> auto const stop,
                              Right&&                           right) const noexcept {
        return (*this)(left, stop, std::ranges::begin(right), std::ranges::end(right));
    }
} starts_with;

template <text_range R>
struct text_range_formatter {
    neo::scalar_box<R> _r;

    text_range_formatter(R&& r) noexcept
        : _r(NEO_FWD(r)) {}

    inline friend void
    ufmt_append(std::string&                out,
                text_range_formatter const& self) noexcept(ranges::nothrow_range<R>) {
        text_append(out, self._r.get());
    }
};

template <text_range R>
explicit text_range_formatter(R&&) -> text_range_formatter<R>;

template <typename CharEqual = std::ranges::equal_to>
struct text_range_equal_to {
    NEO_NO_UNIQUE_ADDRESS CharEqual _equal;

    template <forward_text_range L,
              forward_text_range R,
              typename Lv = view_text_t<L&>,
              typename Rv = view_text_t<R&>>
        requires neo::invocable2<CharEqual,
                                 std::ranges::range_reference_t<Lv>,
                                 std::ranges::range_reference_t<Rv>>
    constexpr inline bool operator()(L&& left, R&& right) const
        noexcept(ranges::nothrow_range<L>and ranges::nothrow_range<R>) {
        return std::ranges::equal(neo::view_text(left), neo::view_text(right), _equal);
    }
};

template <typename Compare = std::compare_three_way>
struct text_range_compare_3way {
    NEO_NO_UNIQUE_ADDRESS Compare _compare;

    template <forward_text_range L,
              forward_text_range R,
              typename Lv = view_text_t<L>,
              typename Rv = view_text_t<R>,
              typename Lc = text_char_t<Lv>,
              typename Rc = text_char_t<Rv>>
        requires neo::invocable2<Compare, Lc, Rc>
    constexpr neo::invoke_result_t<Compare, Lc, Rc> operator()(L&& left, R&& right) const
        noexcept(ranges::nothrow_range<L>and ranges::nothrow_range<R>) {
        const auto vl   = neo::view_text(left);
        const auto vr   = neo::view_text(right);
        auto       lit  = std::ranges::begin(vl);
        auto       rit  = std::ranges::begin(vr);
        const auto lend = std::ranges::end(vl);
        const auto rend = std::ranges::end(vr);
        while (lit != lend and rit != rend) {
            auto c = _compare(*lit, *rit);
            if (std::is_neq(c)) {
                return c;
            }
        }
        // We ran to the end of one of the ranges
        if (lit == lend) {
            if (rit == rend) {
                // We hit the end of both
                return std::strong_ordering::equal;
            } else {
                // left is shorter than right, and thus less-than
                return std::strong_ordering::less;
            }
        } else {
            // left is longer than right, and thus greater-than
            return std::strong_ordering::greater;
        }
    }
};

template <forward_text_range R>
struct text_range_ostream_inserter {
    NEO_NO_UNIQUE_ADDRESS neo::scalar_box<R> _range;

    constexpr explicit text_range_ostream_inserter(R&& r) noexcept
        : _range(NEO_FWD(r)) {}

    template <typename Os, typename View = view_text_t<R>>
    constexpr friend Os&
    operator<<(Os& out, text_range_ostream_inserter&& self) noexcept(ranges::nothrow_range<R>)
        requires requires(std::add_pointer_t<std::ranges::range_value_t<View>> sptr,
                          std::ranges::range_size_t<View>                      size) {
                     out.put(*std::ranges::begin(self._range.get()));
                     out.write(sptr, size);
                 }
    {
        auto view = neo::view_text(self._range.get());
        if constexpr (contiguous_text_range<View>) {
            out.write(std::ranges::data(view), std::ranges::size(view));
        } else {
            for (auto&& c : view) {
                out.put(c);
            }
        }
        return out;
    }
};

template <forward_text_range R>
explicit text_range_ostream_inserter(R&&) -> text_range_ostream_inserter<R>;

}  // namespace neo

namespace std::ranges {

template <typename... Ts>
constexpr bool enable_borrowed_range<neo::text_algo_detail::str_concat_tuple<Ts...>>
    = (std::ranges::borrowed_range<Ts> and ...);

template <typename... Ts>
constexpr bool enable_view<neo::text_algo_detail::str_concat_tuple<Ts...>>
    = (std::ranges::view<Ts> and ...);

}  // namespace std::ranges
