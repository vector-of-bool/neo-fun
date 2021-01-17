#pragma once

namespace neo {

template <typename T>
using add_rref_t = T&&;

}  // namespace neo

/**
 * @brief Equivalent of std::declval<T>(), but less compiler overhead
 */
#define NEO_DECLVAL(...) (((::neo::add_rref_t<__VA_ARGS__>(*)())(void*)(nullptr))())
