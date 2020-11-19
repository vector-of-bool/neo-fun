#pragma once

#include <numeric>
#include <tuple>
#include <utility>

namespace neo {

/**
 * @brief "Take" the given object. 'obj' is replaced with a default-init value.
 *
 * Equivalent to 'std::exchange(obj, T())'.
 */
template <typename T>
constexpr T take(T& obj) noexcept(noexcept(std::remove_cvref_t<T>())) {
    auto ret = std::move(obj);
    obj      = std::remove_cvref_t<T>();
    return ret;
}

namespace oper {

/**
 * @brief Use with operator== and operator!= to do a natural language set membership check
 */
template <typename... Items>
class any_of {
    std::tuple<const Items&...> _items;

    template <typename Oper, std::size_t... I, typename Other>
    constexpr bool _op_1(std::index_sequence<I...>, Other&& o) const noexcept {
        return (Oper()(o, std::get<I>(_items)) || ...);
    }

    template <typename Oper, typename Other>
    constexpr bool _op(Other&& other) const noexcept {
        return _op_1<Oper>(std::index_sequence_for<Items...>(), other);
    }

public:
    constexpr explicit any_of(const Items&... i) noexcept
        : _items(i...) {}

    template <typename Left>
    constexpr friend bool operator==(Left&& lhs, const any_of& rhs) noexcept {
        return rhs._op<std::equal_to<>>(lhs);
    }

    template <typename Left>
    constexpr friend bool operator!=(Left&& lhs, const any_of& rhs) noexcept {
        return rhs._op<std::not_equal_to<>>(lhs);
    }
};

template <typename... Items>
explicit any_of(const Items&...) -> any_of<Items...>;

/**
 * @brief Use with operator== and operator!= to do a natural language set membership check
 */
template <typename... Items>
class none_of {
    std::tuple<const Items&...> _items;

    template <typename Oper, std::size_t... I, typename Other>
    constexpr bool _op_1(std::index_sequence<I...>, Other&& o) const noexcept {
        return (Oper()(o, std::get<I>(_items)) || ...);
    }

    template <typename Oper, typename Other>
    constexpr bool _op(Other&& other) const noexcept {
        return _op_1<Oper>(std::index_sequence_for<Items...>(), other);
    }

public:
    constexpr explicit none_of(const Items&... i) noexcept
        : _items(i...) {}

    template <typename Left>
    constexpr friend bool operator==(Left&& lhs, const none_of& rhs) noexcept {
        return !rhs._op<std::equal_to<>>(lhs);
    }

    template <typename Left>
    constexpr friend bool operator!=(Left&& lhs, const none_of& rhs) noexcept {
        return !rhs._op<std::not_equal_to<>>(lhs);
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
constexpr bool between(Arg&& arg, Low&& low, High&& high) noexcept requires requires {
    arg <= high;
    arg >= low;
}
{ return (arg <= high) && (arg >= low); }

/**
 * @brief Determine if 'arg' is "between" 'low' and 'high', exclusive.
 *
 * @param arg The value to check to be between the range
 * @param low The lower value of the range
 * @param high The upper value of the range
 */
template <typename Arg, typename Low, typename High>
constexpr bool properly_between(Arg&& arg, Low&& low, High&& high) noexcept requires requires {
    arg < high;
    arg > low;
}
{ return (arg < high) && (arg > low); }

}  // namespace neo
