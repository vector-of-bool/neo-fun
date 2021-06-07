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

template <typename>
struct tag {};

template <typename T, bool WantType>
class value_writer {
    std::string* _out;
    const T*     _value;

public:
    enum { want_type = WantType, want_value = true };

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
    enum { want_type = true, want_value = false };
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
concept has_adl_repr_into = requires(type_writer& out, T* t) {
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

}  // namespace repr_detail

template <repr_detail::repr_check T>
constexpr auto repr_type() noexcept {
    std::string              ret;
    repr_detail::type_writer wr{ret};
    if constexpr (repr_detail::has_adl_repr_into<T>) {
        repr_into(wr, (T*)(nullptr));
    } else {
        using repr_detail::repr_fallback;
        repr_fallback(wr, repr_detail::tag<T>{});
    }
    return ret;
}

template <repr_detail::repr_check T>
constexpr auto repr_value(const T& value) noexcept {
    std::string                         ret;
    repr_detail::value_writer<T, false> wr{ret, value};
    if constexpr (repr_detail::has_adl_repr_into<T>) {
        repr_into(wr, (T*)(nullptr));
    } else {
        using repr_detail::repr_fallback;
        repr_fallback(wr, repr_detail::tag<T>{});
    }
    return ret;
}

/// Function object type for neo::repr
struct repr_fn {
    template <repr_detail::repr_check T>
    constexpr std::string operator()(const T& arg) const {
        std::string ret;
        // Write both type and value
        repr_detail::value_writer<T, true> wr{ret, arg};
        if constexpr (repr_detail::has_adl_repr_into<T>) {
            repr_into(wr, std::addressof(arg));
        } else {
            using repr_detail::repr_fallback;
            repr_fallback(wr, repr_detail::tag<T>{});
        }
        return ret;
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
    static_assert(!out.want_value);
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

template <reprable T>
constexpr void repr_fallback(auto out, tag<T*>) noexcept {
    if constexpr (out.want_type) {
        if constexpr (out.want_value) {
            out("[{} {}]", repr_type<T*>(), repr_value(out.value()));
        } else {
            out("{}*", repr_type<T>());
        }
    } else if constexpr (out.want_value) {
        if (out.value() != nullptr) {
            out("[{}]", repr_value(*out.value()));
        } else {
            out("nullptr");
        }
    }
}

#define DECL_REPR_TYPE_CASE(Type, TypeName)                                                        \
    else if constexpr (std::same_as<Integral, Type>) {                                             \
        if constexpr (out.want_type && out.want_value) {                                           \
            out("{}:{}", repr_value(out.value()), TypeName);                                       \
        } else if constexpr (out.want_value) {                                                     \
            out("{}", out.value());                                                                \
        } else if constexpr (out.want_type) {                                                      \
            out("{}", TypeName);                                                                   \
        }                                                                                          \
    }

template <std::integral Integral>
requires(!std::is_pointer_v<Integral> && std::same_as<Integral, std::remove_cvref_t<Integral>>)  //
    constexpr void repr_fallback(auto out, tag<Integral>) noexcept {
    if constexpr (std::same_as<Integral, bool>) {
        if (out.want_value) {
            out(out.value() ? "true" : "false");
        } else {
            out("bool");
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

template <typename Pair>
requires requires {
    requires detect_pair<Pair>;
    requires reprable<typename Pair::first_type>;
    requires reprable<typename Pair::second_type>;
}
constexpr void repr_fallback(auto out, tag<Pair>) noexcept {
    if constexpr (out.want_type) {
        if constexpr (out.want_value) {
            out("pair{{}, {}}", repr(out.value().first), repr(out.value().second));
        } else {
            out("pair<{}, {}>",
                repr_type<typename Pair::first_type>(),
                repr_type<typename Pair::second_type>());
        }
    } else {
        out("{{}, {}}", repr_value(out.value().first), repr_value(out.value().second));
    }
}

template <typename Optional>
requires requires {
    requires detect_optional<Optional>;
    requires reprable<typename Optional::value_type>;
}
constexpr void repr_fallback(auto out, tag<Optional>) noexcept {
    if constexpr (out.want_type) {
        if constexpr (out.want_value) {
            out("[optional<{}>", repr_type<typename Optional::value_type>());
            if (out.value().has_value()) {
                out(" [{}]]", out.value().value());
            } else {
                out(" nullopt]");
            }
        } else {
            out("optional<{}>", repr_type<typename Optional::value_type>());
        }
    } else if constexpr (out.want_value) {
        if (out.value().has_value()) {
            out("{}", repr_value(out.value().value()));
        } else {
            out("nullopt");
        }
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

constexpr void repr_fallback(auto out, tag<neo::alike<std::string> auto>) noexcept {
    if constexpr (out.want_value) {
        repr_write_string_val(out, out.value());
        if constexpr (out.want_type) {
            out("s");
        }
    } else if constexpr (out.want_type) {
        out("std::string");
    }
}

constexpr void repr_fallback(auto out, tag<neo::alike<std::string_view> auto>) noexcept {
    if constexpr (out.want_value) {
        repr_write_string_val(out, out.value());
        if constexpr (out.want_type) {
            out("sv");
        }
    } else if constexpr (out.want_type) {
        out("std::string_view");
    }
}

template <typename Map>
requires requires {
    requires detect_map<Map>;
    requires reprable<typename Map::key_type>;
    requires reprable<typename Map::mapped_type>;
}
constexpr void repr_fallback(auto out, tag<Map>) noexcept {
    if constexpr (out.want_type) {
        out("map<{}, {}>",
            repr_type<typename Map::key_type>(),
            repr_type<typename Map::mapped_type>());
    }
    if constexpr (out.want_value) {
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
    requires reprable<std::ranges::range_value_t<R>>;
}
constexpr void repr_fallback(auto out, tag<R>) noexcept {
    if constexpr (out.want_type) {
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
    if constexpr (out.want_value) {
        out("{");
        auto end = std::ranges::cend(out.value());
        for (auto it = std::ranges::cbegin(out.value()); it != end; ++it) {
            if constexpr (out.want_type) {
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
    if (out.want_type) {
        if (out.want_value) {
            out("char[]\"{}\"", out.value());
        } else {
            out("const char*");
        }
    } else {
        out("\"{}\"", out.value());
    }
}

}  // namespace repr_detail

}  // namespace neo
