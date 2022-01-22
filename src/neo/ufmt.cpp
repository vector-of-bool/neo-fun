#include "./ufmt.hpp"

#include "./assert.hpp"

#include <charconv>
#include <limits>

using namespace neo;

void neo::detail::ufmt_too_many_args(std::string_view fmt_str, std::size_t n_args_given) noexcept {
    neo_assert_always(expects,
                      false,
                      "Too many arguments were provided to neo::ufmt()",
                      fmt_str,
                      n_args_given);
}

void neo::detail::ufmt_too_few_args(std::string_view fmt_str, std::size_t n_args_given) noexcept {
    neo_assert_always(expects,
                      false,
                      "Too few arguments were provided to neo::ufmt()",
                      fmt_str,
                      n_args_given);
}

namespace {

template <typename C>
void write_str(std::string& out, std::basic_string_view<C> sv) noexcept {
    for (auto c_ : sv) {
        auto c = static_cast<char32_t>(c_);
        if (static_cast<std::size_t>(c) > std::numeric_limits<char>::max()) {
            auto v = static_cast<std::uint16_t>(c);
            char buf[8];
            auto res = std::to_chars(buf, buf + 8, v, 16);

            constexpr std::ptrdiff_t fill_size = sizeof(C) * 2;
            const std::ptrdiff_t     n_digits  = res.ptr - buf;
            const auto               pad_size  = fill_size - n_digits;
            out.append("\\x");
            out.append(static_cast<std::size_t>(pad_size), '0');
            out.append(buf, static_cast<std::size_t>(n_digits));
        } else {
            out.push_back(static_cast<char>(c));
        }
    }
}

void write_val(std::string& out, auto val) noexcept {
    char buf[32];
    auto res     = std::to_chars(buf, buf + sizeof(buf), val);
    auto n_chars = res.ptr - buf;
    out.append(buf, buf + n_chars);
}

}  // namespace

void neo::ufmt_detail::write_str(std::string& out, std::wstring_view sv) noexcept {
    ::write_str(out, sv);
}
void neo::ufmt_detail::write_str(std::string& out, std::u8string_view sv) noexcept {
    ::write_str(out, sv);
}
void neo::ufmt_detail::write_str(std::string& out, std::u16string_view sv) noexcept {
    ::write_str(out, sv);
}
void neo::ufmt_detail::write_str(std::string& out, std::u32string_view sv) noexcept {
    ::write_str(out, sv);
}

void neo::ufmt_append(std::string& str, double d) noexcept { str.append(std::to_string(d)); }

void neo::ufmt_detail::write_i64(std::string& out, std::int64_t v) noexcept { ::write_val(out, v); }
void neo::ufmt_detail::write_u64(std::string& out, std::uint64_t v) noexcept {
    ::write_val(out, v);
}
