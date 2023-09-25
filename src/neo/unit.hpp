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
    constexpr unit() noexcept = default;
    constexpr unit(decltype(nullptr)) noexcept {}
    template <typename X>
    constexpr explicit unit(X&&) noexcept {}

    constexpr bool operator==(unit) const noexcept { return true; }

    constexpr std::strong_ordering operator<=>(unit) const noexcept {
        return std::strong_ordering::equal;
    }
};

namespace unit_detail {

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

}  // namespace unit_detail

/**
 * @brief Ensures that `T` is not void. If given `void`, returns `neo::unit`
 */
template <typename T>
using make_nonvoid_t = unit_detail::make_nonvoid<void_type<T>>::template f<T>;

}  // namespace neo
