#pragma once

#include "./concepts.hpp"
#include "./ufmt.hpp"

#include <cinttypes>
#include <concepts>
#include <ranges>
#include <string>
#include <tuple>

namespace neo {

namespace repr_detail {

#define DECL_TYPENAME(Type, Name)                                                                  \
    constexpr std::string_view typename_str(Type*) noexcept { return Name; }                       \
    static_assert(true)

DECL_TYPENAME(char, "char");
DECL_TYPENAME(wchar_t, "wchar_t");
DECL_TYPENAME(std::uint8_t, "u8");
DECL_TYPENAME(std::uint16_t, "u16");
DECL_TYPENAME(std::uint32_t, "u32");
DECL_TYPENAME(std::uint64_t, "u64");
DECL_TYPENAME(std::int8_t, "i8");
DECL_TYPENAME(std::int16_t, "i16");
DECL_TYPENAME(std::int32_t, "i32");
DECL_TYPENAME(std::int64_t, "i64");

#undef DECL_TYPENAME

template <typename T>
concept has_member_typename = requires {
    { T::typename_str() }
    ->std::convertible_to<std::string_view>;
};

template <typename T>
concept has_adl_typename = requires(T* ptr) {
    { typename_str(ptr) }
    ->std::convertible_to<std::string_view>;
};

template <typename T>
concept typename_str_check = has_member_typename<T> || has_adl_typename<T>;

template <typename_str_check T>
constexpr std::string_view do_repr_typename(T*) {
    if constexpr (has_member_typename<T>) {
        return T::typename_str();
    } else {
        return typename_str((T*)(0));
    }
}

}  // namespace repr_detail

template <repr_detail::typename_str_check T>
constexpr std::string_view repr_typename = repr_detail::do_repr_typename((T*)(0));

template <typename String>
class repr_writer;

struct repr_fn;

namespace repr_detail {

template <typename T>
constexpr int special_repr = 0;

template <typename T, typename Writer>
concept has_adl_repr_into = requires(Writer& out, const T& t) {
    repr_into(out, t);
};

template <typename T, typename Writer>
concept has_member_repr_into = requires(Writer& out, const T& t) {
    t.repr_into(out);
};

template <typename T, typename Writer>
concept has_special_repr_into = requires(Writer& out, const T& t) {
    special_repr<T>(out, t);
};

template <typename T, typename Writer>
concept repr_check =  //
    has_member_repr_into<
        T,
        Writer> || has_adl_repr_into<T, Writer> || has_special_repr_into<T, Writer>;

template <typename String>
class repr_writer {
    String* _out;

    template <typename T>
    constexpr void _repr_one(const T& arg) {}

public:
    constexpr explicit repr_writer(String& s)
        : _out(&s) {}

    template <formattable... Ts>
    constexpr void operator()(const Ts&... args) const {
        neo::ufmt_into(*_out, args...);
    }
};

}  // namespace repr_detail

struct repr_into_fn {
    template <typename Writer, repr_detail::repr_check<Writer> T>
    void operator()(Writer&& out, const T& arg) const {
        if constexpr (repr_detail::has_member_repr_into<T, Writer>) {
            arg.repr_into(out);
        } else if constexpr (repr_detail::has_adl_repr_into<T, Writer>) {
            repr_into(out, arg);
        } else if constexpr (repr_detail::has_special_repr_into<T, Writer>) {
            repr_detail::special_repr<T>(out, arg);
        }
    }
};

inline constexpr repr_into_fn repr_into{};

struct repr_fn {
    using _writer_type = repr_detail::repr_writer<std::string>;

    template <repr_detail::repr_check<_writer_type> T>
    constexpr std::string operator()(const T& arg) const {
        std::string  ret;
        _writer_type wr{ret};
        repr_into(wr, arg);
        return ret;
    }
};

inline constexpr repr_fn repr{};

namespace repr_detail {

template <std::integral I>
auto special_repr<I> = [](auto wr, I val) noexcept { wr("{}.{}", val, repr_typename<I>); };

template <>
auto special_repr<bool> = [](auto wr, bool val) noexcept {
    if (val) {
        wr("true");
    } else {
        wr("false");
    }
};

template <>
auto special_repr<std::string_view> = [](auto out, auto sv) noexcept { out("\"{}\"sv", sv); };

template <>
auto special_repr<std::string> = [](auto out, const std::string& s) noexcept { out("\"{}\"s", s); };

template <auto>
constexpr bool check_constexpr = true;

template <typename T>
concept detect_vector = requires(T& vec, std::ranges::range_value_t<T> item) {
    typename T::value_type;
    typename T::allocator_type;
    requires std::same_as<T, typename T::vector>;
    vec.push_back(std::move(item));
    vec.pop_back();
};

template <typename T>
concept detect_std_array = requires(T& arr, std::ranges::range_value_t<T> item) {
    arr.data();
    arr[1] = std::move(item);
    requires std::same_as<T, typename T::array>;
    check_constexpr<T().size()>;
};

template <std::ranges::forward_range R>
auto special_repr<R> = [](auto out, const R& range) noexcept {
    auto value_type_str = repr_typename<std::ranges::range_value_t<R>>;
    if constexpr (detect_vector<R>) {
        out("vector<{}>", value_type_str);
    } else if constexpr (std::is_array_v<R>) {
        out("{}[]", value_type_str);
    } else if constexpr (detect_std_array<R>) {
        out("array<{}>", value_type_str);
    } else {
        out("range<{}>", value_type_str);
    }
    out("{");
    auto end = std::ranges::cend(range);
    for (auto it = std::ranges::cbegin(range); it != end; ++it) {
        out("{}", repr(*it));
        if (std::next(it) != end) {
            out(", ");
        }
    }
    out("}");
};

template <typename Tuple>
concept tuple_like = requires {
    typename std::tuple_element_t<0, Tuple>;
    std::tuple_size_v<Tuple>;
    requires(
        requires(Tuple t) { std::get<0>(t); } ||  //
        requires(Tuple t) { t.template get<0>(); });
    requires !detect_std_array<Tuple>;
};

template <typename Pair>
concept detect_pair = tuple_like<Pair>&& requires(Pair                       p,
                                                  typename Pair::first_type  f,
                                                  typename Pair::second_type s) {
    typename Pair::first_type;
    typename Pair::second_type;
    requires std::same_as<Pair, typename Pair::pair>;
    p.first  = std::move(f);
    p.second = std::move(s);
};

template <tuple_like Tup>
auto special_repr<Tup> = [](auto out, const auto& tup) { out("tuple"); };

template <>
auto special_repr<const char*> = [](auto out, auto str) { out("\"{}\"", str); };

template <detect_pair Pair>
auto special_repr<Pair> =
    [](auto out, const auto& pair) { out("pair{{}, {}}", repr(pair.first), repr(pair.second)); };

}  // namespace repr_detail

template <typename T>
concept reprable = requires(T&& t) {
    neo::repr(NEO_FWD(t));
};

}  // namespace neo
