#pragma once

#include <neo/concepts.hpp>
#include <neo/object_t.hpp>
#include <neo/swap.hpp>

#include <compare>  // operator<=>
#include <cstdint>  // std::uint16_t
#include <utility>  // emplacement tags

namespace neo::_variant_detail {

/// Yields `true` for types that are emplacement tags (in_place_type, in_place_index)
template <typename T>
constexpr bool is_emplacement_tag = false;

template <typename T>
constexpr bool is_emplacement_tag<std::in_place_type_t<T>> = true;

template <std::size_t I>
constexpr bool is_emplacement_tag<std::in_place_index_t<I>> = true;

/**
 * @brief An empty psuedo-integral type that is always equal to zero, used for
 * indexing in a variant of a single alternative.
 */
struct always_zero {
    constexpr always_zero() = default;

    consteval explicit always_zero(std::size_t n) {
        if (n != 0) {
            throw;
        }
    }

    template <neo::integral I>
    constexpr explicit operator I() const noexcept {
        return static_cast<I>(0);
    }

    constexpr bool operator==(neo::integral auto i) const noexcept { return i == 0; }
    constexpr std::strong_ordering operator<=>(neo::integral auto i) const noexcept {
        return 0 <=> i;
    }

    bool operator==(always_zero const&) const noexcept  = default;
    auto operator<=>(always_zero const&) const noexcept = default;
};

template <typename... Ts>
using pick_index_type
    = conditional_t<sizeof...(Ts) == 1,
                    always_zero,
                    conditional_t<sizeof...(Ts) < 256, std::uint8_t, std::uint16_t>>;

// If we have no destructors to execute, don't define one for ourself
template <typename... Ts>
concept enable_nontrivial_dtor = not(neo_is_trivially_destructible(object_t<Ts>) and ...);

// If any of our alts is not *trivially* copyable, but all alts ARE copyable, define
// a non-trivial copy constructor
template <typename... Ts>
concept enable_nontrivial_copy_construct = not(trivially_copy_constructible<object_t<Ts>> and ...)
    and (copy_constructible<object_t<Ts>> and ...);

// Define a copy-assignment if:
// - All alts are copyable
// - AND one of:
//      - Any alt is not trivially destructible (since we'll need to destroy it)
//      - Any alt is not trivially copy assignable (Since we may need to assign-thru)
//      - Any alt is not trivially copy constructible (Since we may need to copy)
template <typename... Ts>
concept enable_nontrivial_copy_assign =                       //
    (enable_nontrivial_dtor<Ts...>                            //
     or not(trivially_copy_assignable<object_t<Ts>> and ...)  //
     or enable_nontrivial_copy_construct<Ts...>)              //
    and(copyable<object_t<Ts>> and ...);

// If any of our alts is not *trivially* movable, but all alts ARE movable, define
// a non-trivial move constructor
template <typename... Ts>
concept enable_nontrivial_move_construct =  //
    not(trivially_move_constructible<object_t<Ts>> and ...)
    and (move_constructible<object_t<Ts>> and ...);

// Define a move-assignment if:
// - All alts are movable
// - AND one of:
//      - Any alt is not trivially destructible (since we'll need to destroy it)
//      - Any alt is not trivially move assignable (Since we may need to assign-thru)
//      - Any alt is not trivially move-constructible (Since we may need to move-construct it)
template <typename... Ts>
concept enable_nontrivial_move_assign =                       //
    (enable_nontrivial_dtor<Ts...>                            //
     or not(trivially_move_assignable<object_t<Ts>> and ...)  //
     or enable_nontrivial_move_construct<Ts...>)              //
    and(move_assignable<object_t<Ts>> and ...);

/**
 * @brief Helper used select the "best" alternative to a candidate conversion.
 *
 * @tparam Args A type list for the candidate conversion targets
 */
template <typename Args, typename Seq = std::make_index_sequence<meta::len_v<Args>>>
struct select_conversion;

/**
 * @brief Defines a single conversion function candidate
 *
 * @tparam std::size_t N the index of the alternative
 * @tparam Arg The argument type of the overload
 */
template <std::size_t N, typename To>
struct select_conversion_base {
    // This conversion is only viable if the conversion is defined AND non-narrowing.
    // Return the index of the conversion candidate (given via `N`)
    template <non_narrowing_convertible_to<To> From>
    static meta::val<N> select(To);
};

template <neo_ttparam L, typename... Args, std::size_t... Ns>
struct select_conversion<L<Args...>, std::index_sequence<Ns...>>
    // Zip the indices and the conversion candidates together in a variadic base class list:
    : select_conversion_base<Ns, Args>... {
    // Pull the base class functions to form the overload set
    using select_conversion_base<Ns, Args>::select...;
};

/**
 * @brief Find the zero-based index of the "best" conversion available from `From` to one of
 * `Ts...`. Yields a type with a nested `::value` specifying the zero-based index of
 * the selected conversion target in `Ts...`
 *
 * @tparam From The type which is being converted-from
 * @tparam Ts The candidate conversion targets
 *
 * Requires that EXACTLY one of the `Ts` be a valid target for a non-narrowing conversion from
 * `From`.
 *
 * If a conversion target is `void`, it is replaced with `neo::unit` for the overload argument.
 */
template <typename From, typename... Ts>
    requires(not is_emplacement_tag<remove_cvref_t<From>>)
using best_conversion_index_t
    = decltype(select_conversion<meta::list<nonvoid_t<Ts>...>>::template select<From>(
        NEO_DECLVAL(From)));

/**
 * @brief Defined later in variant.hpp
 *
 * @tparam bool Unused template parameter, only used to defer inspection of the variant base
 */
template <bool Unused>
struct variant_operators;

}  // namespace neo::_variant_detail
