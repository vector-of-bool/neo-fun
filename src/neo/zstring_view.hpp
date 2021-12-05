#pragma once

#include "./assert.hpp"

#include <concepts>
#include <iterator>
#include <ranges>
#include <string_view>

namespace neo {

/**
 * @brief A string view with a guaranteed null terminator.
 *
 * This type is based on std::basic_string_view, and supports all operations
 * thereof except those that would create a 'zstring_view' that does not have a
 * null terminator. Namely, it lacks `substr(size_type, size_type)` and
 * `remove_suffix`, which would both allow breaking the invariant.
 */
template <typename Char, typename Traits = std::char_traits<Char>>
class basic_zstring_view : public std::basic_string_view<Char, Traits> {
    using _base = std::basic_string_view<Char, Traits>;

    /// Make these APIs private
    using _base::remove_suffix;
    using _base::substr;
    using _base::swap;

public:
    using string_view_type = std::basic_string_view<Char, Traits>;

    using const_pointer = typename string_view_type::const_pointer;
    using traits_type   = typename string_view_type::traits_type;
    using size_type     = typename string_view_type::size_type;
    using value_type    = typename string_view_type::value_type;

public:
    /// Default-construct with an empty string
    constexpr basic_zstring_view() noexcept = default;

    /**
     * @brief Construct a new zstring_view from the given pointer+length.
     *
     * Asserts that the 'ptr[count]' is a null character.
     */
    constexpr basic_zstring_view(const_pointer ptr, size_type count) noexcept
        : _base(ptr, count) {
        neo_assert(expects,
                   ptr[count] == value_type{0},
                   "Constructed a zstring_view with a pointer and size, but *(pointer + size) was "
                   "not a null character.",
                   *this,
                   ptr[count],
                   ptr,
                   count);
    }

    /// Construct from a C-pointer-to-string
    constexpr basic_zstring_view(const_pointer ptr) noexcept
        : _base(ptr) {}

    /**
     * @brief Construct from a contiguous range defined by the iterator and sentinel.
     *
     * Asserts that the resulting string contains a null terminator
     */
    template <std::contiguous_iterator Iter, std::sized_sentinel_for<Iter> Sent>
    requires std::same_as<std::iter_value_t<Iter>, value_type>  //
    constexpr basic_zstring_view(Iter first, Sent sent) noexcept
        : basic_zstring_view(std::to_address(first), static_cast<std::size_t>(sent - first)) {}

    /**
     * @brief Construct from a contiguous range of the same character type.
     *
     * Asserts that the resulting string contains a null terminator
     */
    template <std::ranges::contiguous_range R>
    requires std::same_as<std::ranges::range_value_t<R>, value_type>  //
    explicit constexpr basic_zstring_view(R&& r) noexcept
        : basic_zstring_view(std::ranges::begin(r), std::ranges::end(r)) {}

    /**
     * @brief Convert from a std::basic_string, which always guarantees that it has a null
     * terminator
     */
    template <typename Alloc>
    constexpr basic_zstring_view(
        const std::basic_string<value_type, traits_type, Alloc>& str) noexcept
        : _base(str) {}

    /**
     * @brief Convert from a character array, which is assumed to have a null terminator
     */
    template <std::size_t N>
    constexpr basic_zstring_view(const value_type (&arr)[N]) noexcept
        : _base(arr, N - 1) {}

    friend constexpr void do_repr(auto out, const basic_zstring_view* self) noexcept {
        if constexpr (std::same_as<string_view_type, std::string_view>) {
            out.type("neo::zstring_view");
        } else if constexpr (std::same_as<string_view_type, std::wstring_view>) {
            out.type("neo::wzstring_view");
        } else if constexpr (std::same_as<string_view_type, std::u8string_view>) {
            out.type("neo::u8zstring_view");
        } else if constexpr (std::same_as<string_view_type, std::u16string_view>) {
            out.type("neo::u16string_view");
        } else if constexpr (std::same_as<string_view_type, std::u32string_view>) {
            out.type("neo::u32string_view");
        } else {
            out.type("neo::basic_zstring_view<{}>", out.template repr_type<value_type>());
        }
        if (self) {
            out.value("{}", out.repr_value(static_cast<const string_view_type&>(*self)));
        }
    }

    /**
     * @brief Remove the first 'n' characters from the view.
     *
     * @param n The number of characters to remove
     *
     * Asserts than 'n <= size()'
     */
    constexpr void remove_prefix(size_type n) noexcept {
        neo_assert(expects,
                   n <= this->size(),
                   "Attempted to remove more characters from a zstring_view than are available",
                   *this,
                   n,
                   this->size());
        _base::remove_prefix(n);
    }

    /**
     * @brief Take a substring of this view without the first 'pos' characters.
     *
     * @param pos The number of characters to remove from the resulting string
     *
     * Asserts that 'pos <= size()'
     */
    [[nodiscard]] constexpr basic_zstring_view substr(size_type pos) const noexcept {
        neo_assert(expects,
                   pos <= this->size(),
                   "Attempted to take a substring starting beyond-the-end of a zstring_view",
                   pos,
                   this->size(),
                   *this);
        auto cp = *this;
        cp.remove_prefix(pos);
        return cp;
    }

    [[nodiscard]] constexpr bool contains(string_view_type sv) const noexcept {
        return this->find(sv) != this->npos;
    }
    [[nodiscard]] constexpr bool contains(value_type c) const noexcept {
        return this->find(c) != this->npos;
    }
    [[nodiscard]] constexpr bool contains(const_pointer ptr) const noexcept {
        return this->find(ptr) != this->npos;
    }
};

using zstring_view    = basic_zstring_view<char>;
using wzstring_view   = basic_zstring_view<wchar_t>;
using u8zstring_view  = basic_zstring_view<char8_t>;
using u16zstring_view = basic_zstring_view<char16_t>;
using u32zstring_view = basic_zstring_view<char32_t>;

}  // namespace neo
