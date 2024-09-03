#pragma once

#include "./concepts.hpp"
#include "./declval.hpp"
#include "./fwd.hpp"
#include "./query.hpp"
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

}  // namespace _memory_detail

/**
 * @brief Query object type that obtains the allocator from an object
 */
struct get_allocator_q : query_interface<get_allocator_q> {
    static constexpr auto exec(const auto& obj) noexcept
        requires requires { obj.get_allocator(); }
    {
        return obj.get_allocator();
    }
};

/**
 * @brief Query object that obtains the allocator associated with an object
 */
inline constexpr get_allocator_q get_allocator;

template <typename T>
constexpr auto allocator_of(T&& t, auto alloc = std::allocator<void>{}) noexcept {
    return query_or(get_allocator, t, alloc);
}

/**
 * @brief Match a type that is an allocator for the given object, according to
 * std::uses_allocator_v
 */
template <typename Allocator, typename T>
concept allocator_for = std::uses_allocator_v<T, Allocator>;

}  // namespace neo
