#pragma once

#include <neo/addressof.hpp>
#include <neo/concepts.hpp>
#include <neo/declval.hpp>
#include <neo/like.hpp>
#include <neo/meta.hpp>

#include <array>
#include <cstddef>
#include <tuple>
#include <variant>

namespace neo::_get {

// Match an opt-ref like type of unspecified referred-to type
template <typename Ref>
concept opt_ref_like = requires(const Ref const_ref, Ref mut_ref) {
    { const_ref ? 0 : 0 } noexcept;
    { *const_ref };
    { *const_ref } -> neo::weak_same_as<decltype(*mut_ref)>;
    requires semiregular<Ref>;
};

// Match an opt-ref-like whose dereference operator returns the given type
template <typename OptRef, typename Ref>
concept opt_ref_to = opt_ref_like<OptRef> and requires(const OptRef ref) {
    { *ref } -> neo::weak_same_as<Ref>;
};

// Some standard library types are not sfinae-friendly, but we want to make a SFINAE-friendly get<>
// Use some variable templates to force the issue. In the base case, the mask is always `true`
// since we can rely on other methods of exclusing non-stdlib types.
template <typename Var, typename Alt>
constexpr bool sfinae_mask = true;

template <typename T, std::size_t N>
constexpr bool sfinae_mask_n = true;

template <typename... Ts, typename Alt>
constexpr bool sfinae_mask<std::variant<Ts...>, Alt> = (weak_same_as<Ts, Alt> || ...);

template <typename... Ts, typename Alt>
constexpr bool sfinae_mask<std::tuple<Ts...>, Alt> = (weak_same_as<Ts, Alt> || ...);

template <typename... Ts, std::size_t N>
constexpr bool sfinae_mask_n<std::variant<Ts...>, N> = sizeof...(Ts) > N;

template <typename... Ts, std::size_t N>
constexpr bool sfinae_mask_n<std::tuple<Ts...>, N> = sizeof...(Ts) > N;

template <typename T, std::size_t Len, std::size_t N>
constexpr bool sfinae_mask_n<std::array<T, Len>, N> = N < Len;

template <typename Alt>
void try_get() = delete;

template <std::size_t N>
void try_get() = delete;

template <std::size_t N>
void get() = delete;

template <typename Alt>
void get() = delete;

namespace declared {
// Does `T` have a `nth_type<>` member alias template?
template <typename T, std::size_t N>
concept has_member_template_nth_type = requires { typename T::template nth_type<N>; };
// Does `T` have a valid std::tuple_element_t<N, T>?
template <typename T, std::size_t N>
concept has_std_tuple_element
    = sfinae_mask_n<T, N> and requires { typename std::tuple_element_t<N, T>; };
// Does `T` have a valid std::variant_alternative_t<N, T>?
template <typename T, std::size_t N>
concept has_std_variant_alternative
    = sfinae_mask_n<T, N> and requires { typename std::variant_alternative_t<N, T>; };

template <bool HasTplElem>
struct try_var_alt {};

template <bool HasTplElem>
struct try_tpl_elem {
    template <typename T, std::size_t N>
    using f = try_var_alt<has_std_variant_alternative<T, N>>::template f<T, N>;
};

template <bool HasNthType>
struct try_nth_type {
    template <typename T, std::size_t N>
    using f = try_tpl_elem<has_std_tuple_element<T, N>>::template f<T, N>;
};

template <>
struct try_nth_type<true> {
    template <typename T, std::size_t N>
    using f = typename T::template nth_type<N>;
};

template <>
struct try_tpl_elem<true> {
    template <typename T, std::size_t N>
    using f = std::tuple_element_t<N, T>;
};

template <>
struct try_var_alt<true> {
    template <typename T, std::size_t N>
    using f = std::variant_alternative_t<N, T>;
};

template <typename T, std::size_t N>
using nth = try_nth_type<has_member_template_nth_type<T, N>>::template f<T, N>;
}  // namespace declared

template <typename T, std::size_t N>
using nth_type_t = declared::nth<remove_cvref_t<T>, N>;

template <typename T, std::size_t N>
concept has_nth_type = requires { typename nth_type_t<T, N>; };

template <typename T, std::size_t N>
using forward_nth_type_t = forward_like_tuple_t<T, nth_type_t<T, N>>;

template <typename T, std::size_t N>
concept has_member_try_get_nth = requires(T&& var) {
    { var.template try_get<N>() } noexcept -> opt_ref_to<forward_nth_type_t<T&, N>>;
};

template <typename T, std::size_t N>
concept has_free_try_get_nth = requires(T&& var) {
    { try_get<N>(var) } noexcept -> opt_ref_to<forward_nth_type_t<T&, N>>;
};

template <typename T, std::size_t N>
concept has_free_get_if_nth = requires(T&& var) {
    requires sfinae_mask_n<remove_cvref_t<T>, N>;
    { get_if<N>(NEO_ADDRESSOF(var)) } noexcept -> opt_ref_to<forward_nth_type_t<T&, N>>;
};

template <typename T, std::size_t N>
concept has_any_try_get_nth_backend
    = has_member_try_get_nth<T, N> or has_free_try_get_nth<T, N> or has_free_get_if_nth<T, N>;

template <typename T, std::size_t N>
concept has_member_get_nth = requires(T&& var) {
    { NEO_FWD(var).template get<N>() } noexcept -> weak_same_as<forward_nth_type_t<T, N>>;
};

template <typename T, std::size_t N>
concept has_free_get_nth = sfinae_mask_n<remove_cvref_t<T>, N> and requires(T&& var) {
    { get<N>(NEO_FWD(var)) } noexcept -> weak_same_as<forward_nth_type_t<T, N>>;
};

template <typename T, std::size_t N>
concept has_any_direct_get_nth_backend = not has_any_try_get_nth_backend<T, N>
    and (has_free_get_nth<T, N> or has_member_get_nth<T, N>);

constexpr inline std::size_t invalid_index = ~std::size_t(0);

template <typename L, typename Alt>
constexpr std::size_t lookup_v = invalid_index;

template <neo_ttparam L, typename... Ts, typename Alt>
    requires((weak_same_as<Ts, Alt> + ...) == 1)
constexpr std::size_t lookup_v<L<Ts...>, Alt> = meta::find_type_v<Alt, Ts...>;

template <typename T, typename Alt, std::size_t N = lookup_v<remove_cvref_t<T>, Alt>>
concept can_try_get_alt = N != invalid_index and has_any_try_get_nth_backend<T, N>;

template <typename T, typename Alt, std::size_t N = lookup_v<remove_cvref_t<T>, Alt>>
concept can_direct_get_alt
    = N != invalid_index and not can_try_get_alt<T, Alt> and has_any_direct_get_nth_backend<T, N>;

/*
d8b                        888
Y8P                        888
                           888
888 88888b.d88b.  88888b.  888
888 888 "888 "88b 888 "88b 888
888 888  888  888 888  888 888
888 888  888  888 888 d88P 888
888 888  888  888 88888P"  888
                  888
                  888
                  888
*/

template <std::size_t N>
struct try_nth {
    template <_get::has_any_try_get_nth_backend<N> T>
    NEO_ALWAYS_INLINE constexpr opt_ref_like auto operator()(T&& obj) const noexcept {
        if constexpr (has_member_try_get_nth<T, N>) {
            return obj.template try_get<N>();
        } else if constexpr (has_free_try_get_nth<T, N>) {
            return try_get<N>(obj);
        } else {
            return get_if<N>(NEO_ADDRESSOF(obj));
        }
    }
};

template <std::size_t N>
struct nth {
    template <has_any_try_get_nth_backend<N> T>
    NEO_ALWAYS_INLINE constexpr forward_nth_type_t<T, N> operator()(T&& obj) const {
        auto oref = try_nth<N>{}(obj);
        if (not oref) {
            throw std::bad_variant_access();
        } else {
            return static_cast<forward_nth_type_t<T, N>>(*oref);
        }
    }

    template <has_any_direct_get_nth_backend<N> T>
    NEO_ALWAYS_INLINE constexpr forward_nth_type_t<T, N> operator()(T&& obj) const {
        if constexpr (has_member_get_nth<T, N>) {
            return NEO_FWD(obj).template get<N>();
        } else {
            return get<N>(NEO_FWD(obj));
        }
    }
};

template <typename Alt>
struct try_as {
    template <can_try_get_alt<Alt> T>
    NEO_ALWAYS_INLINE constexpr opt_ref_like auto operator()(T&& obj) const noexcept {
        constexpr auto N = lookup_v<remove_cvref_t<T>, Alt>;
        return try_nth<N>{}(NEO_FWD(obj));
    }
};

template <typename Alt>
struct as {
    template <can_try_get_alt<Alt> T>
    NEO_ALWAYS_INLINE constexpr forward_like_tuple_t<T, Alt> operator()(T&& obj) const {
        auto opt = try_as<Alt>{}(obj);
        if (not opt) {
            throw std::bad_variant_access();
        } else {
            return static_cast<forward_like_tuple_t<T, Alt>>(*opt);
        }
    }

    template <can_direct_get_alt<Alt> T>
    NEO_ALWAYS_INLINE constexpr forward_like_tuple_t<T, Alt> operator()(T&& obj) const {
        constexpr auto N = lookup_v<remove_cvref_t<T>, Alt>;
        return nth<N>{}(NEO_FWD(obj));
    }
};

}  // namespace neo::_get
