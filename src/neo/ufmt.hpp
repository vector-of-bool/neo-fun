#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

#include "./concepts.hpp"

namespace neo {

void ufmt_append(std::string& str, neo::alike<bool> auto b) noexcept {
    str.append(b ? "true" : "false");
}

void ufmt_append(std::string& str, std::string_view s) noexcept;
void ufmt_append(std::string& str, char c) noexcept;

void ufmt_append(std::string& str, std::uint8_t i) noexcept;
void ufmt_append(std::string& str, std::int8_t i) noexcept;
void ufmt_append(std::string& str, std::uint16_t i) noexcept;
void ufmt_append(std::string& str, std::int16_t i) noexcept;
void ufmt_append(std::string& str, std::uint32_t i) noexcept;
void ufmt_append(std::string& str, std::int32_t i) noexcept;
void ufmt_append(std::string& str, std::uint64_t i) noexcept;
void ufmt_append(std::string& str, std::int64_t i) noexcept;

namespace detail {

[[noreturn]] void ufmt_too_few_args(std::string_view fmt_str, std::size_t count) noexcept;
[[noreturn]] void ufmt_too_many_args(std::string_view fmt_str, std::size_t count) noexcept;

template <std::size_t... Seq, typename... Ts>
void ufmt_append_nth(std::string_view fmt_str,
                     std::string&     out,
                     int              idx,
                     std::index_sequence<Seq...>,
                     const Ts&... ts) {
    bool any = (((idx == Seq) && static_cast<bool>(ufmt_append(out, ts), true)) || ...);
    if (!any) {
        ufmt_too_few_args(fmt_str, sizeof...(ts));
    }
}

}  // namespace detail

template <typename T>
concept formattable = requires(std::string& out, const T& arg) {
    ufmt_append(out, arg);
};

template <typename String, formattable... Args>
constexpr void ufmt_into(String& arg, const std::string_view fmt_str, const Args&... args) {
    auto remaining_fmt_str = fmt_str;
    arg.reserve(arg.size() + (fmt_str.size() * 2));
    auto idx = 0;
    while (true) {
        auto next_pl_pos = remaining_fmt_str.find("{}");
        if (next_pl_pos == remaining_fmt_str.npos) {
            arg.append(remaining_fmt_str);
            return;
        }
        auto next_lit_part = remaining_fmt_str.substr(0, next_pl_pos);
        arg.append(next_lit_part);
        detail::ufmt_append_nth(fmt_str, arg, idx, std::index_sequence_for<Args...>(), args...);
        remaining_fmt_str.remove_prefix(next_pl_pos + 2);
        ++idx;
    }
    detail::ufmt_too_many_args(fmt_str, sizeof...(args));
}

template <formattable... Ts>
std::string ufmt(const std::string_view fmt_str, const Ts&... args) {
    std::string ret;
    ufmt_into(ret, fmt_str, args...);
    return ret;
}

}  // namespace neo
