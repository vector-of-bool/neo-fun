#pragma once

#include "./fwd.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace neo {

/**
 * @brief Obtain a std::basic_string<> for the given string or string view.
 *
 * Generates a nested typename `type` that is a specialization of
 * std::basic_string<> matching the char type and traits of the provided
 * string. If given a basic_string_view, the default allocator will be used.
 *
 * @tparam T A std::basic_string<> or std::basic_string_view<> type.
 * @tparam Alloc The allocator to use in the std::basic_string if the given
 *         T is a string view (and has no associate allocator type).
 */
template <typename T, typename Alloc = std::allocator<void>>
struct string_type {};

template <typename Char, typename Traits, typename Alloc, typename BackupAlloc>
struct string_type<std::basic_string<Char, Traits, Alloc>, BackupAlloc> {
    using type = std::basic_string<Char, Traits, Alloc>;
};

template <typename Char, typename Traits, typename Allocator>
struct string_type<std::basic_string_view<Char, Traits>, Allocator> {
    using _alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Char>;
    using type   = std::basic_string<Char, Traits, _alloc>;
};

/**
 * @brief Obtain a std::basic_string_view<> for the given string or string view.
 *
 * Generates a nested typename `type` that is a specialization of
 * std::basic_string_view<> matching the char type and traits of the provided
 * string. If given a std::basic_string<>, the allocator thereof will be
 * ignored.
 *
 * @tparam T A std::basic_string<> or std::basic_string_view<> type.
 */
template <typename T>
struct string_view_type {};

template <typename Char, typename Traits>
struct string_view_type<std::basic_string_view<Char, Traits>> {
    using type = std::basic_string_view<Char, Traits>;
};

template <typename Char, typename Traits, typename Alloc>
struct string_view_type<std::basic_string<Char, Traits, Alloc>> {
    using type = std::basic_string_view<Char, Traits>;
};

/**
 * @brief Map a string or string-view to a basic_string<>
 *
 * @tparam T A string or string-view type
 * @tparam The fallback allocator if the given T is a string view.
 */
template <typename T, typename Allocator = std::allocator<void>>
using string_type_t = typename string_type<T>::type;

/**
 * @brief Map a string or string-view to a basic_string_view<>
 *
 * @tparam T A string or string-view type
 */
template <typename T>
using string_view_type_t = typename string_view_type<T>::type;

/**
 * @brief Obtain an allocator type appropriate for a string or string view.
 *
 * If given a std::basic_string<>, nested name 'type' is the allocator type of
 * that basic_string<>.
 *
 * If given a std::basic_string_view<>, nested name 'type' is given Default
 * fallback allocator type rebound for the character type of the string.
 *
 * @tparam T A string or string view
 * @tparam Default The fallback allocator type. The allocated type will
 *         automatically be rebound to the character type of the string.
 */
template <typename T, typename Default = std::allocator<void>>
struct string_allocator_type {};

template <typename C, typename T, typename Alloc, typename Default>
struct string_allocator_type<std::basic_string<C, T, Alloc>, Default> {
    using type = Alloc;
};

template <typename C, typename T, typename Default>
struct string_allocator_type<std::basic_string_view<C, T>, Default> {
    using type = typename std::allocator_traits<Default>::template rebind_alloc<C>;
};

/**
 * @brief Map a string or string-view to an allocator appropriate for that type.
 *
 * @tparam T A string or string-view type.
 * @tparam Default The fallback allocator type in case that 'T' is a string-view
 *         and has no allocator. The allocator will be rebound for the character
 *         type of the basic_string_view<>.
 */
template <typename T, typename Default = std::allocator<void>>
using string_allocator_type_t = typename string_allocator_type<T, Default>::type;

/**
 * @brief Get the allocator used by the given string, or use a fallback.
 *
 * If given a string, then this returns the allocator of that string. If given
 * a string view, returns the default allocator provided
 *
 * @param s A string or string view
 * @param a The fallback allocator to return
 * @return A new allocator, either copied from the given string, or a copy of
 *         the given fallback allocator.
 */
template <typename C, typename T, typename A, typename F = std::allocator<void>>
constexpr A get_string_allocator(const std::basic_string<C, T, A>& s, F = {}) noexcept {
    return s.get_allocator();
}

template <typename C, typename T, typename Alloc = std::allocator<void>>
constexpr auto get_string_allocator(const std::basic_string_view<C, T>&, Alloc a = {}) noexcept {
    return typename std::allocator_traits<Alloc>::template rebind_alloc<C>(a);
}

/**
 * @brief Convert a string or string view into a std::basic_string<>.
 *
 * This conversion will maintain the string's content.
 *
 * If given a string, then the new string will share its allocator.
 * If given a string view, the new string will use the allocator given for 'a'
 *
 * @param t A string or string-view from which to create a new string
 * @param a The fallback allocator to use with the new string.
 */
template <typename T, typename Alloc = string_allocator_type_t<std::remove_cvref_t<T>>>
constexpr string_type_t<std::remove_cvref_t<T>> make_string(T&& t, Alloc a = {}) noexcept {
    auto alloc = get_string_allocator(t, a);
    return string_type_t<std::remove_cvref_t<T>>(NEO_FWD(t), alloc);
}

/**
 * @brief Convert a string or string view into a std::basic_string_view<>.
 *
 * This conversion will generate a new string view of the content of the given
 * string.
 *
 * @param t A string or string-view from which to create the new string
 */
template <typename T>
constexpr string_view_type_t<T> make_string_view(const T& t) noexcept {
    return string_view_type_t<T>(t);
}

/**
 * @brief Create a new empty string from the given string or string view.
 *
 * If given a string view, will simply default-construct an appropriate string
 * type. If given a string, will construct a new empty string using the same
 * allocator as that string.
 *
 * @param t A string or string-view.
 */
template <typename String, typename Alloc = string_allocator_type_t<String>>
constexpr string_type_t<String> make_empty_string_from(const String& t, Alloc a = {}) noexcept {
    return string_type_t<String>(get_string_allocator(t, a));
}

}  // namespace neo
