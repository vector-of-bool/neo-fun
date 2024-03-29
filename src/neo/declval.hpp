#pragma once

#include <neo/attrib.hpp>

namespace neo {

#if NEO_HAS_BUILTIN(__add_rvalue_reference)
template <typename T>
using add_rref_t = __add_rvalue_reference(T);
#define _neoDeclVal(...) (((__add_rvalue_reference(__VA_ARGS__)(*)())(void*)(nullptr))())
#else
template <typename T>
using add_rref_t = T&&;
#define _neoDeclVal(...) (((::neo::add_rref_t<__VA_ARGS__>(*)())(void*)(nullptr))())
#endif

}  // namespace neo

/**
 * @brief Equivalent of std::declval<T>(), but less compiler overhead
 */
#define NEO_DECLVAL(...) _neoDeclVal(__VA_ARGS__)
