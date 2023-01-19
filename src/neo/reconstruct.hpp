#pragma once

#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./text_range.hpp"
#include "./version.hpp"

#include <ranges>
#include <type_traits>

namespace neo {

namespace _sr = std::ranges;

#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranegs_to_container >= 202202L
using std::from_range;
using std::from_range_t;
#else
struct from_range_t {
    explicit from_range_t() = default;
};

inline constexpr from_range_t from_range{};
#endif

namespace range_detail {

template <typename T>
concept check_member_enable_reconstructible
    = requires { requires static_cast<bool>(T::enable_reconstructible_range); };

}  // namespace range_detail

template <typename T>
constexpr bool enable_reconstructible_range = range_detail::check_member_enable_reconstructible<T>;

namespace range_detail {

// Detect std::basic_string and std::basic_string_view
template <typename T>
concept detect_str_common = neo::text_range<T>  //
    && requires(const remove_cvref_t<T> const_str) {
           const_str.length();
           requires constructible_from<remove_cvref_t<T>,
                                       typename remove_cvref_t<T>::const_pointer,
                                       _sr::range_size_t<T>>;
       };

template <typename T>
struct inherit_from {};

template <typename T>
    requires(neo_is_class(T) and not neo_is_final(T))
struct inherit_from<T> : T {};

template <typename T>
concept detect_string_view = detect_str_common<T>  //
    && neo::text_view<T>                           //
    && requires(T&& strv, remove_cvref_t<T> mut_sv) {
           typename inherit_from<remove_cvref_t<T>>::basic_string_view;
           requires weak_same_as<typename inherit_from<remove_cvref_t<T>>::basic_string_view,
                                 remove_cvref_t<T>>;
       };

template <typename T>
concept detect_string = detect_str_common<T>  //
    && neo::mutable_text_range<T>             //
    && requires(remove_cvref_t<T> str, text_char_t<T> chr) {
           typename inherit_from<remove_cvref_t<T>>::basic_string;
           requires weak_same_as<typename inherit_from<remove_cvref_t<T>>::basic_string,
                                 remove_cvref_t<T>>;
       };

template <typename T>
concept detect_string_or_view = detect_string_view<T> || detect_string<T>;

#define TYPENAME_IS(Type, Name)                                                                    \
    (requires { requires weak_same_as<typename inherit_from<T>::Name, Type>; })
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
concept detect_reconstructible_std_range = _sr::range<T>              //
    && constructible_from<T, _sr::iterator_t<T>, _sr::sentinel_t<T>>  //
    && detect_std_container_name<T>;

template <detect_string_or_view SV>
constexpr auto _reconstruct(const SV&                 sv,
                            _sr::iterator_t<const SV> iter,
                            _sr::sentinel_t<const SV> sentinel) noexcept {
    const auto start_offset = std::distance(_sr::begin(sv), iter);
    const auto size         = std::distance(iter, sentinel);
    return sv.substr(static_cast<_sr::range_size_t<SV>>(start_offset),
                     static_cast<_sr::range_size_t<SV>>(size));
}

template <typename T>
concept has_specialized_reconstruct = _sr::range<T>  //
    && requires(T&& r) { range_detail::_reconstruct(r, _sr::begin(r), _sr::end(r)); };

template <typename T>
constexpr bool blessed_reconstructible_range
    = has_specialized_reconstruct<T> || detect_reconstructible_std_range<T>;

template <typename T>
constexpr bool reconstruct_manually_enabled = neo::enable_reconstructible_range<T>;

template <typename T>
concept enable_reconstructible = neo::enable_reconstructible_range<T>  //
    || blessed_reconstructible_range<T>;

template <typename T>
constexpr bool is_nothrow_special_reconstruct_v = false;

template <has_specialized_reconstruct T>
constexpr bool is_nothrow_special_reconstruct_v<T>
    = requires(T && range) {
          {
              range_detail::_reconstruct(NEO_FWD(range),
                                         std::ranges::begin(range),
                                         std::ranges::end(range))
          } noexcept;
      };

}  // namespace range_detail

template <typename T>
concept reconstructible_range = (_sr::range<T>  //
                                 and constructible_from<remove_cvref_t<T>,
                                                        _sr::iterator_t<T>,
                                                        _sr::sentinel_t<T>>  //
                                 and range_detail::enable_reconstructible<remove_cvref_t<T>>)
    or range_detail::has_specialized_reconstruct<T>;

template <typename R>
constexpr bool is_nothrow_reconstructible_range_v =  //
    (reconstructible_range<R>                        //
     and nothrow_constructible_from<remove_cvref_t<R>,
                                    _sr::iterator_t<R>,
                                    _sr::sentinel_t<R>>)  //
    or range_detail::is_nothrow_special_reconstruct_v<remove_cvref_t<R>>;

template <typename R>
    requires reconstructible_range<remove_cvref_t<R>>
constexpr remove_cvref_t<R> reconstruct_range(R&&                range,
                                              _sr::iterator_t<R> iter,
                                              _sr::sentinel_t<R> sentinel)  //
    noexcept(is_nothrow_reconstructible_range_v<remove_cvref_t<R>>) {
    if constexpr (range_detail::has_specialized_reconstruct<remove_cvref_t<R>>) {
        return range_detail::_reconstruct(NEO_FWD(range), iter, sentinel);
    } else {
        return remove_cvref_t<R>(iter, sentinel);
    }
}

}  // namespace neo
