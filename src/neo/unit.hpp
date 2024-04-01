#pragma once

#include <neo/type_traits.hpp>

#include <compare>

namespace neo {

/**
 * @brief A type which is regular, strongly-ordered, empty, and stateless.
 *
 * All instances are strongly equal. Is constructible from a literal zero.
 * Explicit-convertible from anything.
 */
struct unit {
    // Default-constructor is a no-op
    constexpr unit() noexcept = default;
    // Construct from a literal zero, also a no-op
    constexpr unit(decltype(nullptr)) noexcept {}
    // Explicitly convert from anything (just discards the argument)
    template <typename X>
    constexpr explicit unit(X&&) noexcept {}

    // Default equality for a unit is always `true`
    constexpr bool operator==(unit const&) const noexcept = default;
    // Default ordering for a unit is always `equal`
    constexpr std::strong_ordering operator<=>(unit const&) const noexcept = default;
};

namespace _unit_detail {

template <bool IsVoid>
struct make_nonvoid {
    template <typename T>
    using f = T;
};

template <>
struct make_nonvoid<true> {
    template <typename>
    using f = unit;
};

}  // namespace _unit_detail

/**
 * @brief Ensures that `T` is not void. If given `void`, returns `neo::unit`
 */
template <typename T>
using nonvoid_t = _unit_detail::make_nonvoid<void_type<T>>::template f<T>;

}  // namespace neo
