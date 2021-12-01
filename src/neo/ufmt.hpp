#pragma once

#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

#include "./concepts.hpp"

namespace neo {

namespace ufmt_detail {

/// Check if T has a .to_string() member function
template <typename T>
concept to_string_member = requires(const T& item) {
    { item.to_string() }
    ->std::same_as<std::string>;
};

/// Check if to_string() is visible via ADL for T
template <typename T>
concept to_string_adl = requires(const T& item) {
    { to_string(item) }
    ->std::same_as<std::string>;
};

inline void write_str(std::string& s, std::string_view sv) noexcept { s.append(sv); }

void write_str(std::string& s, std::wstring_view sv) noexcept;
void write_str(std::string& s, std::u8string_view sv) noexcept;
void write_str(std::string& s, std::u16string_view sv) noexcept;
void write_str(std::string& s, std::u32string_view sv) noexcept;

}  // namespace ufmt_detail

/// Check if the given type has a .to_string() member or a to_string() ADL-visible function.
template <typename T>
concept can_to_string = ufmt_detail::to_string_member<T> || ufmt_detail::to_string_adl<T>;

void ufmt_append(std::string& str, neo::alike<bool> auto b) noexcept {
    str.append(b ? "true" : "false");
}

template <typename Char, typename Traits>
inline void ufmt_append(std::string& str, std::basic_string_view<Char, Traits> sv) noexcept {
    ufmt_detail::write_str(str, std::basic_string_view<Char>(sv.data(), sv.size()));
}

template <typename Char, typename Traits, typename Alloc>
inline void ufmt_append(std::string& str, const std::basic_string<Char, Traits, Alloc> s) noexcept {
    ufmt_detail::write_str(str, std::basic_string_view<Char>(s.data(), s.size()));
}

inline void ufmt_append(std::string& str, const char* s) noexcept { str.append(s); }
inline void ufmt_append(std::string& str, char c) noexcept { str.push_back(c); }

void ufmt_append(std::string& str, std::uint8_t i) noexcept;
void ufmt_append(std::string& str, std::int8_t i) noexcept;
void ufmt_append(std::string& str, std::uint16_t i) noexcept;
void ufmt_append(std::string& str, std::int16_t i) noexcept;
void ufmt_append(std::string& str, std::uint32_t i) noexcept;
void ufmt_append(std::string& str, std::int32_t i) noexcept;
void ufmt_append(std::string& str, std::uint64_t i) noexcept;
void ufmt_append(std::string& str, std::int64_t i) noexcept;
void ufmt_append(std::string& str, float i) noexcept;
void ufmt_append(std::string& str, double i) noexcept;

void ufmt_append(std::string& str, neo::alike<std::size_t> auto i) noexcept
    requires(!std::same_as<std::size_t, std::uint64_t>) {
    ufmt_append(str, static_cast<std::uint64_t>(i));
}
void ufmt_append(std::string& str, neo::alike<std::ptrdiff_t> auto i) noexcept
    requires(!std::same_as<std::ptrdiff_t, std::int64_t>) {
    ufmt_append(str, static_cast<std::int64_t>(i));
}

/// Check if the given T can be formatted via ufmt_append()
template <typename T>
concept ufmt_formattable = requires(std::string& out, const T& arg) {
    ufmt_append(out, arg);
};

/// Check if the given type is valid to pass to neo::ufmt()
template <typename T>
concept formattable = ufmt_formattable<T> || can_to_string<T>;

/// Append the string represenation of the given item to the given string
template <formattable T>
constexpr void to_string_into(std::string& out, const T& item) noexcept {
    if constexpr (ufmt_formattable<T>) {
        ufmt_append(out, item);
    } else if constexpr (ufmt_detail::to_string_member<T>) {
        out.append(item.to_string());
    } else {
        out.append(to_string(item));
    }
}

namespace detail {
[[noreturn]] void ufmt_too_few_args(std::string_view fmt_str, std::size_t count) noexcept;
[[noreturn]] void ufmt_too_many_args(std::string_view fmt_str, std::size_t count) noexcept;

template <std::size_t... Seq, typename... Ts>
void ufmt_append_nth(std::string_view fmt_str,
                     std::string&     out,
                     int              idx,
                     std::index_sequence<Seq...>,
                     const Ts&... ts) {
    bool any = (((idx == Seq) && static_cast<bool>(to_string_into(out, ts), true)) || ...);
    if (!any) {
        ufmt_too_few_args(fmt_str, sizeof...(ts));
    }
}

}  // namespace detail

/**
 * @brief Append the result of the format-string `fmt_str` with `args` to the end of `str`
 */
template <typename String, formattable... Args>
constexpr void ufmt_into(String& str, const std::string_view fmt_str, const Args&... args) {
    auto remaining_fmt_str = fmt_str;
    str.reserve(str.size() + (fmt_str.size() * 2));
    auto idx = 0;
    while (true) {
        auto next_pl_pos = remaining_fmt_str.find("{}");
        if (next_pl_pos == remaining_fmt_str.npos) {
            str.append(remaining_fmt_str);
            return;
        }
        auto next_lit_part = remaining_fmt_str.substr(0, next_pl_pos);
        str.append(next_lit_part);
        detail::ufmt_append_nth(fmt_str, str, idx, std::index_sequence_for<Args...>(), args...);
        remaining_fmt_str.remove_prefix(next_pl_pos + 2);
        ++idx;
    }
    detail::ufmt_too_many_args(fmt_str, sizeof...(args));
}

/**
 * @brief Generate a std::string containing the result of rendering the given format-string
 */
template <formattable... Ts>
std::string ufmt(const std::string_view fmt_str, const Ts&... args) {
    std::string ret;
    ufmt_into(ret, fmt_str, args...);
    return ret;
}

namespace _to_string_fn_ns_ {

inline constexpr struct to_string_fn {
    /**
     * @brief Convert the given item into its string representation.
     *
     * Works with any type that has a .to_string() member, a to_string() ADL-visible function, or
     * supported by neo::ufmt
     */
    template <formattable T>
    auto operator()(const T& item) const noexcept {
        std::string ret;
        to_string_into(ret, item);
        return ret;
    }
} to_string;

}  // namespace _to_string_fn_ns_

using namespace _to_string_fn_ns_;

}  // namespace neo
