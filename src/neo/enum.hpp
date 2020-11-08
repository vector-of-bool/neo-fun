#pragma once

#include "./tag.hpp"

#include <type_traits>

/**
 * @brief Declare the given enum type to be a set of bitwise-flags.
 *
 * Declares the set of bitwise operators necessary to use `Type` as a set of
 * bitwise flags.
 */
#define NEO_DECL_ENUM_BITOPS(Type)                                                                 \
    constexpr Type operator|(Type left, Type right) noexcept {                                     \
        using Int = std::underlying_type_t<Type>;                                                  \
        return Type(Int(left) | Int(right));                                                       \
    }                                                                                              \
    constexpr Type& operator|=(Type& left, Type right) noexcept { return left = left | right; }    \
    constexpr Type  operator&(Type left, Type right) noexcept {                                    \
        using Int = std::underlying_type_t<Type>;                                                 \
        return Type(Int(left) & Int(right));                                                      \
    }                                                                                              \
    constexpr Type& operator&=(Type& left, Type right) noexcept { return left = left & right; }    \
    constexpr Type  operator^(Type left, Type right) noexcept {                                    \
        using Int = std::underlying_type_t<Type>;                                                 \
        return Type(Int(left) ^ Int(right));                                                      \
    }                                                                                              \
    constexpr Type& operator^=(Type& left, Type right) noexcept { return left = left ^ right; }    \
    constexpr Type  operator~(Type op) noexcept {                                                  \
        using Int = std::underlying_type_t<Type>;                                                 \
        return Type(~Int(op));                                                                    \
    }                                                                                              \
    constexpr bool _neo_is_bitset_(neo::tag<Type>, int) noexcept { return true; }                  \
    static_assert(std::is_enum_v<                                                                  \
                      Type> && !std::is_convertible_v<Type, std::underlying_type_t<Type>>,         \
                  "NEO_DECL_ENUM_BITOPS may only be used with `enum class` types")

namespace neo {

/**
 * @brief Check if the flag(s) in the right-hand side is/are set in the set of bits on the left-hand
 * side.
 *
 * Equivalent to: `(flagset & flag) == flag`
 *
 * @param flagset A set of bits (an enum or integral)
 * @param flag The bit or set of bits to test.
 * @return true If every bit in `flag` is also set in `flagset`
 * @return false Otherwise
 */
template <typename E>
[[nodiscard]] constexpr bool test_flags(E flagset, E flag) noexcept {
    return (flagset & flag) == flag;
}

/**
 * @brief Create a flag-testing callable object.
 *
 * The returned object will be a callable that returns 'true' if passed a
 * flag that is set in 'flagset'
 *
 * @param flagset The set of flags to check against
 */
template <typename E>
[[nodiscard]] constexpr auto test_flags(E flagset) noexcept {
    return [flagset](E check) noexcept { return test_flags(flagset, check); };
}

/**
 * @brief Create a flag-testing callable object from a reference
 *
 * @tparam E
 * @param flagset
 * @return constexpr auto
 */
template <typename E>
[[nodiscard]] constexpr auto test_flags(E* flagset) noexcept {
    return [flagset](E check) noexcept { return test_flags(*flagset, check); };
}

/**
 * @brief Clear the flag(s) in the right-hand side from the left-hand side
 *
 * Equivalent to: `flagset & ~flag`
 *
 * @param flagset A set of bits (an enum or integral)
 * @param flag The bit or set of bits to un-set in 'flagset'
 * @return E 'flagset' with the bit(s) designated in 'flag' set to zero
 */
template <typename E>
[[nodiscard]] constexpr E unset_flags(E flagset, E flag) noexcept {
    return flagset & ~flag;
}

/**
 * @brief Toggle the bit(s) in 'flagset' designated by 'flag'
 *
 * For every set-bit in 'flag', toggle the value of that bit in 'flagset', where
 * in this case 'toggle' means that a one-bit becomes a zero-bit, and a zero-bit
 * becomes a one-bit.
 *
 * @param flagset The set of flags to modify
 * @param flag The bit(s) to modify in 'flagset'
 * @return E 'flagset' with each bit designated in 'flag' swapped from its previous value
 */
template <typename E>
[[nodiscard]] constexpr E toggle_flags(E flagset, E flag) noexcept {
    // Get the current value of bits-of-interest
    const E cur_bits = flagset & flag;
    // Swap those bits
    const E swapped = ~cur_bits;
    // Mask out the one-bits that we don't want
    const E masked = swapped & flag;
    // Get all of the bits that are not designated by 'flag'
    const E keep_bits = unset_flags(flagset, flag);
    // Merge
    return keep_bits | masked;
}

/**
 * @brief Toggle the bit(s) in 'flagset' designated by 'flag' to the value of the given bool 'val'.
 *
 * If 'val' is true, then this is equivalent to `flagset | flag`, otherise it is
 * `unset_flags(flagset, flag)`.
 *
 * @param flagset The set of flags to modify
 * @param flag The bit(s) to modify in 'flagset'
 * @param val Whether the designated bits should be set high (one) or low (zero)
 * @return E 'flagset' with the designated bits set as appropriate
 */
template <typename E>
[[nodiscard]] constexpr E toggle_flags(E flagset, E flag, bool val) noexcept {
    if (val) {
        return flagset | flag;
    } else {
        return unset_flags(flagset, flag);
    }
}

namespace detail {

template <typename T>
constexpr bool _neo_is_bitset_(tag<T>, ...) noexcept {
    return false;
}

template <typename T>
constexpr bool check_is_bitset() noexcept {
    return _neo_is_bitset_(tag_v<T>, 0);
}

}  // namespace detail

/**
 * @brief Check whether the given type has been declared to be a set of bitflags
 * with `NEO_DECL_ENUM_BITOPS`
 */
template <typename E>
constexpr bool is_bitset_enum_v = detail::check_is_bitset<E>();

/**
 * @brief Any type that is a (possibly qualified) enum type annotated by 'NEO_DECL_ENUM_BITOPS'
 */
template <typename T>
concept bitset_enum = is_bitset_enum_v<std::remove_cvref_t<T>>;

}  // namespace neo
