#include "./ufmt.hpp"

#include <charconv>

using namespace neo;

void neo::ufmt_append(std::string& out, std::string_view part) noexcept { out.append(part); }
void neo::ufmt_append(std::string& out, bool b) noexcept { out.append(b ? "true" : "false"); }
void neo::ufmt_append(std::string& out, char c) noexcept { out.push_back(c); }

#define INT_FMT(Type)                                                                              \
    void neo::ufmt_append(std::string& out, Type val) noexcept {                                   \
        char buf[32];                                                                              \
        auto res     = std::to_chars(buf, buf + sizeof(buf), val);                                 \
        auto n_chars = res.ptr - buf;                                                              \
        out.append(buf, buf + n_chars);                                                            \
    }                                                                                              \
    static_assert(true)

INT_FMT(std::int8_t);
INT_FMT(std::uint8_t);
INT_FMT(std::int16_t);
INT_FMT(std::uint16_t);
INT_FMT(std::int32_t);
INT_FMT(std::uint32_t);
INT_FMT(std::int64_t);
INT_FMT(std::uint64_t);
