#pragma once

#include "./concepts.hpp"
#include "./declval.hpp"
#include "./fwd.hpp"
#include "./type_traits.hpp"

#include <memory>
#include <type_traits>

namespace neo {

/**
 * @brief Rebind the given allocator to a new type
 *
 * Equivalent to:
 *
 *      std::allocator_traits<Alloc>::rebind_alloc<T>;
 */
template <typename Alloc, typename T>
using rebind_alloc_t = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;

/**
 * @brief Construct a new allocator by rebind-constructing the given allocator to a new type
 *
 * @tparam T The type to be allocated
 * @param alloc The allocator to rebind
 */
template <typename T, typename Allocator>
constexpr rebind_alloc_t<Allocator, T> rebind_alloc(Allocator alloc) noexcept {
    return rebind_alloc_t<Allocator, T>(alloc);
}

/**
 * @brief Create a decay-copy of the given object wrapped in a shared_ptr
 */
template <typename T, typename R = remove_cvref_t<T>>
constexpr auto copy_shared(T&& t) noexcept(nothrow_constructible_from<R, T>) {
    return std::shared_ptr<R>(new R(NEO_FWD(t)));
}

/**
 * @brief Create a decay-copy of the given object wrapped in a unique_ptr
 */
template <typename T, typename R = remove_cvref_t<T>>
constexpr auto copy_unique(T&& t) noexcept(nothrow_constructible_from<R, T>) {
    return std::unique_ptr<R>(new R(NEO_FWD(t)));
}

namespace _memory_detail {

template <typename T>
constexpr bool has_value_type = requires { typename T::value_type; };

template <typename T>
constexpr bool has_get_allocator = requires(T & t) { t.get_allocator(); };

}  // namespace _memory_detail

template <typename T>
constexpr auto allocator_of(T&& t, auto alloc = std::allocator<void>{}) noexcept {
    if constexpr (_memory_detail::has_get_allocator<T>) {
        return t.get_allocator();
    } else if constexpr (_memory_detail::has_value_type<T>) {
        return rebind_alloc<typename T::value_type>(alloc);
    } else {
        return alloc;
    }
}

template <typename T>
using get_allocator_t = decltype(allocator_of(NEO_DECLVAL(T)));

}  // namespace neo
