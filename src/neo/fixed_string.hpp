#pragma once

#if __cpp_nontype_template_args < 201911L && (__GNUC__ < 10)

#error "<neo/fixed_string.hpp> requires C++20 class types as non-type template parameters"

#else

#include <neo/value.hpp>

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <string_view>

namespace neo {

template <typename Char, std::size_t Size>
class basic_fixed_string;

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
    /// The size of the string, in value_type
    static constexpr std::size_t size() noexcept { return Size; }
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

    /// Construct the string from an array of the appropriate size
    constexpr basic_fixed_string(const value_type (&arr)[Size + 1])
        : basic_fixed_string(arr, arr + Size) {}

    /// Initialize the string with the given range defined by [first, last)
    template <std::input_iterator I, std::sentinel_for<I> S>
    constexpr basic_fixed_string(I first, S last) {
        assign(first, last);
    }

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
    [[nodiscard]] constexpr reference operator[](std::size_t n) noexcept { return _p_chars[n]; }
    [[nodiscard]] constexpr const_reference operator[](std::size_t n) const noexcept {
        return _p_chars[n];
    }

    /**
     * @brief Get a substring of this string
     *
     * @tparam Pos The starting index of the string
     * @tparam Len The length of the new string. Clamped to the max possible length
     * @return The new string
     */
    template <auto Pos, auto Len>
    [[nodiscard]] constexpr auto substr(value<Pos>, value<Len>) const noexcept {
        return substr<Pos, Len>();
    }

    template <auto Pos>
    [[nodiscard]] constexpr auto substr(value<Pos>) const noexcept {
        return substr<Pos>();
    }

    template <std::size_t Pos, std::size_t Len = npos>
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
     * @brief Replace the contents of this string with the contents of the given string view. The
     * string view must have the same length as this string.
     *
     * @param str The string to assign into this string
     */
    constexpr void assign(string_view_type str) noexcept { assign(str.begin(), str.end()); }

    /**
     * @brief Replace the contents of the string with the given range
     */
    template <std::input_iterator I, std::sentinel_for<I> S>
    constexpr void assign(I it, S stop) noexcept {
        std::ranges::copy(it, stop, begin());
    }

    /**
     * @brief Replace the contents of the string with the given range
     */
    template <std::forward_iterator I, std::sentinel_for<I> S>
    constexpr void assign(I it, S stop) noexcept {
        assert(std::ranges::distance(it, stop) == size());
        std::ranges::copy(it, stop, begin());
    }

    constexpr string_view_type view() const noexcept { return *this; }

    template <std::size_t S>
    friend constexpr bool operator==(const basic_fixed_string&                left,
                                     const basic_fixed_string<value_type, S>& right) noexcept {
        return string_view_type{left} == string_view_type{right};
    }

    template <std::size_t S>
    friend constexpr bool operator==(const basic_fixed_string& left,
                                     const value_type (&arr)[S]) noexcept {
        return left == basic_fixed_string<Char, S - 1>{arr};
    }

    template <std::size_t S>
    friend constexpr auto operator<=>(const basic_fixed_string&                left,
                                      const basic_fixed_string<value_type, S>& right) noexcept {
        return string_view_type{left} <=> string_view_type{right};
    }

    template <std::size_t S>
    friend constexpr auto operator<=>(const basic_fixed_string& left,
                                      const value_type (&right)[S]) noexcept {
        return left <=> basic_fixed_string<Char, S - 1>{right};
    }

    template <std::size_t S>
    friend constexpr auto operator+(const basic_fixed_string&                left,
                                    const basic_fixed_string<value_type, S>& right) noexcept {
        constexpr std::size_t new_size = left.size() + right.size();
        using ret_type                 = basic_fixed_string<value_type, new_size>;
        ret_type ret;
        auto     out = ret.begin();
        out          = std::ranges::copy(left, out).out;
        std::ranges::copy(right, out);
        return ret;
    }
};

template <typename Char, std::size_t N>
basic_fixed_string(const Char (&arr)[N]) -> basic_fixed_string<Char, N - 1>;

template <typename Char, std::size_t N>
basic_fixed_string(const basic_fixed_string<Char, N>&) -> basic_fixed_string<Char, N>;

template <std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

}  // namespace neo

#endif
