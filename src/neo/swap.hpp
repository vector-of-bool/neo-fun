#pragma once

#include "./fwd.hpp"
#include "./type_traits.hpp"

namespace neo {

namespace _swap {

// Detect an ADL-visible overload of swap()
template <typename T, typename U>
concept has_adl_swap
    = (class_or_enum_type<remove_cvref_t<T>> or class_or_enum_type<remove_cvref_t<U>>)  //
    and requires(T&& left, T&& right) { swap(NEO_FWD(left), NEO_FWD(right)); };

// Detect support for swapping via a temporary
template <typename T>
concept has_shuffle_swap = not has_adl_swap<T&, T&> and requires(T& t) {
    T(NEO_MOVE(t));
    t = NEO_MOVE(t);
};

// The swapping function
struct fn {
    /**
     * @brief Swap two objects via an ADL-visible custom swap() function
     */
    template <typename T, typename U>
        requires has_adl_swap<T, U>
    constexpr void operator()(T&& t, U&& u) const noexcept(noexcept(swap(NEO_FWD(t), NEO_FWD(u)))) {
        (void)swap(NEO_FWD(t), NEO_FWD(u));
    }

    /**
     * @brief Swap two objects by constructing a temporary and exchanging through it
     */
    template <typename T>
        requires has_shuffle_swap<T>
    constexpr void operator()(T& t, T& u) const
        noexcept(noexcept(t = NEO_MOVE(u)) and noexcept(T(NEO_MOVE(u)))) {
        T tmp = NEO_MOVE(t);
        t     = NEO_MOVE(u);
        u     = NEO_MOVE(tmp);
    }
};

}  // namespace _swap

inline namespace _cpo {
/**
 * @brief CPO implementation of std::swap()
 */
inline constexpr auto swap = _swap::fn{};
}  // namespace _cpo

}  // namespace neo
