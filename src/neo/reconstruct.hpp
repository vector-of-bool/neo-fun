#pragma once

#include "./fwd.hpp"
#include "./text_range.hpp"

#include <ranges>
#include <type_traits>

namespace neo {

namespace range_detail {

template <typename T>
concept check_member_enable_reconstructible = requires {
    requires static_cast<bool>(T::enable_reconstructible_range);
};

}  // namespace range_detail

template <typename T>
constexpr bool enable_reconstructible_range = range_detail::check_member_enable_reconstructible<T>;

namespace range_detail {

// Detect std::basic_string and std::basic_string_view
template <typename T>
concept detect_str_common = neo::text_range<T>  //
    && requires(const std::remove_cvref_t<T> const_str) {
    const_str.length();
    requires std::constructible_from < std::remove_cvref_t<T>,
    typename std::remove_cvref_t<T>::const_pointer, std::ranges::range_size_t<T> > ;
};

template <typename T>
struct inherit_from {};

template <typename T>
requires(std::is_class_v<T> and not std::is_final_v<T>) struct inherit_from<T> : T {
};

template <typename T>
concept detect_string_view = detect_str_common<T>  //
    && neo::text_view<T>                           //
    && requires(T&& strv, std::remove_cvref_t<T> mut_sv) {
    typename inherit_from<std::remove_cvref_t<T>>::basic_string_view;
    requires std::same_as<typename inherit_from<std::remove_cvref_t<T>>::basic_string_view,
                          std::remove_cvref_t<T>>;
};

template <typename T>
concept detect_string = detect_str_common<T>  //
    && neo::mutable_text_range<T>             //
    && requires(std::remove_cvref_t<T> str, text_char_t<T> chr) {
    typename inherit_from<std::remove_cvref_t<T>>::basic_string;
    requires std::same_as<typename inherit_from<std::remove_cvref_t<T>>::basic_string,
                          std::remove_cvref_t<T>>;
};

template <typename T>
concept detect_string_or_view = detect_string_view<T> || detect_string<T>;

#define TYPENAME_IS(Type, Name) (requires { std::same_as<typename inherit_from<T>::Name, Type>; })
template <typename T>
constexpr bool detect_std_container_name =  //
    TYPENAME_IS(T, vector)                  //
    || TYPENAME_IS(T, list)                 //
    || TYPENAME_IS(T, deque)                //
    || TYPENAME_IS(T, forward_list)         //
    || TYPENAME_IS(T, set)                  //
    || TYPENAME_IS(T, map)                  //
    || TYPENAME_IS(T, multiset)             //
    || TYPENAME_IS(T, multimap)             //
    || TYPENAME_IS(T, unordered_set)        //
    || TYPENAME_IS(T, unordered_map)        //
    || TYPENAME_IS(T, unordered_multiset)   //
    || TYPENAME_IS(T, unordered_multimap)   //
    || TYPENAME_IS(T, span)                 //
    || TYPENAME_IS(T, subrange);

#undef TYPENAME_IS

template <typename T>
concept detect_reconstructible_std_range = std::ranges::range<T>                           //
    && std::constructible_from<T, std::ranges::iterator_t<T>, std::ranges::sentinel_t<T>>  //
    && detect_std_container_name<T>;

template <detect_string_or_view SV>
constexpr auto _reconstruct(const SV&                         sv,
                            std::ranges::iterator_t<const SV> iter,
                            std::ranges::sentinel_t<const SV> sentinel) noexcept {
    const auto start_offset = std::distance(std::ranges::begin(sv), iter);
    const auto size         = std::distance(iter, sentinel);
    return sv.substr(static_cast<std::ranges::range_size_t<SV>>(start_offset),
                     static_cast<std::ranges::range_size_t<SV>>(size));
}

template <typename T>
concept has_specialized_reconstruct = std::ranges::range<T>  //
    && requires(T&& r) {
    range_detail::_reconstruct(r, std::ranges::begin(r), std::ranges::end(r));
};

template <typename T>
constexpr bool blessed_reconstructible_range
    = has_specialized_reconstruct<T> || detect_reconstructible_std_range<T>;

template <typename T>
constexpr bool reconstruct_manually_enabled = neo::enable_reconstructible_range<T>;

template <typename T>
concept enable_reconstructible = neo::enable_reconstructible_range<T>  //
    || blessed_reconstructible_range<T>;

}  // namespace range_detail

template <typename T>
concept reconstructible_range = (std::ranges::range<T>  //
                                 && std::constructible_from<std::remove_cvref_t<T>,
                                                            std::ranges::iterator_t<T>,
                                                            std::ranges::sentinel_t<T>>  //
                                 && range_detail::enable_reconstructible<std::remove_cvref_t<T>>)
    or range_detail::has_specialized_reconstruct<T>;

template <typename R>
requires reconstructible_range<std::remove_cvref_t<R>>
constexpr std::remove_cvref_t<R> reconstruct_range(R&&                        range,
                                                   std::ranges::iterator_t<R> iter,
                                                   std::ranges::sentinel_t<R> sentinel)  //
    noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>,
                                             std::ranges::iterator_t<R>,
                                             std::ranges::sentinel_t<R>>) {
    if constexpr (range_detail::has_specialized_reconstruct<std::remove_cvref_t<R>>) {
        return range_detail::_reconstruct(NEO_FWD(range), iter, sentinel);
    } else {
        return std::remove_cvref_t<R>(iter, sentinel);
    }
}

}  // namespace neo
