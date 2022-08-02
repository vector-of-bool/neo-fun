#pragma once

#include "./ranges.hpp"
#include "./reconstruct.hpp"
#include "./ref_member.hpp"
#include "./string.hpp"
#include "./substring.hpp"
#include "./tag.hpp"
#include "./text_range.hpp"

#include <algorithm>
#include <concepts>
#include <iosfwd>
#include <numeric>
#include <tuple>

namespace neo {

inline constexpr struct trim_fn {
    template <text_range R, std::predicate<char32_t> Predicate>
    constexpr text_range auto operator()(R&& text, Predicate&& pred) const noexcept {
        auto new_begin = std::ranges::find_if_not(text, pred);
        auto new_end   = std::ranges::find_if_not(std::views::reverse(text), pred);
        return substring(text, new_begin, new_end.base());
    }

    template <text_range R>
    constexpr text_range auto operator()(R&& text) const noexcept {
        return (*this)(text, [](char32_t c) {
            return c == ' ' or c == '\t' or c == '\r' or c == '\f' or c == '\n';
        });
    }
} trim;

namespace text_algo_detail {

namespace stdr = std::ranges;

template <text_range Joiner>
struct join_text_closure : neo::ranges::pipable {
    Joiner _join;

    explicit join_text_closure(Joiner&& j) noexcept
        : _join(NEO_FWD(j)) {}

    template <std::ranges::input_range R>
    requires text_range<std::ranges::range_reference_t<R>>
    constexpr auto operator()(R&& strings) const noexcept {
        using RetType = neo::copy_text_t<std::ranges::range_reference_t<R>>;
        RetType acc   = neo::make_empty_string_from(_join);
        if constexpr (stdr::forward_range<R>) {
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
            acc.append(neo::view_text(*it));
            ++it;
            if (it != stop) {
                acc.append(neo::view_text(_join));
            }
        }
        return acc;
    }
};

template <typename... Ts>
struct str_concat_types {
    using char_type = std::common_type_t<std::ranges::range_value_t<Ts>...>;
    using reference = std::common_reference_t<std::ranges::range_reference_t<Ts>...>;
};

template <>
struct str_concat_types<> {
    using char_type = char;
    using reference = const char&;
};

}  // namespace text_algo_detail

inline constexpr struct join_text_fn {
    template <text_range Joiner>
    constexpr auto operator()(Joiner&& j) const noexcept {
        return text_algo_detail::join_text_closure<Joiner>{NEO_FWD(j)};
    }

    template <std::ranges::input_range R, text_range Joiner>
    requires neo::text_range<std::ranges::range_reference_t<R>>
    constexpr auto operator()(R&& r, Joiner&& j) const noexcept { return (*this)(j)(r); }
} join_text;

template <text_range... Ts>
class str_concat_tuple {
    std::tuple<neo::wrap_ref_member_t<Ts>...> _strs;

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
        return neo::unref_member(std::get<N>(_strs));
    }

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using char_type       = typename text_algo_detail::str_concat_types<Ts...>::char_type;
    using reference       = typename text_algo_detail::str_concat_types<Ts...>::reference;

public:
    constexpr str_concat_tuple() requires((std::is_default_constructible_v<Ts> and ...))  //
        = default;

    constexpr explicit str_concat_tuple(Ts&&... strings) noexcept
        : _strs(NEO_FWD(strings)...) {}

    constexpr size_type size() const noexcept {
        return std::apply(
            [this](auto&&... elems) {
                return (neo::text_range_size(neo::unref_member(elems)) + ... + 0ull);
            },
            _strs);
    }

    constexpr auto data() const noexcept
        requires(sizeof...(Ts) == 1 and (std::ranges::contiguous_range<Ts> and ...)) {
        std::ranges::data(_nth<0>());
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
        constexpr reference _deref() const noexcept {
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

        constexpr void _advance(std::size_t off) noexcept {
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

        constexpr std::ptrdiff_t _dist_fwd(const iterator& other) const noexcept {
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
        constexpr void advance(std::ptrdiff_t off) noexcept {
            if (off > 0) {
                _advance(off);
            } else if (off < 0) {
                _rewind(static_cast<std::size_t>(-off));
            }
        }

        constexpr std::ptrdiff_t distance_to(iterator const& other) const noexcept {
            if (_index > other._index) {
                return _dist_bwd(other);
            } else if (_index < other._index) {
                return _dist_fwd(other);
            } else {
                return other._inner_idx - _inner_idx;
            }
        }

        constexpr reference dereference() const noexcept { return _deref<0>(); }

        constexpr bool operator==(iterator other) const noexcept {
            return _index == other._index and _inner_idx == other._inner_idx;
        }

        constexpr auto to_address() const noexcept
            requires(sizeof...(Ts) == 1 and (std::ranges::contiguous_range<Ts> and ...)) {
            return std::addressof(dereference());
        }
    };

    constexpr auto begin() const noexcept { return iterator{*this, 0}; }
    constexpr auto end() const noexcept { return iterator{*this, sizeof...(Ts)}; }

    constexpr reference operator[](size_type off) const noexcept { return begin()[off]; }

    template <typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& out,
                                                        str_concat_tuple const& self) noexcept {
        std::apply([&](auto&&... elems) { ((out << neo::unref_member(elems)), ...); }, self._strs);
        return out;
    }

    constexpr bool operator==(text_range auto&& other) const noexcept {
        auto size = this->size();
        return size == neo::text_range_size(other)
            and std::ranges::equal(*this,
                                   std::ranges::subrange(std::ranges::begin(other),
                                                         std::ranges::begin(other)
                                                             + neo::text_range_size(other)));
    }

    constexpr auto operator<=>(text_range auto&& other) const noexcept {
        auto ob = std::ranges::begin(other);
        return std::lexicographical_compare_three_way(  //
            begin(),
            end(),
            ob,
            ob + neo::text_range_size(other));
    }

    template <mutable_text_range S>
    constexpr explicit operator S() const noexcept {
        S ret;
        ret.resize(size());
        std::ranges::copy(*this, std::ranges::begin(ret));
        return ret;
    }
};

struct empty_string : str_concat_tuple<> {
    constexpr explicit empty_string() noexcept = default;
};

template <text_range... Ts>
explicit str_concat_tuple(Ts&&...) -> str_concat_tuple<Ts...>;

inline constexpr struct str_concat_fn {
    template <text_range... Strings>
    constexpr auto operator()(Strings&&... strings) const noexcept {
        if constexpr (sizeof...(Strings) == 0) {
            return empty_string{};
        } else {
            return str_concat_tuple{NEO_FWD(strings)...};
        }
    }
} str_concat;

inline constexpr struct starts_with_fn {
    template <std::forward_iterator                           Left,
              std::forward_iterator                           Right,
              std::indirect_equivalence_relation<Left, Right> Compare = std::equal_to<>>
    constexpr bool operator()(Left                                left,
                              std::sentinel_for<Left> auto const  left_end,
                              Right                               right,
                              std::sentinel_for<Right> auto const right_end,
                              Compare&&                           compare = {}) const noexcept {
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

    template <std::ranges::forward_range                                       Left,
              std::ranges::forward_range                                       Right,
              std::equivalence_relation<std::ranges::range_reference_t<Left>,
                                        std::ranges::range_reference_t<Right>> Compare
              = std::equal_to<>>
    constexpr bool operator()(Left&& left, Right&& right, Compare compare = {}) const noexcept {
        auto lit  = std::ranges::begin(left);
        auto rit  = std::ranges::begin(right);
        auto lend = std::ranges::end(left);
        auto rend = std::ranges::end(right);
        return (*this)(lit, lend, rit, rend, compare);
    }

    // template <std::ranges::forward_range Left, std::ranges::forward_range Right>
    // constexpr bool operator()(Left&& left, Right&& right) const noexcept {
    //     return (*this)(left, right, std::equal_to<>{});
    // }

    template <std::ranges::forward_range Left, std::forward_iterator RightIter>
    constexpr bool
    operator()(Left&& left, RightIter it, std::sentinel_for<RightIter> auto const stop) {
        return (
            *this)(std::ranges::begin(left), std::ranges::end(left), it, stop, std::equal_to<>{});
    }

    template <std::forward_iterator LeftIter, std::ranges::forward_range Right>
    constexpr bool operator()(LeftIter                               left,
                              std::sentinel_for<LeftIter> auto const stop,
                              Right&&                                right) const noexcept {
        return (*this)(left, stop, std::ranges::begin(right), std::ranges::end(right));
    }
} starts_with;
}  // namespace neo

namespace std::ranges {

template <typename... Ts>
constexpr bool
    enable_borrowed_range<neo::str_concat_tuple<Ts...>> = (std::ranges::borrowed_range<Ts> and ...);

template <typename... Ts>
constexpr bool enable_view<neo::str_concat_tuple<Ts...>> = (std::ranges::view<Ts> and ...);

template <>
constexpr inline bool enable_borrowed_range<neo::empty_string> = true;

template <>
constexpr bool enable_view<neo::empty_string> = true;

}  // namespace std::ranges
