#pragma once

#include "neo/type_traits.hpp"
#include <neo/concepts.hpp>

#include <optional>

namespace neo {

template <typename T>
struct tombstone_traits;

template <typename T>
class optional;

using std::nullopt;
using std::nullopt_t;

/**
 * @brief Detect whether T is a specialization of neo::optional
 */
template <typename T>
constexpr bool is_optional_v = false;

template <typename T>
constexpr bool is_optional_v<optional<T>> = true;

}  // namespace neo

namespace neo::opt_detail {

template <typename Traits, typename T>
concept valid_tombstone_traits = requires {
    typename Traits::state_type;
    requires object_type<typename Traits::state_type>;
} and requires(typename Traits::state_type& mstate, typename Traits::state_type const& cstate) {
    { Traits::has_value(cstate) } noexcept -> simple_boolean;
    { Traits::get(cstate) } noexcept -> convertible_to<add_const_reference_t<T>>;
    { Traits::get(mstate) } noexcept -> convertible_to<add_lvalue_reference_t<T>>;
    { Traits::destroy(mstate) } noexcept;
    { Traits::trivial_copy } -> simple_boolean;
    { Traits::trivial_copy_assign } -> simple_boolean;
    { Traits::trivial_move } -> simple_boolean;
    { Traits::trivial_move_assign } -> simple_boolean;
};

template <class_type T>
struct inherit_from : T {};

[[noreturn]] void throw_bad_optional();
[[noreturn]] void terminate_bad_optional();

/**
 * This class defines the comparison operators for neo::optional. They are injected
 * via ADL from inheritance, to prevent left/right ambiguity during overload selection.
 */
class adl_operators {
    template <typename T, equality_comparable_with<T> U>
    friend constexpr bool operator==(const optional<T>& self, const optional<U>& other) noexcept {
        if (self.has_value() != other.has_value()) {
            return false;
        }
        return not self.has_value() or *self == *other;
    }

    template <typename T>
    friend constexpr bool operator==(const optional<T>& self, nullopt_t) noexcept {
        return not self.has_value();
    }

    template <typename T, typename U>
        requires(not is_optional_v<U>) and equality_comparable_with<T, U>
    friend constexpr bool operator==(const optional<T>& self, const U& other) noexcept {
        return self.has_value() and *self == other;
    }

    /**
     * @brief Compare an optional against another optional of a value_type
     * that is comparable to the value_type of this optional.
     *
     * A null optional is always less than an engaged one, two null optionals
     * are equivalent, and two engaged optionals are compared by the value
     * that they contain.
     */
    template <typename T, three_way_comparable_with<T> U>
    friend constexpr std::compare_three_way_result_t<T, U>
    operator<=>(const optional<T>& self, const optional<U> other) noexcept {
        if (self.has_value() and other.has_value()) {
            return std::compare_three_way{}(*self, *other);
        } else {
            return self.has_value() <=> other.has_value();
        }
    }

    /**
     * @brief Compare the optional with a null-constant.
     *
     * The null constant is always less-than any engaged optional, and equivalent
     * to a null optional.
     */
    template <typename T>
    friend constexpr std::strong_ordering  //
    operator<=>(const optional<T>& self, nullopt_t) noexcept {
        if (self.has_value()) {
            return std::strong_ordering::greater;
        } else {
            return std::strong_ordering::equal;
        }
    }

    /**
     * @brief Compare the optional with a value of another type which is comparable with the
     * optional's value_type
     *
     * A null optional is always less-than any other value
     */
    template <typename T, typename U>
        requires(not is_optional_v<U>) and three_way_comparable_with<T, U>
    friend constexpr std::compare_three_way_result_t<T, U>  //
    operator<=>(const optional<T>& self, const U& value) noexcept {
        if (self.has_value()) {
            return std::compare_three_way{}(*self, value);
        } else {
            return std::strong_ordering::less;
        }
    }
};

}  // namespace neo::opt_detail