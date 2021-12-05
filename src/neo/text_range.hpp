#pragma once

#include "./declval.hpp"

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
 * @brief Match an input iterator whose value type is a character type
 */
template <typename Iter>
concept character_iterator =
    std::input_iterator<Iter>
    && character_type<std::iter_value_t<Iter>>;

/**
 * @brief Match a range type whose value type is a character type
 */
template <typename R>
concept character_range =
    std::ranges::input_range<R>
    && character_iterator<std::ranges::iterator_t<R>>;

/**
 * @brief Obtain the character type of a character_iterator.
 *
 * Substitution failure if 'Iter' is not a character_iterator
 */
template <character_iterator Iter>
using iter_char_t = std::iter_value_t<Iter>;

/**
 * @brief Match a contiguous iterator whose value type is a character type.
 *
 * Implies character_iterator<Iter>
 */
template <typename Iter>
concept text_iterator =
    character_iterator<Iter>
    && std::contiguous_iterator<Iter>;

/**
 * @brief Match any contiguous sized range whose iterator is a text_iterator
 */
template <typename T>
concept text_range =
    std::ranges::contiguous_range<T>
    && std::ranges::sized_range<T>
    && character_range<T>;

/**
 * @brief Obtain the character type of a text range
 *
 * Substitution failure if 'T' is not a character_range
 */
template <character_range T>
using text_char_t = std::ranges::range_value_t<T>;

template <text_range R>
constexpr std::size_t text_range_size(const R& r) noexcept {
    return std::ranges::size(r);
}

template <character_type C, std::size_t N>
constexpr std::size_t text_range_size(const C (&)[N]) noexcept {
    return N-1;
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
                               std::ranges::range_size_t<T> >
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
 * @brief Match a text_range whose data() always returns a pointer-to-const.
 */
template <typename T>
concept text_view =
    text_range<T>
    && !mutable_text_range<T>
    && requires(std::remove_const_t<T> sv) {
        { std::ranges::data(sv) } noexcept
            -> std::same_as<text_char_t<T> const*>;
    };
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

    template <typename T>
    constexpr auto get_allocator_of(T&&, auto alloc) noexcept {
        return alloc;
    }

    template <typename T>
    constexpr auto get_allocator_of(T &&) noexcept {
        return std::allocator<text_char_t<T>>{};
    }

    template <typename T>
    constexpr auto get_allocator_of(T && t) noexcept requires requires {
        t.get_allocator();
    }
    { return t.get_allocator(); }

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
 * a 'alloc', if provided, otherwise a default-constructed std::allocator
 */
template <text_range R>
constexpr auto text_allocator(R&& r, auto alloc) noexcept {
    return text_range_detail::get_allocator_of(r, alloc);
}

template <text_range R>
constexpr auto text_allocator(R&& r) noexcept {
    return text_range_detail::get_allocator_of(r);
}

/**
 * @brief Get the type of allocator associated with the text_range `R`, or a
 * std::allocator for the text range's character type.
 */
template <text_range R>
using text_allocator_t = decltype(neo::text_allocator(NEO_DECLVAL(R)));

/**
 * @brief Obtain a text_view of the given text_range `r`
 *
 * If `r` is already a text_view, returns `r`, otherwise creates a
 * std::basic_string_view for the contents of the text range
 */
template <text_range R>
constexpr text_view auto view_text(R&& r) noexcept {
    if constexpr (text_view<R>) {
        return r;
    } else {
        using traits = text_char_traits_t<R>;
        using sv     = std::basic_string_view<text_char_t<R>, traits>;
        return sv(std::ranges::data(r), neo::text_range_size(r));
    }
}

/**
 * @brief Obtain a type that will be returned by `view_text()` for the text_range `R`
 */
template <text_range R>
using view_text_t = decltype(neo::view_text(NEO_DECLVAL(R)));

/**
 * @brief Create a mutable copy of the given text_range
 */
template <text_range R>
constexpr auto copy_text(const R& r, auto alloc) noexcept {
    if constexpr (mutable_text_range<R>) {
        return R(std::ranges::data(r), neo::text_range_size(r), alloc);
    } else {
        using traits = text_char_traits_t<R>;
        using str    = std::basic_string<text_char_t<R>, traits, text_allocator_t<R>>;
        return str(std::ranges::data(r), neo::text_range_size(r), alloc);
    }
}

template <text_range R>
constexpr auto copy_text(const R& r) noexcept {
    auto alloc = neo::text_allocator(r);
    return copy_text(r, alloc);
}

template <text_range R>
using copy_text_t = decltype(copy_text(NEO_DECLVAL(R)));

}  // namespace neo
