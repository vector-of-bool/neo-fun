#pragma once

#include "./concepts.hpp"
#include "./ufmt.hpp"

#include <charconv>
#include <cinttypes>
#include <concepts>
#include <ranges>
#include <string>
#include <tuple>

namespace neo {

namespace repr_detail {

template <typename>
struct tag {};

template <typename T, bool WantType>
class value_writer {
    std::string* _out;
    const T*     _value;

public:
    enum { just_type = false, just_value = not WantType };

    constexpr explicit value_writer(std::string& out, const T& val)
        : _out(&out)
        , _value(std::addressof(val)) {}

    const T& value() const noexcept { return *_value; }

    template <formattable... Ts>
    constexpr void operator()(const Ts&... args) const {
        neo::ufmt_into(*_out, args...);
    }
};

class type_writer {
    std::string* _out;

public:
    enum { just_type = true, just_value = false };
    constexpr explicit type_writer(std::string& out)
        : _out(&out) {}

    template <formattable... Ts>
    constexpr void operator()(const Ts&... args) const {
        neo::ufmt_into(*_out, args...);
    }
};

template <typename T>
struct strip_all_cvrptr {
    using type = T;
};

template <typename T>
requires(!std::same_as<T, std::remove_cvref_t<T>>)  //
    struct strip_all_cvrptr<T> : strip_all_cvrptr<std::remove_cvref_t<T>> {};

template <typename T>
struct strip_all_cvrptr<T*> : strip_all_cvrptr<T> {};

template <typename T>
using strip_all_cvrptr_t = strip_all_cvrptr<T>::type;

// Give these names, even though they are unused
void repr_fallback();
void repr_into();

/// Check if we have a non-member ADL-visible repr_into()
template <typename T>
concept has_adl_repr_into = requires(type_writer& out, const T* t) {
    repr_into(out, t);
};

/// Check if we have a fallback function for this one
template <typename T>
concept has_repr_fallback = requires(type_writer& out, tag<T> t) {
    repr_fallback(out, t);
};

/// Check that we can write a repr of T via ADL, member, or a fallback
template <typename T>
concept repr_check = has_adl_repr_into<T> || has_repr_fallback<T>;

template <typename T>
struct type_repr {
    std::string string() const noexcept { return neo::ufmt("{}", *this); }
};

template <typename T, bool WantType>
struct value_repr {
    const T&    value;
    std::string string() const noexcept { return neo::ufmt("{}", *this); }

    friend std::ostream& operator<<(std::ostream& out, value_repr self) noexcept {
        out << self.string();
        return out;
    }
};

template <typename T>
void ufmt_append(std::string& out, type_repr<T>) noexcept {
    repr_detail::type_writer wr{out};
    if constexpr (repr_detail::has_adl_repr_into<T>) {
        repr_into(wr, (T*)(nullptr));
    } else {
        using repr_detail::repr_fallback;
        repr_fallback(wr, repr_detail::tag<T>{});
    }
}

template <typename T, bool WantType>
void ufmt_append(std::string& out, value_repr<T, WantType> r) noexcept {
    repr_detail::value_writer<T, WantType> wr{out, r.value};
    if constexpr (repr_detail::has_adl_repr_into<T>) {
        repr_into(wr, std::addressof(r.value));
    } else {
        using repr_detail::repr_fallback;
        repr_fallback(wr, repr_detail::tag<T>{});
    }
}

}  // namespace repr_detail

template <repr_detail::repr_check T>
constexpr auto repr_type() noexcept {
    return repr_detail::type_repr<T>{};
}

template <repr_detail::repr_check T>
constexpr auto repr_value(const T& value) noexcept {
    return repr_detail::value_repr<T, false>{value};
}

/// Function object type for neo::repr
struct repr_fn {
    template <repr_detail::repr_check T>
    constexpr auto operator()(const T& arg) const {
        return repr_detail::value_repr<T, true>{arg};
        // std::string ret;
        // // Write both type and value
        // repr_detail::value_writer<T, true> wr{ret, arg};
        // if constexpr (repr_detail::has_adl_repr_into<T>) {
        //     repr_into(wr, std::addressof(arg));
        // } else {
        //     using repr_detail::repr_fallback;
        //     repr_fallback(wr, repr_detail::tag<T>{});
        // }
        // return ret;
    }
};

/**
 * @brief Generate a string representation of the given object, including its type information
 */
inline constexpr repr_fn repr{};

/// Constraint to a type that is valid for neo::repr()
template <typename T>
concept reprable = repr_detail::repr_check<std::remove_cvref_t<T>>;

namespace repr_detail {

template <typename T>
requires(!std::same_as<T, std::remove_cvref_t<T>> && repr_check<std::remove_cvref_t<T>>)  //
    constexpr void repr_fallback(auto out, tag<T>) noexcept {
    static_assert(out.just_type);
    if constexpr (std::is_const_v<T>) {
        out("{} const", repr_type<std::remove_const_t<T>>());
    } else if constexpr (std::is_volatile_v<T>) {
        out("{} volatile", repr_type<std::remove_volatile_t<T>>());
    } else if constexpr (std::is_lvalue_reference_v<T>) {
        out("{}&", repr_type<std::remove_reference_t<T>>());
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        out("{}&&", repr_type<std::remove_reference_t<T>>());
    }
}

template <typename T>
constexpr void repr_fallback(auto out, tag<T*>) noexcept {
    if constexpr (std::is_void_v<T>) {
        if constexpr (out.just_type) {
            out("void*");
        } else {
            if (out.value() == nullptr) {
                out("nullptr");
            } else {
                char             buf[32];
                const auto       ptrv = reinterpret_cast<std::intptr_t>(out.value());
                const auto       res  = std::to_chars(buf, buf + 32, ptrv, 16);
                std::string_view view{buf, static_cast<std::size_t>(res.ptr - buf)};
                out("0x{}", view);
            }
        }
    } else if constexpr (out.just_type) {
        if constexpr (reprable<T>) {
            out("{}*", repr_type<T>());
        } else {
            out("unknown-pointer");
        }
    } else if constexpr (out.just_value) {
        if constexpr (reprable<T>) {
            if (out.value() != nullptr) {
                out("[{}]", repr_value(*out.value()));
            } else {
                out("nullptr");
            }
        } else {
            out("{}", repr_value(static_cast<const void*>(out.value())));
        }
    } else {
        out("[{} {}]", repr_type<T*>(), repr_value(out.value()));
    }
}

#define DECL_REPR_TYPE_CASE(Type, TypeName)                                                        \
    else if constexpr (std::same_as<Integral, Type>) {                                             \
        if constexpr (out.just_type) {                                                             \
            out(TypeName);                                                                         \
        } else if constexpr (out.just_value) {                                                     \
            out("{}", out.value());                                                                \
        } else {                                                                                   \
            out("{}:{}", repr_value(out.value()), TypeName);                                       \
        }                                                                                          \
    }

template <std::integral Integral>
requires(!std::is_pointer_v<Integral> && std::same_as<Integral, std::remove_cvref_t<Integral>>)  //
    constexpr void repr_fallback(auto out, tag<Integral>) noexcept {
    if constexpr (std::same_as<Integral, bool>) {
        if constexpr (out.just_type) {
            out("bool");
        } else {
            out(out.value() ? "true" : "false");
        }
    }
    DECL_REPR_TYPE_CASE(char, "char")
    DECL_REPR_TYPE_CASE(wchar_t, "wchar_t")
    DECL_REPR_TYPE_CASE(std::uint8_t, "uint8")
    DECL_REPR_TYPE_CASE(std::uint16_t, "uint16")
    DECL_REPR_TYPE_CASE(std::uint32_t, "uint32")
    DECL_REPR_TYPE_CASE(std::uint64_t, "uint64")
    DECL_REPR_TYPE_CASE(std::int8_t, "int8")
    DECL_REPR_TYPE_CASE(std::int16_t, "int16")
    DECL_REPR_TYPE_CASE(std::int32_t, "int32")
    DECL_REPR_TYPE_CASE(std::int64_t, "int64")
    else {
        static_assert(std::is_void_v<Integral>, "Unhandled built-in integral type");
    }
}

#undef DECL_REPR_TYPE_CASE

template <auto>
constexpr bool check_constexpr = true;

template <typename T>
concept detect_vector = requires(T vec, std::ranges::range_value_t<T> item) {
    typename T::value_type;
    typename T::allocator_type;
    requires std::same_as<T, typename T::vector>;
    vec.push_back(std::move(item));
    vec.pop_back();
};

template <typename T>
concept forward_range_ = std::ranges::forward_range<T>;

template <typename T>
concept detect_map = forward_range_<T>  //
    && requires(T& map, std::ranges::range_value_t<T> pair, typename T::key_type key) {
    typename T::key_type;
    typename T::mapped_type;
    map.insert(pair);
    map.find(key);
};

template <typename T>
concept detect_std_array = requires(T arr, std::ranges::range_value_t<T> item) {
    arr.data();
    requires std::same_as<T, typename T::array>;
    check_constexpr<T().size()>;
};

template <typename Tuple>
concept detect_tuple = requires {
    requires(
        requires(Tuple t) { std::get<0>(t); } ||  //
        requires(Tuple t) { t.template get<0>(); });
    typename std::tuple_element_t<0, Tuple>;
    std::tuple_size_v<Tuple>;
    requires !detect_std_array<Tuple>;
};

template <typename Pair>
concept detect_pair = detect_tuple<Pair>&& requires(std::remove_cvref_t<Pair>& p,
                                                    typename Pair::first_type  f,
                                                    typename Pair::second_type s) {
    typename Pair::first_type;
    typename Pair::second_type;
    requires std::same_as<Pair, typename Pair::pair>;
    requires alike<decltype(p.first), typename Pair::first_type>;
    requires alike<decltype(p.second), typename Pair::second_type>;
};

template <typename Opt>
concept detect_optional = requires(Opt opt) {
    typename Opt::value_type;
    {opt.has_value()};
    {opt.value()};
    {*opt};
    {opt.reset()};
};

template <typename Path>
concept detect_path = requires(Path path) {
    std::same_as<typename Path::path, Path>;
    path.native();
    path.generic_string();
    path.make_preferred();
    path.root_name();
    path.root_directory();
    path.root_path();
    path.filename();
    path.stem();
    path.extension();
};

template <detect_path Path>
constexpr void repr_fallback(auto out, tag<Path>) noexcept {
    if constexpr (out.just_type) {
        out("path");
    } else if constexpr (out.just_value) {
        out("{}", repr_value(out.value().string()));
    } else {
        out("[path {}]", repr_value(out.value()));
    }
}

template <typename Pair>
requires requires {
    requires detect_pair<Pair>;
    requires reprable<typename Pair::first_type>;
    requires reprable<typename Pair::second_type>;
}
constexpr void repr_fallback(auto out, tag<Pair>) noexcept {
    if constexpr (out.just_type) {
        out("pair<{}, {}>",
            repr_type<typename Pair::first_type>(),
            repr_type<typename Pair::second_type>());
    } else if constexpr (out.just_value) {
        out("{{}, {}}", repr_value(out.value().first), repr_value(out.value().second));
    } else {
        out("pair{{}, {}}", repr(out.value().first), repr(out.value().second));
    }
}

template <typename Optional>
requires requires {
    requires detect_optional<Optional>;
    requires reprable<typename Optional::value_type>;
}
constexpr void repr_fallback(auto out, tag<Optional>) noexcept {
    if constexpr (out.just_type) {
        out("optional<{}>", repr_type<typename Optional::value_type>());
    } else if constexpr (out.just_value) {
        if (out.value().has_value()) {
            out("[{}]", repr_value(out.value().value()));
        } else {
            out("nullopt");
        }
    } else {
        out("[{} {}]", repr_type<Optional>(), repr_value(out.value()));
    }
}

constexpr void repr_write_string_val(auto out, std::string_view sv) noexcept {
    out("\"");
    while (!sv.empty()) {
        auto qpos = sv.find('"');
        auto head = sv.substr(0, qpos);
        out("{}", head);
        if (qpos != sv.npos) {
            out("\\\"");
            sv = sv.substr(qpos + 1);
        } else {
            break;
        }
    }
    out("\"");
}

template <alike<std::string> StdString>
constexpr void repr_fallback(auto out, tag<StdString>) noexcept {
    if constexpr (out.just_type) {
        out("std::string");
    } else {
        repr_write_string_val(out, out.value());
        if constexpr (not out.just_value) {
            out("s");
        }
    }
}

template <alike<std::string_view> StdStringView>
constexpr void repr_fallback(auto out, tag<StdStringView>) noexcept {
    if constexpr (out.just_type) {
        out("std::string_view");
    } else {
        repr_write_string_val(out, out.value());
        if constexpr (not out.just_value) {
            out("sv");
        }
    }
}

template <typename Map>
requires requires {
    requires detect_map<Map>;
    requires reprable<typename Map::key_type>;
    requires reprable<typename Map::mapped_type>;
}
constexpr void repr_fallback(auto out, tag<Map>) noexcept {
    if constexpr (not out.just_value) {
        out("map<{}, {}>",
            repr_type<typename Map::key_type>(),
            repr_type<typename Map::mapped_type>());
    }
    if constexpr (not out.just_type) {
        out("{");
        auto& map  = out.value();
        auto  iter = std::ranges::begin(map);
        auto  end  = std::ranges::end(map);
        while (iter != end) {
            auto& pair               = *iter;
            const auto& [key, value] = pair;
            out("[{} => {}]", repr_value(key), repr_value(value));
            ++iter;
            if (iter != end) {
                out(", ");
            }
        }
        out("}");
    }
}

template <typename R>
requires requires {
    requires forward_range_<R>;
    requires !alike<std::string, R> && !alike<std::string_view, R>;
    requires !detect_map<R>;
    requires !detect_path<R>;
    requires reprable<std::ranges::range_value_t<R>>;
}
constexpr void repr_fallback(auto out, tag<R>) noexcept {
    if constexpr (not out.just_value) {
        auto value_type_str = repr_type<std::ranges::range_value_t<R>>();
        if constexpr (detect_vector<R>) {
            out("vector<{}>", value_type_str);
        } else if constexpr (std::is_array_v<R>) {
            out("{}[]", value_type_str);
        } else if constexpr (detect_std_array<R>) {
            out("array<{}>", value_type_str);
        } else {
            out("range<{}>", value_type_str);
        }
    }
    if constexpr (not out.just_type) {
        out("{");
        auto end = std::ranges::cend(out.value());
        for (auto it = std::ranges::cbegin(out.value()); it != end; ++it) {
            if constexpr (not out.just_value) {
                out("{}", repr_value(*it));
            } else {
                out("{}", repr(*it));
            }
            if (std::next(it) != end) {
                out(", ");
            }
        }
        out("}");
    }
}

constexpr void tuple_out_next(auto, auto, const auto&) noexcept {}

template <std::size_t First, std::size_t... Idx>
constexpr void tuple_out_next(auto out, std::index_sequence<First, Idx...>, const auto& tup) {
    out("{}", repr(std::get<First>(tup)));
    if (sizeof...(Idx) != 0) {
        out(", ");
        tuple_out_next(out, std::index_sequence<Idx...>{}, tup);
    }
}

template <typename Tup, std::size_t... Seq>
void check_tuple_can_repr(const Tup&, std::index_sequence<Seq...>) requires(
    (reprable<std::tuple_element_t<Seq, Tup>>)&&...) {}

template <typename Tuple>
requires requires(const Tuple& tup) {
    requires detect_tuple<Tuple>;
    requires !detect_pair<Tuple>;
    check_tuple_can_repr(tup, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}
constexpr void repr_fallback(auto out, const Tuple& tup) noexcept {
    out("tuple{");
    tuple_out_next(out, std::make_index_sequence<std::tuple_size_v<Tuple>>{}, tup);
    out("}");
}

constexpr void repr_fallback(auto out, tag<const char*>) noexcept {
    if constexpr (out.just_type) {
        out("const char*");
    } else if constexpr (out.just_value) {
        out("{}", repr_value(std::string_view(out.value())));
    } else {
        out("char[]{}", repr_value(std::string_view(out.value())));
    }
}

}  // namespace repr_detail

}  // namespace neo
