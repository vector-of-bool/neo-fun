#pragma once

#include "./fwd.hpp"
#include "./tuple.hpp"
#include "./type_traits.hpp"

#include <compare>

namespace neo {

/**
 * @brief "Take" the given object. 'obj' is replaced with a default-init value.
 *
 * Equivalent to 'std::exchange(obj, T())'.
 */
template <typename T>
constexpr T take(T& obj) noexcept(noexcept(remove_cvref_t<T>())) {
    auto ret = NEO_MOVE(obj);
    obj      = remove_cvref_t<T>();
    return ret;
}

namespace oper {

constexpr inline struct equal_to_t {
    template <typename Left, typename Right>
    constexpr bool operator()(Left&& l, Right&& r) const noexcept {
        return l == r;
    }
} equal_to;

constexpr inline struct not_equal_to_t {
    template <typename Left, typename Right>
    constexpr bool operator()(Left&& l, Right&& r) const noexcept {
        return l != r;
    }
} not_equal_to;

constexpr inline struct logical_or_t {
    template <typename Left, typename Right>
    constexpr bool operator()(Left&& l, Right&& r) const noexcept {
        return l || r;
    }
} logical_or;

constexpr inline struct logical_and_t {
    template <typename Left, typename Right>
    constexpr bool operator()(Left&& l, Right&& r) const noexcept {
        return l && r;
    }
} logical_and;

/**
 * @brief Use with operator== and operator!= to do a natural language set membership check
 */
template <typename... Items>
class any_of {
    core::tuple<const Items&...> _items;

    template <auto Oper, std::size_t... I, typename Other>
    constexpr bool _op_1(std::index_sequence<I...>, logical_or_t, Other&& o) const noexcept {
        return (Oper(o, _items.template get<I>()) || ...);
    }

    template <auto Oper, std::size_t... I, typename Other>
    constexpr bool _op_1(std::index_sequence<I...>, logical_and_t, Other&& o) const noexcept {
        return (Oper(o, _items.template get<I>()) && ...);
    }

    template <auto Oper, auto Joiner, typename Other>
    constexpr bool _op(Other&& other) const noexcept {
        return _op_1<Oper>(std::index_sequence_for<Items...>(), Joiner, other);
    }

public:
    constexpr explicit any_of(const Items&... i) noexcept
        : _items(neo::tuple_construct_tag{}, i...) {}

    template <typename Left>
    constexpr friend bool operator==(Left&& lhs, const any_of& rhs) noexcept {
        return rhs._op<equal_to, logical_or>(lhs);
    }

    template <typename Left>
    constexpr friend bool operator!=(Left&& lhs, const any_of& rhs) noexcept {
        return rhs._op<not_equal_to, logical_and>(lhs);
    }
};

template <typename... Items>
explicit any_of(const Items&...) -> any_of<Items...>;

/**
 * @brief Use with operator== and operator!= to do a natural language set membership check
 */
template <typename... Items>
class none_of {
    core::tuple<const Items&...> _items;

    template <auto Oper, std::size_t... I, typename Other>
    constexpr bool _op_1(std::index_sequence<I...>, Other&& o) const noexcept {
        return (Oper(o, _items.template get<I>()) || ...);
    }

    template <auto Oper, typename Other>
    constexpr bool _op(Other&& other) const noexcept {
        return _op_1<Oper>(std::index_sequence_for<Items...>(), other);
    }

public:
    constexpr explicit none_of(const Items&... i) noexcept
        : _items(tuple_construct_tag{}, i...) {}

    template <typename Left>
    constexpr friend bool operator==(Left&& lhs, const none_of& rhs) noexcept {
        return !rhs._op<equal_to>(lhs);
    }
};

template <typename... Items>
explicit none_of(const Items&...) -> none_of<Items...>;

}  // namespace oper

/**
 * @brief Determine if 'arg' is "between" 'low' and 'high', inclusive.
 *
 * Use 'properly_between' for exclusive range check.
 *
 * @param arg The value to check to be between the range
 * @param low The lower value of the range
 * @param high The upper value of the range
 */
template <typename Arg, typename Low, typename High>
constexpr bool between(Arg&& arg, Low&& low, High&& high) noexcept
    requires requires {
        arg <= high;
        arg >= low;
    }
{
    return (arg <= high) && (arg >= low);
}

/**
 * @brief Determine if 'arg' is "between" 'low' and 'high', exclusive.
 *
 * @param arg The value to check to be between the range
 * @param low The lower value of the range
 * @param high The upper value of the range
 */
template <typename Arg, typename Low, typename High>
constexpr bool properly_between(Arg&& arg, Low&& low, High&& high) noexcept
    requires requires {
        arg < high;
        arg > low;
    }
{
    return (arg < high) && (arg > low);
}

/**
 * @brief Add this as a member (with NEO_NO_UNIQUE_ADDRESS) to force your type to become move-only
 */
struct move_only_cookie {
    constexpr move_only_cookie()                              = default;
    constexpr move_only_cookie(move_only_cookie&&)            = default;
    constexpr move_only_cookie& operator=(move_only_cookie&&) = default;

    constexpr bool operator==(const move_only_cookie&) const noexcept  = default;
    constexpr auto operator<=>(const move_only_cookie&) const noexcept = default;
};

}  // namespace neo
