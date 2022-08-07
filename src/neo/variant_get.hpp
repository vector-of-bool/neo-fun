#pragma once

#include "./addressof.hpp"
#include "./like.hpp"
#include "./overload.hpp"
#include "./tl.hpp"

#include <concepts>

namespace neo {

namespace var_detail {

[[noreturn]] void throw_bad_variant_access();

template <typename>
void try_get(...) = delete;

template <typename Result, typename Alt>
concept try_get_result = requires(Result res, void (*fn)(Alt& ref)) {
    { res ? 0 : 0 }
    noexcept;
    fn(*res);
};

template <typename Variant, typename Alt>
concept has_adl_try_get = requires(Variant&& var) {
    { try_get<Alt>(var) } -> try_get_result<forward_like_t<Variant, Alt>>;
};

template <typename Variant, typename Alt>
concept has_member_try_get = requires(Variant&& var) {
    { var.template try_get<Alt>() } -> try_get_result<forward_like_t<Variant, Alt>>;
};

template <typename Variant, typename Alt>
concept has_adl_get_if = requires(Variant&& var) {
    { get_if<Alt>(neo::addressof(var)) } -> try_get_result<forward_like_t<Variant, Alt>>;
};

template <typename Alt>
constexpr inline auto try_get_impl = neo::ordered_overload{
    [](has_member_try_get<Alt> auto&& var) noexcept { return var.template try_get<Alt>(); },
    [](has_adl_try_get<Alt> auto&& var) noexcept { return try_get<Alt>(var); },
    [](has_adl_get_if<Alt> auto&& var) noexcept { return get_if<Alt>(neo::addressof(var)); },
};

template <typename Variant, typename Alt>
concept has_try_get =                    //
    has_adl_try_get<Variant, Alt>        //
    or has_member_try_get<Variant, Alt>  //
    or has_adl_get_if<Variant, Alt>;

template <typename Var, typename Alt>
constexpr bool std_variant_mask = true;

template <typename T>
struct inherit_from : T {};

template <template <class...> class VariantTmpl, typename... Types, typename Alt>
requires std::same_as<typename inherit_from<VariantTmpl<Types...>>::variant, VariantTmpl<Types...>>
constexpr bool std_variant_mask<VariantTmpl<Types...>, Alt> = (std::same_as<Types, Alt> || ...);

template <typename Variant, typename Alt>
concept supports_alternative =    //
    has_try_get<Variant, Alt> &&  //
    std_variant_mask<std::remove_cvref_t<Variant>, Alt>;

}  // namespace var_detail

/**
 * @brief Check that the given sum-type-like 'Variant' supports 'Alt' as one of its alternatives
 *
 * @tparam Variant A variant-like type that can hold one of multiple types
 * @tparam Alt A possible type to assign-to/obtain-from Variant
 */
// clang-format off
template <typename Variant, typename Alt>
concept supports_alternative
      = var_detail::supports_alternative<Variant, Alt>
    and std::assignable_from<std::remove_cvref_t<Variant>&, Alt&&>
    and std::convertible_to<Alt&&, std::remove_cvref_t<Variant>>;
// clang-format on

template <typename Alternative>
struct try_get_fn {
    template <supports_alternative<Alternative> Var>
    constexpr auto operator()(Var&& var) const
        NEO_RETURNS(var_detail::try_get_impl<Alternative>(NEO_FWD(var)));
};

/**
 * @brief Try to obtain a pointer to a given alternative from a variant-like type
 *
 * @returns nullptr if the variant-like object is not currently holding the given alternative type,
 * otherwise returns a pointer to the held alternative.
 */
template <typename Alt>
constexpr inline auto try_get = try_get_fn<Alt>{};

template <typename Alternative>
struct holds_alternative_fn {
    constexpr bool operator()(supports_alternative<Alternative> auto const& var) const noexcept {
        return static_cast<bool>(try_get<Alternative>(var));
    }
};

/**
 * @brief Determine whether a variant-like type is currently holding the given alternative type.
 */
template <typename Alt>
constexpr inline auto holds_alternative = holds_alternative_fn<Alt>{};

template <typename Alternative>
struct get_fn {
    template <supports_alternative<Alternative> Variant>
    constexpr decltype(auto) operator()(Variant&& var) const {
        auto maybe = try_get<Alternative>(var);
        if (not static_cast<bool>(maybe)) {
            var_detail::throw_bad_variant_access();
        }
        if constexpr (std::is_reference_v<Alternative>) {
            return static_cast<Alternative>(*maybe);
        } else {
            if constexpr (std::is_rvalue_reference_v<Variant&&>) {
                return NEO_MOVE(*maybe);
            } else {
                return *maybe;
            }
        }
    }
};

/**
 * @brief Obtain the given alternative of a variant-like if that alternative is active.
 *
 * @tparam Alt The type to pull from a variant-like type
 * @throws std::bad_variant_access If attempting to pull a value from a variant that does not have
 * that type active.
 */
template <typename Alt>
constexpr inline auto get = get_fn<Alt>{};

}  // namespace neo
