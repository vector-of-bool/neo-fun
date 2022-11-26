#pragma once

#include "./concepts.hpp"
#include "./declval.hpp"
#include "./memory.hpp"
#include "./ranges.hpp"
#include "./version.hpp"

#include <algorithm>
#include <ranges>
#include <string>

namespace neo {

/**
 * @brief Variable template to check if a given type is a character type. Can be specialized.
 */
template <typename C>
constexpr bool is_char_type_v = false;

template <>
constexpr bool is_char_type_v<char> = true;

template <>
constexpr bool is_char_type_v<wchar_t> = true;

template <>
constexpr bool is_char_type_v<char8_t> = true;

template <>
constexpr bool is_char_type_v<char16_t> = true;

template <>
constexpr bool is_char_type_v<char32_t> = true;

// clang-format off
/**
 * @brief Match a type that is a character type.
 *
 * @tparam T A character type
 */
template <typename T>
concept character_type = is_char_type_v<T>;

/**
 * @brief Match an iterator whose value type is a character type.
 *
 * Implies character_iterator<Iter>
 */
template <typename Iter>
concept text_iterator =
    std::input_or_output_iterator<Iter>
    and character_type<std::iter_value_t<Iter>>
    ;

/**
 * @brief Obtain the character type of a character_iterator.
 *
 * Substitution failure if 'Iter' is not a character_iterator
 */
template <text_iterator Iter>
using iter_char_t = std::iter_value_t<Iter>;

/**
 * @brief Match an input iterator whose value type is a character type
 *
 * @deprecated Use text_iterator instead
 */
template <typename Iter>
concept character_iterator = text_iterator<Iter>;

/**
 * @brief Match any range whose iterator is a text_iterator
 */
template <typename R>
concept text_range =
    std::ranges::range<R>
    and text_iterator<std::ranges::iterator_t<R>>;

/**
 * @brief Match a range type whose value type is a character type
 * @deprecated Use text_range instead
 */
template <typename R>
concept character_range = text_range<R>;

/// A text_range which is also an input_range
template <typename R>
concept input_text_range = text_range<R> and std::ranges::input_range<R>;

/// A text_range which is also an output_range
template <typename R, typename Char>
concept output_text_range = text_range<R> and std::ranges::output_range<R, Char>;

/// A text_range which is also forward-iterable
template <typename R>
concept forward_text_range = text_range<R> and std::ranges::forward_range<R>;

/// A text_range which is bidirectionally iterable
template <typename R>
concept bidirectional_text_range = text_range<R> and std::ranges::bidirectional_range<R>;

/// A text_range which is random-access
template <typename R>
concept random_access_text_range = text_range<R> and std::ranges::random_access_range<R>;

/// A text_range which is also contiguous
template <typename R>
concept contiguous_text_range = text_range<R> and std::ranges::contiguous_range<R>;

/// A text_range which is a sized_range (has a constant-time known size)
template <typename R>
concept sized_text_range = text_range<R> and std::ranges::sized_range<R>;

/// Trait to detect a character array reference
template <typename T>
constexpr bool is_text_array_ref_v = false;

template <typename Char, std::size_t N>
    requires character_type<std::remove_const_t<Char>>
constexpr bool is_text_array_ref_v<Char (&)[N]> = true;

template <typename Char, std::size_t N>
    requires character_type<std::remove_const_t<Char>>
constexpr bool is_text_array_ref_v<Char (&&)[N]> = true;

/// Match an argument that is a text array
template <typename R>
concept text_array_ref = contiguous_text_range<R> and is_text_array_ref_v<R>;

/**
 * @brief Obtain the character type of a text range
 *
 * Substitution failure if 'T' is not a text_range
 */
template <text_range T>
using text_char_t = std::ranges::range_value_t<T>;

/**
 * @brief Compute the size of a sized_text_range
 */
template <sized_text_range R>
constexpr std::size_t text_range_size(const R& r) noexcept {
    return std::ranges::size(r);
}

// Overload for character arrays
template <character_type C, std::size_t N>
constexpr std::size_t text_range_size(const C (&)[N]) noexcept {
    return N-1;
}

/**
 * @brief Compute the size of the given text_range, even if it is not a sized_range
 *
 * This can degrade to linear performance if the range is not sized or random-access
 */
template <text_range R>
constexpr std::ranges::range_difference_t<R> text_range_distance(const R& r) noexcept {
    if constexpr (text_range<R>) {
        return static_cast<std::ranges::range_difference_t<R>>(text_range_size(r));
    } else {
        return std::ranges::distance(r);
    }
}

/**
 * @brief Match a text range with the some common text manipulation methods.
 *
 * Expects for an `r` of type `T`, with `text_char_t` of `C`:
 *
 *  - `r.append(C*, range_size_t<T>)`
 *  - `r.data()` for non-const `r` returns a pointer-to-mutable `C`
 *  - `r.data()` for const `r` returns a pointer-to-const `C`
 *  - `r.push_back(C)`
 *  - `r.pop_back()`
 *  - `r.erase(range_iter_t<T>, range_sentinel_t<T>)`
 */
template <typename T>
concept mutable_text_range =
    text_range<T>
    && std::regular<std::remove_cvref_t<T>>
    && std::constructible_from<std::remove_cvref_t<T>,
                               text_char_t<T> const*,
                               std::ranges::range_size_t<T>>
    && requires (T string,
                 const std::remove_cvref_t<T> const_str,
                 std::ranges::range_size_t<T> size,
                 text_char_t<T> const* cptr,
                 text_char_t<T> chr,
                 std::ranges::iterator_t<T> iter,
                 std::ranges::sentinel_t<T> sentinel) {
        string.append(cptr, size);
        { std::ranges::data(const_str) } noexcept
            -> std::same_as<text_char_t<T> const*>;
        { std::ranges::data(string) } noexcept
            -> std::same_as<text_char_t<T>*>;
        string.push_back(chr);
        string.pop_back();
        string.erase(iter, sentinel);
    };

/**
 * @brief Match a text_range that is also a view.
 */
template <typename R>
concept text_view = text_range<R> and std::ranges::view<R>;

template <typename R>
concept viewable_text_range = text_range<R> and std::ranges::viewable_range<R>;
// clang-format on

namespace text_range_detail {

template <typename R, typename Char>
struct char_traits_of {
    using type = std::char_traits<Char>;
};

template <typename R, typename C>
requires requires { typename R::traits_type; }
struct char_traits_of<R, C> {
    using type = typename R::traits_type;
};

}  // namespace text_range_detail

/**
 * @brief Get the char_traits for the given text_range `T`.
 *
 * If `T` has a nested `traits_type`, returns `T::traits_type`, otherwise
 * `std::char_traits<text_char_t<T>>`
 */
template <text_range T>
using text_char_traits_t = typename text_range_detail::char_traits_of<T, text_char_t<T>>::type;

/**
 * @brief Get an allocator associated with the given text_range
 *
 * If `R` has a `get_allocator()` method, returns `r.get_allocator()`, otherwise
 * a 'alloc', if provided, otherwise a default-constructed std::allocator for the
 * range's character type
 */
template <text_range R, typename Alloc = std::allocator<text_char_t<R>>>
constexpr auto text_allocator(R&& r, Alloc alloc = Alloc()) noexcept {
    if constexpr (text_array_ref<R>) {
        return neo::rebind_alloc<text_char_t<R>>(alloc);
    } else {
        return neo::allocator_of(r, alloc);
    }
}

/**
 * @brief Get the type of allocator associated with the text_range `R`, or a
 * std::allocator for the text range's character type.
 */
template <text_range R>
using text_allocator_t = decltype(neo::text_allocator(NEO_DECLVAL(R)));

/**
 * @brief Convert a contiguous viewable text range into a std::basic_string_view
 *
 * @param str The text_range to convert
 * @return requires constexpr A new std::basic_string_view of the appropriate character type
 */
template <contiguous_text_range R>
requires viewable_text_range<R>
constexpr auto to_std_string_view(R&& str) noexcept {
    using traits    = text_char_traits_t<R>;
    using view_type = std::basic_string_view<text_char_t<R>, traits>;
    auto p          = std::ranges::data(str);
    auto size       = neo::text_range_size(str);
    return view_type(p, size);
}

/**
 * @brief Obtain a text_view of the given text_range `r`
 *
 * If `r` is already a text_view, returns `r`, otherwise creates a
 * std::basic_string_view for the contents of the text range
 */
template <input_text_range R>
requires viewable_text_range<R>
constexpr text_view auto view_text(R&& r) noexcept {
    if constexpr (text_array_ref<R>) {
        return to_std_string_view(r);
    } else {
        return std::views::all(NEO_FWD(r));
    }
}

/**
 * @brief Obtain the type that will be returned by `view_text()` for the text_range `R`
 */
template <text_range R>
using view_text_t = decltype(neo::view_text(NEO_DECLVAL(R)));

/**
 * @brief Obtain the end sentinel of the given text_range.
 *
 * This differs from std::ranges::end in that it drops one char from
 * character arrays, which are assumed to be null-terminated.
 */
template <text_range R>
constexpr auto text_range_end(R&& r) noexcept {
    if constexpr (text_array_ref<R> or std::ranges::common_range<R>) {
        return std::ranges::next(std::ranges::begin(r), neo::text_range_size(r));
    } else {
        return std::ranges::end(r);
    }
}

/**
 * @brief Convert a given text_range into a single std::basic_string
 */
inline constexpr struct to_std_string_fn : ranges::pipable {
    /**
     * @brief Construct a std::basic_string from the given text_range and allocator
     *
     * @param str An input text range.
     * @param alloc The allocator. If omitted, uses std::allocator
     * @return constexpr auto
     */
    template <input_text_range R, typename Alloc>
    constexpr auto operator()(R&& str, Alloc alloc) const noexcept(ranges::nothrow_range<R>) {
        using traits   = text_char_traits_t<R>;
        using str_type = std::basic_string<text_char_t<R>, traits, Alloc>;

        if constexpr (contiguous_text_range<R>) {
            // We can copy out of the buffer in the string
            const auto ptr = std::ranges::data(str);
            const auto sz  = neo::text_range_size(str);
            return str_type(ptr, sz, alloc);
        }

        if constexpr (std::constructible_from<str_type, R, Alloc>) {
            /// We can convert directly
            return str_type(NEO_FWD(str), alloc);
        }

        if (std::ranges::common_range<R> and random_access_text_range<R>) {
            /// The string has a known size and is common, so we can use the assign() method to get
            /// the data in-place.
            const auto it  = std::ranges::begin(str);
            const auto end = neo::text_range_end(str);
            return str_type(it, end, alloc);
        } else if (random_access_text_range<R> or sized_text_range<R>) {
            // We can calculate the size of the range in constant time, so we will allocate a region
            // and then overwrite it with the range data.
            // Compute the size:
            auto size = static_cast<typename str_type::size_type>(neo::text_range_distance(str));
            // Normalize-away char arrays:
            auto view = neo::view_text(str);

            // If the string is small enough, just create a small buffer then assign-into the
            // std::string
            constexpr auto small_size = 32;
            if (size <= small_size) {
                std::array<text_char_t<R>, small_size> arr;
                std::ranges::copy(view, arr.begin());
                return str_type(arr.data(), arr.data() + size, alloc);
            }

            std::exception_ptr e;
            auto fill = [&](typename str_type::pointer ptr, typename str_type::size_type) {
                if constexpr (neo::ranges::nothrow_range<R>) {
                    std::ranges::copy(view, ptr);
                } else {
                    // Guard around ranges that may throw, since throwing within
                    // resize_and_overwrite it UB. We'll rethrow the exception
                    // when it is safe.
                    try {
                        std::ranges::copy(ptr, view);
                    } catch (...) {
                        e = std::current_exception();
                    }
                }
                return size;
            };

            str_type ret(alloc);
#if __cpp_lib_string_resize_and_overwrite >= 202110L
            ret.resize_and_overwrite(size, fill);
#else
            ret.resize(neo::text_range_distance(str));
            fill(ret.data(), 0);
#endif
            if (e and not ranges::nothrow_range<R>) {
                std::rethrow_exception(e);
            }
            return ret;
        } else {
            // Slow case, we need to just copy out the characters
            str_type ret(alloc);
            std::ranges::copy(std::ranges::begin(str),
                              neo::text_range_end(str),
                              std::back_inserter(ret));
            return ret;
        }
    }

    /**
     * @brief Convert to a std::basic_string.
     *
     * Attempts to obtain an allocator from the range, but falls back to std::allocator otherwise.
     */
    template <input_text_range R>
    constexpr auto operator()(R&& str) const noexcept(ranges::nothrow_range<R>) {
        auto alloc = text_allocator(str);
        return (*this)(NEO_FWD(str), alloc);
    }
} to_std_string;

/**
 * @brief Create a mutable copy of the given text_range
 */
template <input_text_range R>
constexpr auto copy_text(R&& r, auto alloc) noexcept {
    using R1 = std::remove_cvref_t<R>;
    if constexpr (mutable_text_range<R1>) {
        return R1(std::ranges::data(r), neo::text_range_size(r), alloc);
    } else {
        return to_std_string(NEO_FWD(r), alloc);
    }
}

template <input_text_range R>
constexpr auto copy_text(R&& r) noexcept {
    auto alloc = neo::text_allocator(r);
    return copy_text(NEO_FWD(r), alloc);
}

template <text_range R>
using copy_text_t = decltype(copy_text(NEO_DECLVAL(R)));

}  // namespace neo
