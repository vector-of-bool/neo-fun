#pragma once

#include "./concepts.hpp"
#include "./ufmt.hpp"

#include <charconv>
#include <cinttypes>
#include <concepts>
#include <ostream>
#include <ranges>
#include <string>
#include <tuple>

/**
 * @file repr.hpp - Define and render rich representations of objects and values.
 *
 * Refer to the docs/repr.md page for information on using and extending this API
 */

namespace neo {

// First declare the basic interface of a repr()-able type
namespace repr_detail {

// Tag dispatch helper for certain types
template <typename>
struct tag {};

// Type that is passed to the repr() extension points
class repr_writer;

// decl a name that can be using'd below
template <typename T>
struct repr_builtin;

template <typename T>
concept repr_builtin_guard = requires {
    repr_builtin<T>{};
};

// clang-format off
template <typename T>
concept has_adl_do_repr_exact = requires(repr_writer& out, const T* t) {
    do_repr(out, t);  //! READ THIS IF YOU SEE AN ERROR HERE:
    /**
     * This concept detects if there is an ADL-visible overload of `do_repr()`
     * for the given type `T`. `do_repr()` should be a non-member function that
     * takes a deduced parameter `out` and a pointer-to-const of `T`. Refer to
     * the docs/repr.md file on how to write a do_repr() function.
     */
};

/// Check if we have a non-member ADL-visible do_repr()
template <typename T>
concept has_adl_do_repr =
    // Guard against calling do_repr() for types which we have built-ins
    (!repr_builtin_guard<T>)  // NOTE: Users should ignore this condition being false
    // The main requirement:
    and has_adl_do_repr_exact<T>;

/// Check if we have a fallback function for this one
template <typename T>
concept has_repr_builtin =   //
    (!!repr_builtin_guard<T>)  // NOTE: Users should ignore this condition being false
    and requires(repr_writer& out, const T* obj) {
        repr_builtin<T>::write(out, obj);  //! READ THIS IF YOU SEE AN ERROR HERE:
        /**
         * This concept implements a check for certain pre-defined serialization
         * methods. If you see an error here, it means that you are trying to
         * repr() a type for which we can *almost* provide a default
         * representation. The failure is that the type is a wrapper around some
         * other type that *is not* representable. For example, a std::vector<T>
         * is representable if-and-only-if `T` is also representable. Check
         * further in your compiler's diagnostic for the exact `T` that fails
         * with the `do_repr()` check above in 'has_adl_do_repr()'
         */
    };
// clang-format on

/// Check that we can write a repr of T via ADL, member, or a fallback
template <typename T>
concept reprable_impl = has_repr_builtin<std::remove_cvref_t<T>> || has_adl_do_repr<T>;

class repr_writer {
    std::string* _out;

public:
    constexpr static bool just_type  = false;
    constexpr static bool just_value = false;

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

    constexpr void type(std::string_view fmt [[maybe_unused]],
                        const auto&... args [[maybe_unused]]) noexcept {
        if constexpr (not just_value) {
            append(fmt, args...);
        }
    }

    constexpr void value(std::string_view fmt [[maybe_unused]],
                         const auto&... args [[maybe_unused]]) noexcept {
        if constexpr (not just_type) {
            if constexpr (not just_value) {
                append("{");
            }
            append(fmt, args...);
            if constexpr (not just_value) {
                append("}");
            }
        }
    }

    constexpr void bracket_value(std::string_view fmt [[maybe_unused]],
                                 const auto&... args [[maybe_unused]]) noexcept {
        append("{");
        append(fmt, args...);
        append("}");
    }

    constexpr auto repr_sub(const auto& obj) noexcept {
        if constexpr (just_value) {
            return repr_value(obj);
        } else {
            return repr(obj);
        }
    }
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
        auto str = self.string();
        out.write(str.data(), str.size());
        return out;
    }

    /// Serialize the repr() of the type to a ufmt() string
    constexpr friend void ufmt_append(std::string& out, type_repr) noexcept {
        repr_detail::repr_writer_impl<false, true> wr{out};
        if constexpr (repr_detail::has_adl_do_repr_exact<T>) {
            do_repr(wr, (T*)(nullptr));
        } else {
            using repr_detail::repr_builtin;
            repr_builtin<T>::write(wr, (const T*)(0));
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
        auto str = self.string();
        out.write(str.data(), str.size());
        return out;
    }

    /// Append the repr() of the value to the given ufmt() string
    constexpr friend void ufmt_append(std::string& out, value_repr self) noexcept {
        repr_detail::repr_writer_impl<true, WantType> wr{out};
        if constexpr (repr_detail::has_adl_do_repr_exact<T>) {
            do_repr(wr, std::addressof(self.value));
        } else {
            using repr_detail::repr_builtin;
            repr_builtin<T>::write(wr, std::addressof(self.value));
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

template <reprable T>
constexpr auto repr_type(const T& t) noexcept {
    return repr_type<T>();
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
    struct repr_builtin<T> {
    constexpr static void write(auto out, auto*) noexcept {
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
};

/// Handle 'void'
template <>
struct repr_builtin<void> {
    constexpr static void write(auto out, auto*) noexcept {
        static_assert(out.just_type);
        out.append("void");
    }
};

/// repr() a pointer of any type. If the pointer is non-null and the pointed-to-type can be
/// repr()'d, includes the repr() of the pointed-to value
template <typename T>
struct repr_builtin<T*> {
    constexpr static void write(auto out, auto* value) noexcept {
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
            if constexpr (reprable<T>) {
                out.append("{}*", repr_type<T>());
            } else {
                out.append("unknown-pointer");
            }
        } else if constexpr (out.just_value) {
            if constexpr (reprable<T>) {
                if (*value != nullptr) {
                    // Wrap the value in brackets to represent the de-referencing
                    out.append("->{}", repr_value(**value));
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
};

template <>
struct repr_builtin<double> {
    constexpr static void write(auto out, const double* d) noexcept {
        if constexpr (out.just_type) {
            out.append("double");
        } else {
            out.append("{}", *d);
        }
    }
};

template <>
struct repr_builtin<float> {
    constexpr static void write(auto out, const float* f) noexcept {
        if constexpr (out.just_type) {
            out.append("float");
        } else if constexpr (out.just_value) {
            out.append("{}", *f);
        } else {
            out.append("{}f", *f);
        }
    }
};

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
    struct repr_builtin<Integral> {
    constexpr static void write(auto out, auto* value) noexcept {
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
        // Some seem to have size_t defined differently??
        DECL_REPR_TYPE_CASE(std::size_t, "size_t")
        else {
            static_assert(std::is_void_v<Integral>, "Unhandled built-in integral type");
        }
    }
};

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
struct repr_builtin<Path> {
    constexpr static void write(auto out, auto* value) noexcept {
        out.type("path");
        if (value) {
            out.value("{}", out.repr_value(value->string()));
        }
    }
};

template <detect_pair Pair>
struct repr_builtin<Pair> {
    using first  = Pair::first_type;
    using second = Pair::second_type;
    constexpr static void write(auto out, auto* value) noexcept
        requires(reprable<first>&& reprable<second>) {
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
};

template <detect_optional Optional>
struct repr_builtin<Optional> {
    using val_t = Optional::value_type;
    constexpr static void write(auto out, auto* value) noexcept requires reprable<val_t> {
        out.type("optional<{}>", repr_type<typename Optional::value_type>());
        if (value and *value) {
            out.value("->{}", out.repr_value(**value));
        } else {
            out.value("nullopt");
        }
    }
};

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
struct repr_builtin<std::basic_string_view<Char, Traits>> {
    constexpr static void write(auto out, auto* value) noexcept requires reprable<Char> {
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
};

template <typename Char, typename Traits, typename Alloc>
struct repr_builtin<std::basic_string<Char, Traits, Alloc>> {
    constexpr static void write(auto out, auto* value) noexcept requires reprable<Char> {
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
};

template <detect_map Map>
struct repr_builtin<Map> {
    using pair_type = typename Map::value_type;
    constexpr static void write(auto out, auto* value) noexcept requires reprable<pair_type> {
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
                auto& pair                = *iter;
                const auto& [key, mapped] = pair;
                out.append("[{} => {}]", repr_value(key), repr_value(mapped));
                ++iter;
                if (iter != end) {
                    out.append(", ");
                }
            }
            out.append("}");
        }
    }
};

template <typename R>
requires requires {
    requires std::ranges::forward_range<R>;
    requires !alike<std::string, R> && !alike<std::string_view, R>;
    requires !detect_map<R>;
    requires !detect_path<R>;
}
struct repr_builtin<R> {
    constexpr static void write(auto out, auto* value) noexcept requires requires {
        requires reprable<std::ranges::range_value_t<R>>;
    }
    {
        auto value_type_str = repr_type<std::ranges::range_value_t<R>>();
        if constexpr (detect_vector<R>) {
            out.type("vector<{}>", value_type_str);
        } else if constexpr (std::is_array_v<R>) {
            out.type("{}[]", value_type_str);
        } else if constexpr (detect_std_array<R>) {
            out.type("array<{}>", value_type_str);
        } else {
            out.type("range-of<{}>", value_type_str);
        }
        if (value) {
            out.append("{");
            auto end = std::ranges::cend(*value);
            for (auto it = std::ranges::cbegin(*value); it != end; ++it) {
                out.append("{}", repr_value(*it));
                if (std::next(it) != end) {
                    out.append(", ");
                }
            }
            out.append("}");
        }
    }
};

constexpr void tuple_out_next(auto, auto, const auto*) noexcept {}

template <std::size_t First, std::size_t... Idx, typename Tuple>
constexpr void
tuple_out_next(auto out, std::index_sequence<First, Idx...>, const Tuple* tup [[maybe_unused]]) {
    if constexpr (out.just_type) {
        out.append("{}", repr_type<std::tuple_element_t<First, Tuple>>());
    } else {
        out.append("{}", out.repr_sub(std::get<First>(*tup)));
    }
    if constexpr (sizeof...(Idx) != 0) {
        out.append(", ");
        tuple_out_next(out, std::index_sequence<Idx...>{}, tup);
    }
}

template <typename Tup, std::size_t... Seq>
void check_tuple_can_repr(const Tup&, std::index_sequence<Seq...>) requires(
    (reprable<std::tuple_element_t<Seq, Tup>>)&&...) {}

template <typename Tuple>
requires requires {
    requires detect_tuple<Tuple>;
    requires !detect_pair<Tuple>;
}
struct repr_builtin<Tuple> {
    constexpr static void write(auto out, auto* value) noexcept requires requires {
        check_tuple_can_repr(*value, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }
    {
        if constexpr (out.just_type) {
            out.append("tuple<");
        } else if constexpr (out.just_value) {
            out.append("{");
        } else {
            out.append("tuple{");
        }
        tuple_out_next(out, std::make_index_sequence<std::tuple_size_v<Tuple>>{}, value);
        if constexpr (out.just_type) {
            out.append(">");
        } else {
            out.append("}");
        }
    }
};

}  // namespace repr_detail

namespace repr_ostream_operator {

/**
 * @brief Provide an ostream<< insertion operator for any reprable object.
 *
 * This should be pulled into a scope via: 'using neo::repr_ostream_operator::operator<<'
 *
 * NOTE: When you 'using' this 'operator<<', this operator will become *more constrained*
 * than every other 'operator<<' that is in scope. This will cause the compiler to prefer
 * this 'operator<<' for every single object that is reprable. This will change the
 * result of 'operator<<' for almost every type that is already ostream-insertible,
 * including integers, floats, and strings.
 */
std::ostream& operator<<(std::ostream& out, reprable auto const& arg) noexcept {
    out << neo::repr(arg);
    return out;
}

}  // namespace repr_ostream_operator

}  // namespace neo
