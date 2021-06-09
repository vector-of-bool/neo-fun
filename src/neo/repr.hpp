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

// First declare the basic interface of a repr()-able type
namespace repr_detail {

// Tag dispatch helper for certain types
template <typename>
struct tag {};

// Type that is passed to the repr() extension points
class repr_writer;

// decl a name that can be using'd below
void repr_fallback();

/// Check if we have a non-member ADL-visible do_repr()
template <typename T>
concept has_adl_do_repr = requires(repr_writer& out, const T* t) {
    do_repr(out, t);  //! READ THIS IF YOU SEE AN ERROR HERE:
    // This concept detects if there is an ADL-visible overload of `do_repr()`
    // for the given type `T`. `do_repr()` should be a non-member function that
    // takes a deduced parameter `out` and a pointer-to-const of `T`. Refer to the top of this file
    // on how to write a do_repr() function.
};

/// Check if we have a fallback function for this one
template <typename T>
constexpr bool has_repr_fallback = requires(repr_writer& out, tag<T> t, const T* obj) {
    repr_fallback(out, t, obj);
};

/// Check that we can write a repr of T via ADL, member, or a fallback
template <typename T>
concept reprable_impl = has_adl_do_repr<T> || has_repr_fallback<T>;

class repr_writer {
    std::string* _out;

public:
    explicit repr_writer(std::string& out)
        : _out(&out) {}

    template <formattable... Ts>
    constexpr void append(std::string_view fmt_str, const Ts&... args) const {
        neo::ufmt_into(*_out, fmt_str, args...);
    }

    template <reprable_impl T>
    constexpr auto repr_type() const noexcept;

    template <reprable_impl T>
    constexpr auto repr_value(const T& arg) const noexcept;

    template <reprable_impl T>
    constexpr auto repr(const T& arg) const noexcept;

    template <typename T>
    constexpr static bool can_repr = reprable_impl<T>;

    constexpr std::string& underlying_string() noexcept { return *_out; }
};

/// Impl class that actually determines which aspects of an object we wish to repr()
template <bool WantValue, bool WantType>
class repr_writer_impl : public repr_writer {
public:
    enum {
        just_type  = WantType and not WantValue,
        just_value = WantValue and not WantType,
    };
    using repr_writer::repr_writer;
};

/**
 * @brief Return type of repr_type(). Formats and streams into a representation of the given type
 *
 * @tparam T The type to represent
 */
template <reprable_impl T>
struct type_repr {
    /// Realize the type's repr() as a std::string
    constexpr std::string string() const noexcept { return neo::ufmt("{}", *this); }

    /// Serialize the repr() of the type to an output stream
    friend std::ostream& operator<<(std::ostream& out, type_repr self) noexcept {
        out << self.string();
        return out;
    }

    /// Serialize the repr() of the type to a ufmt() string
    constexpr friend void ufmt_append(std::string& out, type_repr) noexcept {
        repr_detail::repr_writer_impl<false, true> wr{out};
        if constexpr (repr_detail::has_adl_do_repr<T>) {
            do_repr(wr, (T*)(nullptr));
        } else {
            using repr_detail::repr_fallback;
            repr_fallback(wr, repr_detail::tag<T>{}, (const T*)(0));
        }
    }
};

/**
 * @brief Return type of repr() and repr_value(). Formats and streams into a representaton of the
 * given value
 *
 * @tparam T The type which is being represented
 * @tparam WantType Whether we should include type information in the representation
 */
template <reprable_impl T, bool WantType>
struct value_repr {
    /// The value that is being repr()'d
    const T& value;
    /// Realize the repr() of the given value as a string
    std::string string() const noexcept { return neo::ufmt("{}", *this); }

    /// Write the repr() of the value to the given stream
    friend std::ostream& operator<<(std::ostream& out, value_repr self) noexcept {
        out << self.string();
        return out;
    }

    /// Append the repr() of the value to the given ufmt() string
    constexpr friend void ufmt_append(std::string& out, value_repr self) noexcept {
        repr_detail::repr_writer_impl<true, WantType> wr{out};
        if constexpr (repr_detail::has_adl_do_repr<T>) {
            do_repr(wr, std::addressof(self.value));
        } else {
            using repr_detail::repr_fallback;
            repr_fallback(wr, repr_detail::tag<T>{}, std::addressof(self.value));
        }
    }
};

}  // namespace repr_detail

/**
 * @brief Concept that represents a type that is valid for passing to repr()
 */
template <typename T>
concept reprable = repr_detail::reprable_impl<std::remove_cvref_t<T>>;

/// Variable-template version of reprable<T>
template <typename T>
constexpr bool reprable_v = reprable<T>;

/**
 * @brief Generate a repr() of the type given as the sole template parameter
 *
 * @tparam T The type of which to generate a representation
 */
template <reprable T>
constexpr auto repr_type() noexcept {
    return repr_detail::type_repr<T>{};
}

/// Function object type for neo::repr_value
struct repr_value_fn {
    template <reprable T>
    [[nodiscard]] constexpr auto operator()(const T& value) const noexcept {
        return repr_detail::value_repr<T, false>{value};
    }
};

/**
 * @brief Generate a repr() of the given value, omitting type information
 *
 * @param value The value of which to generate a representation
 */
inline constexpr repr_value_fn repr_value;

/// Function object type for neo::repr
struct repr_fn {
    template <reprable T>
    [[nodiscard]] constexpr auto operator()(const T& arg) const noexcept {
        return repr_detail::value_repr<T, true>{arg};
    }
};

/**
 * @brief Generate a string representation of the given object, including its type information
 */
inline constexpr repr_fn repr{};

namespace repr_detail {

template <reprable_impl T>
constexpr auto repr_writer::repr_type() const noexcept {
    return neo::repr_type<T>();
}

template <reprable_impl T>
constexpr auto repr_writer::repr_value(const T& arg) const noexcept {
    return neo::repr_value(arg);
}

template <reprable_impl T>
constexpr auto repr_writer::repr(const T& arg) const noexcept {
    return neo::repr(arg);
}

/// repr a cvr-qualified `T` with its qualifiers
template <typename T>
requires(!std::same_as<T, std::remove_cvref_t<T>> && reprable_impl<std::remove_cvref_t<T>>)  //
    constexpr void repr_fallback(auto out, tag<T>, auto*) noexcept {
    static_assert(out.just_type);
    if constexpr (std::is_const_v<T>) {
        out.append("{} const", repr_type<std::remove_const_t<T>>());
    } else if constexpr (std::is_volatile_v<T>) {
        out.append("{} volatile", repr_type<std::remove_volatile_t<T>>());
    } else if constexpr (std::is_lvalue_reference_v<T>) {
        out.append("{}&", repr_type<std::remove_reference_t<T>>());
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        out.append("{}&&", repr_type<std::remove_reference_t<T>>());
    }
}

/// Handle 'void'
constexpr void repr_fallback(auto out, tag<void>, auto*) noexcept {
    static_assert(out.just_type);
    out.append("void");
}

/// repr() a pointer of any type. If the pointer is non-null and the pointed-to-type can be
/// repr()'d, includes the repr() of the pointed-to value
template <typename T>
constexpr void repr_fallback(auto out, tag<T*>, auto* value) noexcept {
    if constexpr (std::is_void_v<T>) {
        // Special case for void*
        if constexpr (out.just_type) {
            out.append("{}*", repr_type<T>());
        } else if constexpr (out.just_value) {
            if (*value == nullptr) {
                out.append("nullptr");
            } else {
                // We don't know what we are pointing to, but we
                // can repr the pointer as an address
                char             buf[32];
                const auto       ptrv = reinterpret_cast<std::intptr_t>(*value);
                const auto       res  = std::to_chars(buf, buf + 32, ptrv, 16);
                std::string_view view{buf, static_cast<std::size_t>(res.ptr - buf)};
                out.append("0x{}", view);
            }
        } else {
            out.append("[{}* {}]", repr_type<T>(), repr_value(*value));
        }
    } else if constexpr (out.just_type) {
        if constexpr (out.template can_repr<T>) {
            out.append("{}*", repr_type<T>());
        } else {
            out.append("unknown-pointer");
        }
    } else if constexpr (out.just_value) {
        if constexpr (out.template can_repr<T>) {
            if (*value != nullptr) {
                // Wrap the value in brackets to represent the de-referencing
                out.append("[{}]", repr_value(**value));
            } else {
                out.append("nullptr");
            }
        } else {
            out.append("{}", repr_value(static_cast<const void*>(*value)));
        }
    } else {
        out.append("[{} {}]", repr_type<T*>(), repr_value(*value));
    }
}

#define DECL_REPR_TYPE_CASE(Type, TypeName)                                                        \
    else if constexpr (std::same_as<Integral, Type>) {                                             \
        if constexpr (out.just_type) {                                                             \
            out.append(TypeName);                                                                  \
        } else if constexpr (out.just_value) {                                                     \
            out.append("{}", *value);                                                              \
        } else {                                                                                   \
            out.append("{}:{}", repr_value(*value), TypeName);                                     \
        }                                                                                          \
    }

template <std::integral Integral>
requires(!std::is_pointer_v<Integral> && std::same_as<Integral, std::remove_cvref_t<Integral>>)  //
    constexpr void repr_fallback(auto out, tag<Integral>, auto* value) noexcept {
    if constexpr (std::same_as<Integral, bool>) {
        if constexpr (out.just_type) {
            out.append("bool");
        } else {
            out.append(*value ? "true" : "false");
        }
    } else if constexpr (std::same_as<Integral, char>) {
        if constexpr (out.just_type) {
            out.append("char");
        } else {
            out.append("'{}'", *value);
        }
    } else if constexpr (std::same_as<Integral, wchar_t>) {
        if constexpr (out.just_type) {
            out.append("wchar_t");
        } else {
            out.append("L'{}'", *value);
        }
    } else if constexpr (std::same_as<Integral, char8_t>) {
        if constexpr (out.just_type) {
            out.append("char8_t");
        } else {
            out.append("u8'{}'", *value);
        }
    } else if constexpr (std::same_as<Integral, char16_t>) {
        if constexpr (out.just_type) {
            out.append("char16_t");
        } else {
            out.append("u'{}'", *value);
        }
    } else if constexpr (std::same_as<Integral, char32_t>) {
        if constexpr (out.just_type) {
            out.append("char32_t");
        } else {
            out.append("U'{}'", *value);
        }
    }
    DECL_REPR_TYPE_CASE(std::uint8_t, "uint8")
    DECL_REPR_TYPE_CASE(std::uint16_t, "uint16")
    DECL_REPR_TYPE_CASE(std::uint32_t, "uint32")
    DECL_REPR_TYPE_CASE(std::uint64_t, "uint64")
    DECL_REPR_TYPE_CASE(std::int8_t, "int8")
    DECL_REPR_TYPE_CASE(std::int16_t, "int16")
    DECL_REPR_TYPE_CASE(std::int32_t, "int32")
    DECL_REPR_TYPE_CASE(std::int64_t, "int64")
    DECL_REPR_TYPE_CASE(std::size_t, "size_t")  // Some seem to have size_t defined differently??
    else {
        static_assert(std::is_void_v<Integral>, "Unhandled built-in integral type");
    }
}

#undef DECL_REPR_TYPE_CASE

template <typename T>
concept detect_vector = requires(T vec, std::ranges::range_value_t<T> item) {
    typename T::value_type;
    typename T::allocator_type;
    requires std::same_as<T, typename T::vector>;
    vec.push_back(std::move(item));
    vec.pop_back();
};

template <typename T>
concept detect_map = std::ranges::forward_range<T>  //
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
    typename Path::path;
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
constexpr void repr_fallback(auto out, tag<Path>, auto* value) noexcept {
    if constexpr (out.just_type) {
        out.append("path");
    } else if constexpr (out.just_value) {
        out.append("{}", repr_value(value->string()));
    } else {
        out.append("[path {}]", repr_value(*value));
    }
}

template <typename Pair>
requires requires {
    requires detect_pair<Pair>;
    requires reprable<typename Pair::first_type>;
    requires reprable<typename Pair::second_type>;
}
constexpr void repr_fallback(auto out, tag<Pair>, auto* value) noexcept {
    if constexpr (out.just_type) {
        out.append("pair<{}, {}>",
                   repr_type<typename Pair::first_type>(),
                   repr_type<typename Pair::second_type>());
    } else if constexpr (out.just_value) {
        out.append("{{}, {}}", repr_value(value->first), repr_value(value->second));
    } else {
        out.append("pair{{}, {}}", repr(value->first), repr(value->second));
    }
}

template <typename Optional>
requires requires {
    requires detect_optional<Optional>;
    requires reprable<typename Optional::value_type>;
}
constexpr void repr_fallback(auto out, tag<Optional>, auto* value) noexcept {
    if constexpr (out.just_type) {
        out.append("optional<{}>", repr_type<typename Optional::value_type>());
    } else if constexpr (out.just_value) {
        if (value->has_value()) {
            out.append("[{}]", repr_value(value->value()));
        } else {
            out.append("nullopt");
        }
    } else {
        out.append("[{} {}]", repr_type<Optional>(), repr_value(*value));
    }
}

template <typename Char, typename Traits>
constexpr void repr_write_string_val(auto out, std::basic_string_view<Char, Traits> sv) noexcept {
    if constexpr (same_as<Char, char>) {
        // No prefix
    } else if constexpr (same_as<Char, wchar_t>) {
        out.append("L");
    } else if constexpr (same_as<Char, char8_t>) {
        out.append("u8");
    } else if constexpr (same_as<Char, char16_t>) {
        out.append("u");
    } else if constexpr (same_as<Char, char32_t>) {
        out.append("U");
    }
    out.append("\"");
    while (!sv.empty()) {
        auto qpos = sv.find('"');
        auto head = sv.substr(0, qpos);
        out.append("{}", head);
        if (qpos != sv.npos) {
            out.append("\\\"");
            sv = sv.substr(qpos + 1);
        } else {
            break;
        }
    }
    out.append("\"");
}

template <typename Char, typename Traits>
constexpr void
repr_fallback(auto out, tag<std::basic_string_view<Char, Traits>>, auto* value) noexcept {
    using view = std::basic_string_view<Char, Traits>;
    using std::same_as;
    if constexpr (out.just_type) {
        if constexpr (same_as<view, std::string_view>) {
            out.append("std::string_view");
        } else if constexpr (same_as<view, std::wstring_view>) {
            out.append("std::wstring_view");
        } else if constexpr (same_as<view, std::u8string_view>) {
            out.append("std::u8string_view");
        } else if constexpr (same_as<view, std::u16string_view>) {
            out.append("std::16string_view");
        } else if constexpr (same_as<view, std::u32string_view>) {
            out.append("std::u32string_view");
        } else {
            out.append("std::basic_string_view<{}, [traits]>", repr_type<Char>());
        }
    } else {
        repr_write_string_val(out, *value);
        if constexpr (not out.just_value) {
            out.append("sv");
        }
    }
}

template <typename Char, typename Traits, typename Alloc>
requires requires {
    reprable<Char>;
}
constexpr void
repr_fallback(auto out, tag<std::basic_string<Char, Traits, Alloc>>, auto* value) noexcept {
    using string = std::basic_string<Char, Traits, Alloc>;
    using std::same_as;
    if constexpr (out.just_type) {
        if constexpr (same_as<string, std::string>) {
            out.append("std::string");
        } else if constexpr (same_as<string, std::wstring>) {
            out.append("std::wstring");
        } else if constexpr (same_as<string, std::u8string>) {
            out.append("std::u8string");
        } else if constexpr (same_as<string, std::u16string>) {
            out.append("std::16string");
        } else if constexpr (same_as<string, std::u32string>) {
            out.append("std::u32string");
        } else {
            out.append("std::basic_string<{}, [traits], [alloc]>", repr_type<Char>());
        }
    } else {
        repr_write_string_val(out, std::basic_string_view<Char, Traits>(*value));
        if constexpr (not out.just_value) {
            out.append("s");
        }
    }
}

template <typename Map>
requires requires {
    requires detect_map<Map>;
    requires reprable<typename Map::key_type>;
    requires reprable<typename Map::mapped_type>;
}
constexpr void repr_fallback(auto out, tag<Map>, auto* value) noexcept {
    if constexpr (not out.just_value) {
        out.append("map<{}, {}>",
                   repr_type<typename Map::key_type>(),
                   repr_type<typename Map::mapped_type>());
    }
    if constexpr (not out.just_type) {
        out.append("{");
        auto& map  = *value;
        auto  iter = std::ranges::begin(map);
        auto  end  = std::ranges::end(map);
        while (iter != end) {
            auto& pair               = *iter;
            const auto& [key, value] = pair;
            out.append("[{} => {}]", repr_value(key), repr_value(value));
            ++iter;
            if (iter != end) {
                out.append(", ");
            }
        }
        out.append("}");
    }
}

template <typename R>
requires requires {
    requires std::ranges::forward_range<R>;
    requires !alike<std::string, R> && !alike<std::string_view, R>;
    requires !detect_map<R>;
    requires !detect_path<R>;
    requires reprable<std::ranges::range_value_t<R>>;
}
constexpr void repr_fallback(auto out, tag<R>, auto* value) noexcept {
    if constexpr (not out.just_value) {
        auto value_type_str = repr_type<std::ranges::range_value_t<R>>();
        if constexpr (detect_vector<R>) {
            out.append("vector<{}>", value_type_str);
        } else if constexpr (std::is_array_v<R>) {
            out.append("{}[]", value_type_str);
        } else if constexpr (detect_std_array<R>) {
            out.append("array<{}>", value_type_str);
        } else {
            out.append("range<{}>", value_type_str);
        }
    }
    if constexpr (not out.just_type) {
        out.append("{");
        auto end = std::ranges::cend(*value);
        for (auto it = std::ranges::cbegin(*value); it != end; ++it) {
            if constexpr (not out.just_value) {
                out.append("{}", repr_value(*it));
            } else {
                out.append("{}", repr(*it));
            }
            if (std::next(it) != end) {
                out.append(", ");
            }
        }
        out.append("}");
    }
}

constexpr void tuple_out_next(auto, auto, const auto&) noexcept {}

template <std::size_t First, std::size_t... Idx, typename Tuple>
constexpr void tuple_out_next(auto out, std::index_sequence<First, Idx...>, const Tuple& tup) {
    if constexpr (out.just_type) {
        out.append("{}", repr_type<std::tuple_element_t<First, Tuple>>());
    } else if constexpr (out.just_value) {
        out.append("{}", repr_value(std::get<First>(tup)));
    } else {
        out.append("{}", repr(std::get<First>(tup)));
    }
    if (sizeof...(Idx) != 0) {
        out.append(", ");
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
constexpr void repr_fallback(auto out, tag<Tuple>, auto* value) noexcept {
    if constexpr (out.just_type) {
        out.append("tuple<");
    } else if constexpr (out.just_value) {
        out.append("{");
    } else {
        out.append("tuple{");
    }
    tuple_out_next(out, std::make_index_sequence<std::tuple_size_v<Tuple>>{}, *value);
    if constexpr (out.just_type) {
        out.append(">");
    } else {
        out.append("}");
    }
}

}  // namespace repr_detail

}  // namespace neo
