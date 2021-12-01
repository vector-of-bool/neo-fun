#pragma once

#if __cpp_nontype_template_args < 201911L && (__GNUC__ < 10) && (__clang_major__ < 11)             \
    && (_MSC_VER < 1926)

#error "<neo/fixed_string.hpp> requires C++20 class types as non-type template parameters"

#else

#include <neo/fwd.hpp>

#include <algorithm>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <string_view>

namespace neo {

/**
 * @brief A string with a compile-time fixed length.
 *
 * This type is entirely constexpr, and is supported as a non-type template parameter
 *
 * @tparam Char The character type of the string
 * @tparam Size The length of the string (not including a null terminator)
 */
template <typename Char, std::size_t Size>
class basic_fixed_string {
public:
    /// The character type of the string
    using value_type = Char;
    /// The unsigned type that refers to the string's size
    using size_type = std::size_t;
    /// The size of the string, in value_type
    static constexpr size_type size() noexcept { return Size; }
    /// Sentinel value for the no-position value
    static constexpr auto npos = std::string_view::npos;

    /// The basic_string_view type that can reference this fixed string
    using string_view_type = std::basic_string_view<Char>;

    /// The actual character array. Do not reference this in the public API
    value_type _p_chars[size() + 1] = {};

    /// The iterator type of this string
    using iterator = value_type*;
    /// The const-iterator type of this string
    using const_iterator = const value_type*;
    /// The data pointer for this string
    using pointer = value_type*;
    /// The pointer-to-const-data for this string
    using const_pointer = const value_type*;
    /// Reference type for the string
    using reference = value_type&;
    /// Reference-to-const for the string
    using const_reference = const value_type&;

    /// Default-construct the string with all zeros
    constexpr basic_fixed_string() = default;
    /// Copy the string
    constexpr basic_fixed_string(const basic_fixed_string&) = default;

    /**
     * @brief Construct from a range of characters
     * @pre The size of the range MUST be exactly `size()`
     */
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, value_type>  //
    constexpr basic_fixed_string(R&& r)
        : basic_fixed_string(std::ranges::begin(r), std::ranges::end(r)) {}

    /// Construct the string from an array of the appropriate size (Presumable a string literal)
    constexpr basic_fixed_string(const value_type (&arr)[Size + 1])
        : basic_fixed_string(arr, arr + Size) {}

    /// Initialize the string with the given range defined by [first, last)
    template <std::input_iterator I, std::sentinel_for<I> S>
    requires std::convertible_to<std::iter_value_t<I>, value_type>  //
    constexpr basic_fixed_string(I first, S last) { assign(first, last); }

    /// Convert to a string_view implicitly
    constexpr operator string_view_type() const noexcept {
        return string_view_type{data(), size()};
    }

    /// Pointer-to-data for this tsring
    [[nodiscard]] constexpr pointer       data() noexcept { return _p_chars; }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return _p_chars; }
    /// Check if the string is empty
    [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
    /// Begining iterator for this string
    [[nodiscard]] constexpr iterator       begin() noexcept { return _p_chars; }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return _p_chars; }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return _p_chars.cbegin(); }
    /// End iterator for this string
    [[nodiscard]] constexpr iterator       end() noexcept { return begin() + size(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return begin() + size(); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return cbegin() + size(); }

    /// Access the Nth character of the string
    [[nodiscard]] constexpr reference       operator[](size_type n) noexcept { return _p_chars[n]; }
    [[nodiscard]] constexpr const_reference operator[](size_type n) const noexcept {
        return _p_chars[n];
    }

    /**
     * @brief Get a substring of this string
     *
     * @tparam Pos The starting index of the string
     * @tparam Len The length of the new string. Clamped to the max possible length
     * @return The new string
     */
    template <size_type Pos, size_type Len = npos>
    [[nodiscard]] constexpr auto substr() const noexcept {
        static_assert(Pos <= size());
        constexpr auto new_size = (std::min)(size() - Pos, Len);
        using substring_type    = basic_fixed_string<value_type, new_size>;
        substring_type ret;
        auto           sub = string_view_type{*this}.substr(Pos, new_size);
        ret.assign(sub);
        return ret;
    }

    /**
     * @brief Replace the contents of this string with the contents of the given range. The
     * range view must have the same length as this string.
     */
    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>  //
    constexpr void assign(R&& r) noexcept { assign(std::ranges::cbegin(r), std::ranges::cend(r)); }

    /**
     * @brief Replace the contents of the string with the given range
     */
    template <std::input_iterator I, std::sentinel_for<I> S>
    constexpr void assign(I it, S stop) noexcept {
        if constexpr (std::forward_iterator<I>) {
            assert(static_cast<size_type>(std::distance(it, stop)) == size());
        }
        std::copy(it, stop, begin());
    }

    /**
     * @brief Obtain a std::basic_string_view that views this string.
     */
    constexpr string_view_type string_view() const noexcept { return *this; }

    /**
     * @brief Compare two basic_fixed_strings
     */
    template <size_type S>
    friend constexpr bool operator==(const basic_fixed_string&                left,
                                     const basic_fixed_string<value_type, S>& right) noexcept {
        return string_view_type{left} == string_view_type{right};
    }

    /**
     * @brief Compare the basic_fixed_string with a string literal
     */
    template <size_type S>
    friend constexpr bool operator==(const basic_fixed_string& left,
                                     const value_type (&arr)[S]) noexcept {
        return left == basic_fixed_string<Char, S - 1>{arr};
    }

    /**
     * @brief Ordering between basic_fixed_strings
     */
    template <size_type S>
    friend constexpr auto operator<=>(const basic_fixed_string&                left,
                                      const basic_fixed_string<value_type, S>& right) noexcept {
        return string_view_type{left} <=> string_view_type{right};
    }

    /**
     * @brief Ordering between basic_fixed_strings and string literals
     */
    template <size_type S>
    friend constexpr auto operator<=>(const basic_fixed_string& left,
                                      const value_type (&right)[S]) noexcept {
        return left <=> basic_fixed_string<Char, S - 1>{right};
    }

    /**
     * @brief Concatenate two basic_fixed_strings
     */
    template <size_type S>
    friend constexpr auto operator+(const basic_fixed_string&                left,
                                    const basic_fixed_string<value_type, S>& right) noexcept {
        constexpr size_type new_size = std::remove_cvref_t<decltype(left)>::size()
            + std::remove_cvref_t<decltype(right)>::size();
        using ret_type = basic_fixed_string<value_type, new_size>;
        ret_type ret;
        auto     out = ret.begin();
        out          = std::copy(left.begin(), left.end(), out);
        std::copy(right.begin(), right.end(), out);
        return ret;
    }

    /**
     * @brief Concatenate a basic_fixed_string with a string literal
     */
    template <size_type N>
    friend constexpr auto operator+(const basic_fixed_string& left,
                                    const value_type (&right)[N]) noexcept {
        auto r = neo::basic_fixed_string{right};
        return left + r;
    }

    /**
     * @brief Concatenate a basic_fixed_string with a string literal
     */
    template <size_type N>
    friend constexpr auto operator+(const value_type (&left)[N],
                                    const basic_fixed_string& right) noexcept {
        auto l = neo::basic_fixed_string{left};
        return l + right;
    }

    friend constexpr void do_repr(auto out, basic_fixed_string const* self) noexcept {
        if constexpr (out.just_type) {
            if constexpr (std::same_as<Char, char>) {
                out.append("neo::fixed_string<{}>", out.repr_value(Size));
            } else {
                out.append("neo::basic_fixed_string<{}, {}>",
                           out.template repr_type<Char>(),
                           out.value(Size));
            }
        } else if constexpr (out.just_value) {
            out.append("{}", out.repr_value(self->string_view()));
        } else {
            if constexpr (std::same_as<Char, char>) {
                out.append("neo::fixed_string{{}}", out.repr_value(*self));
            } else {
                out.append("neo::basic_fixed_string{{}}", out.repr_value(*self));
            }
        }
    }
};

template <typename Char, std::size_t N>
basic_fixed_string(const Char (&arr)[N]) -> basic_fixed_string<Char, N - 1>;

template <typename Char, std::size_t N>
basic_fixed_string(const basic_fixed_string<Char, N>&) -> basic_fixed_string<Char, N>;

template <std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

/**
 * @brief Define a type that is parameterized by a constexpr basic_fixed_string.
 */
template <basic_fixed_string S>
struct tstring {
    /// The fixed-length constexpr string
    constexpr static auto& string = S;

    /// Get a string_view for the string of this tstring
    constexpr static auto string_view() noexcept { return string.string_view(); }

    constexpr friend void do_repr(auto out, const tstring*) noexcept {
        if constexpr (out.just_value) {
            out.append("{}", out.repr_value(tstring::string_view()));
        } else {
            out.append("neo::tstring<{}>", out.repr_value(tstring::string_view()));
        }
    }
};

/**
 * @brief A constexpr NTTP-value string_view into a compile-time string referred to by a `tstring`
 *
 * @tparam T The `tstring` that wrapps the constexpr string that we are viewing
 */
template <typename T>
struct tstring_view {
    /// The `tstring` that wrapps the string that we view
    using tstring_type = T;

    using fixed_string_type = std::remove_cvref_t<decltype(tstring_type::string)>;
    using size_type         = typename fixed_string_type::size_type;
    using value_type        = typename fixed_string_type::value_type;
    using string_view_type  = typename fixed_string_type::string_view_type;

    using pointer       = typename fixed_string_type::pointer;
    using const_pointer = typename fixed_string_type::const_pointer;

    size_type _position = 0;
    size_type _length   = T::string.size();

    static constexpr auto npos = tstring_type::string.npos;

    /// Convert the tstring_view into a coresponding std::basic_string_view
    constexpr string_view_type string_view() const noexcept {
        return tstring_type::string_view().substr(inner_position(), length());
    }

    /// Convert the tstring_view into a coresponding std::basic_string_view
    constexpr operator string_view_type() const noexcept { return string_view(); }

    constexpr size_type inner_position() const noexcept { return _position; }

    constexpr auto begin() const noexcept { return string_view().begin(); }
    constexpr auto cbegin() const noexcept { return string_view().cbegin(); }
    constexpr auto rbegin() const noexcept { return string_view().rbegin(); }
    constexpr auto crbegin() const noexcept { return string_view().crbegin(); }
    constexpr auto end() const noexcept { return string_view().end(); }
    constexpr auto cend() const noexcept { return string_view().cend(); }
    constexpr auto rend() const noexcept { return string_view().rend(); }
    constexpr auto crend() const noexcept { return string_view().crend(); }

    constexpr auto operator[](size_type off) const noexcept { return string_view()[off]; }
    constexpr auto front() const noexcept { return string_view().front(); }
    constexpr auto back() const noexcept { return string_view().back(); }
    constexpr auto data() const noexcept { return string_view().data(); }

    constexpr auto size() const noexcept { return _length; }
    constexpr auto length() const noexcept { return _length; }
    constexpr auto empty() const noexcept { return size() == 0; }

    /// Remove the first "off" characters from the view
    constexpr auto remove_prefix(size_type off) noexcept { *this = substr(off); }
    /// Remove the last "off" characters from the view
    constexpr auto remove_suffix(size_type off) noexcept { *this = substr(0, size() - off); }

    /// Create a new view beginning at `pos` characters into this view
    constexpr tstring_view substr(size_type pos) const noexcept { return substr(pos, npos); }

    /// Create a new view beginning at `pos` characters into this view, with length `len` characters
    constexpr tstring_view substr(size_type pos, size_type len) const noexcept {
        const auto remain_len = _length - pos;
        const auto min_len    = (std::min)(remain_len, len);
        return tstring_view{_position + pos, min_len};
    }

#define DEF_WRAP(Name, Qual)                                                                       \
    template <typename... Args>                                                                    \
    constexpr decltype(auto) Name(Args&&... args)                                                  \
        Qual noexcept(noexcept(this->string_view().Name(NEO_FWD(args)...))) requires requires {    \
        this->string_view().Name(NEO_FWD(args)...);                                                \
    }                                                                                              \
    { return string_view().Name(NEO_FWD(args)...); }                                               \
    static_assert(true)

    DEF_WRAP(compare, const);
    DEF_WRAP(starts_with, const);
    DEF_WRAP(ends_with, const);
    DEF_WRAP(find, const);
    DEF_WRAP(find_first_of, const);
    DEF_WRAP(find_last_of, const);
    DEF_WRAP(find_first_not_of, const);
    DEF_WRAP(find_last_not_of, const);
#undef DEF_WRAP

    /**
     * @brief Compare two basic_fixed_strings
     */
    template <typename O>
    friend constexpr bool operator==(const tstring_view&    left,
                                     const tstring_view<O>& right) noexcept {
        return string_view_type{left} == string_view_type{right};
    }

    /**
     * @brief Compare the basic_fixed_string with a string literal
     */
    friend constexpr bool operator==(const tstring_view& left, string_view_type other) noexcept {
        return string_view_type{left} == other;
    }

    /**
     * @brief Ordering between basic_fixed_strings
     */
    template <typename O>
    friend constexpr auto operator<=>(const tstring_view&    left,
                                      const tstring_view<O>& right) noexcept {
        return left <=> string_view_type{right};
    }

    /**
     * @brief Ordering between basic_fixed_strings and string literals
     */
    friend constexpr auto operator<=>(const tstring_view& left_, string_view_type right) noexcept {
        // Could just be string_view::operator<=>, but MSVC doesn't like that (yet)
        auto left = string_view_type{left_};
        auto comp = left.compare(right);
        if (comp < 0) {
            return std::strong_ordering::less;
        } else if (comp > 0) {
            return std::strong_ordering::greater;
        } else {
            return std::strong_ordering::equal;
        }
    }
};

template <typename TS>
constexpr tstring_view<TS> make_tstring_view{};

template <basic_fixed_string S>
constexpr auto tstring_view_v
    = make_tstring_view<tstring<basic_fixed_string<typename decltype(S)::value_type, S.size()>(S)>>;

}  // namespace neo

#endif
