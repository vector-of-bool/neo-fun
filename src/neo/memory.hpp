#pragma once

#include <memory>

namespace neo {

/**
 * @brief Rebind the given allocator to a new type
 *
 * Equivalent to:
 *
 *      std::allocator_traits<Alloc>::rebind_alloc<T>;
 *
 */
template <typename Alloc, typename T>
using rebind_alloc_t = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;

}  // namespace neo
