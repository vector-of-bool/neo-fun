#pragma once

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

}  // namespace neo
