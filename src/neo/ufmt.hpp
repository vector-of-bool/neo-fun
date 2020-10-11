#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace neo {

void ufmt_append(std::string& str, std::string_view s) noexcept;
void ufmt_append(std::string& str, bool b) noexcept;

void ufmt_append(std::string& str, std::uint8_t i) noexcept;
void ufmt_append(std::string& str, std::int8_t i) noexcept;
void ufmt_append(std::string& str, std::uint16_t i) noexcept;
void ufmt_append(std::string& str, std::int16_t i) noexcept;
void ufmt_append(std::string& str, std::uint32_t i) noexcept;
void ufmt_append(std::string& str, std::int32_t i) noexcept;
void ufmt_append(std::string& str, std::uint64_t i) noexcept;
void ufmt_append(std::string& str, std::int64_t i) noexcept;

namespace detail {

template <int N, typename Head, typename... Ts>
void ufmt_append_nth(std::string& out,
                     int          idx,
                     std::integral_constant<int, N>,
                     const Head& head,
                     const Ts&... ts) {
    if (idx == N) {
        ufmt_append(out, head);
    } else if constexpr (sizeof...(ts)) {
        ufmt_append_nth(out, idx, std::integral_constant<int, N + 1>(), ts...);
    } else {
        std::terminate();
    }
}

}  // namespace detail

template <typename... Ts>
std::string ufmt(std::string_view fmtstr, const Ts&... args) {
    std::string ret;
    ret.reserve(fmtstr.size() * 2);
    auto idx = 0;
    while (true) {
        auto next_pl_pos = fmtstr.find("{}");
        if (next_pl_pos == fmtstr.npos) {
            ret.append(fmtstr);
            return ret;
        }
        auto next_lit_part = fmtstr.substr(0, next_pl_pos);
        ret.append(next_lit_part);
        if constexpr (sizeof...(args)) {
            detail::ufmt_append_nth(ret, idx, std::integral_constant<int, 0>(), args...);
        }
        fmtstr.remove_prefix(next_pl_pos + 2);
        ++idx;
    }
    std::terminate();
}

}  // namespace neo