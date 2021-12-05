#pragma once

#include "./fwd.hpp"

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
 * @brief Create a decay-copy of the given object wrapped in a shared_ptr
 */
template <typename T, typename R = std::remove_cvref_t<T>>
constexpr auto copy_shared(T&& t) noexcept(std::is_nothrow_constructible_v<R, T>) {
    return std::shared_ptr<R>(new R(NEO_FWD(t)));
}

/**
 * @brief Create a decay-copy of the given object wrapped in a unique_ptr
 */
template <typename T, typename R = std::remove_cvref_t<T>>
constexpr auto copy_unique(T&& t) noexcept(std::is_nothrow_constructible_v<R, T>) {
    return std::unique_ptr<R>(new R(NEO_FWD(t)));
}

}  // namespace neo
