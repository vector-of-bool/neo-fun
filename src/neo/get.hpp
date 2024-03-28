#pragma once

#include "./get.detail.hpp"

#include <neo/attrib.hpp>
#include <neo/concepts.hpp>
#include <neo/declval.hpp>

#include <cstddef>
#include <variant>

namespace neo {

/**
 * @brief Attempt to obtain the Nth alternative from a variant-like object
 *
 * @tparam N The index of the alternative to be requested
 */
template <std::size_t N>
constexpr inline auto try_get_nth = _get::try_nth<N>{};

/**
 * @brief Obtain the Nth alternative from a variant-like or tuple-like object
 *
 * @tparam N The index of the alternative to be requested.
 *
 * If accessing a variant and the variant does not currently contain the given
 * alternative, then bad_variant_access will be raised.
 */
template <std::size_t N>
constexpr inline auto get_nth = _get::nth<N>{};

/**
 * @brief Attempt to obtain a given alternative from a variant-like object.
 *
 * @tparam Alt The type to attempt to obtain from the variant.
 */
template <typename Alt>
constexpr inline auto try_get_as = _get::try_as<Alt>{};

/**
 * @brief Obtain a given alternative from a variant-like object.
 *
 * @tparam Alt The type to obtain from the variant.
 *
 * If the variant does not currently contain the given alternative, then bad_variant_access
 * will be raised.
 */
template <typename Alt>
constexpr inline auto get_as = _get::as<Alt>{};

/**
 * @brief The result of try_get_nth with the given type+index
 */
template <typename V, std::size_t N>
using try_get_nth_t = decltype(try_get_nth<N>(NEO_DECLVAL(V)));

/**
 * @brief The result of get_nth with the given type+index
 */
template <typename V, std::size_t N>
using get_nth_t = decltype(get_nth<N>(NEO_DECLVAL(V)));

/**
 * @brief The result type of try_get_as with the given type+index
 */
template <typename V, typename Alt>
using try_get_as_t = decltype(try_get_as<Alt>(NEO_DECLVAL(V)));

/**
 * @brief The result type of get_as with the given type+index
 */
template <typename V, typename Alt>
using get_as_t = decltype(get_as<Alt>(NEO_DECLVAL(V)));

template <typename Tpl, std::size_t Index>
concept can_get_nth = requires(Tpl&& tpl) { get_nth<Index>(NEO_FWD(tpl)); };

template <typename T, typename Alternative>
concept can_get_as = requires(T&& obj) { get_as<Alternative>(NEO_FWD(obj)); };

}  // namespace neo
