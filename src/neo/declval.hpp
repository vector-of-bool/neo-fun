#pragma once

#include <neo/attrib.hpp>

namespace neo {

template <typename T>
using add_rref_t = T&&;
#define _neoDeclVal(...) (((::neo::add_rref_t<__VA_ARGS__> (*)())(void*)(nullptr))())

}  // namespace neo

/**
 * @brief Equivalent of std::declval<T>(), but less compiler overhead
 */
#define NEO_DECLVAL(...) _neoDeclVal(__VA_ARGS__)
