#pragma once

#include <neo/attrib.hpp>

namespace neo::move_detail {

template <typename T>
struct strip_type {
    using type = T;
};

template <typename T>
struct strip_type<T&> {
    using type = T;
};

template <typename T>
struct strip_type<T&&> {
    using type = T;
};

#if NEO_HAS_BUILTIN(__remove_reference)
#define _neo_typeof_noref(...) __remove_reference(decltype(__VA_ARGS__))
#else
#define _neo_typeof_noref(...) typename ::neo::move_detail::strip_type<decltype(__VA_ARGS__)>::type
#endif

}  // namespace neo::move_detail

/**
 * @brief Like std::move, but less overhead
 */
#define NEO_MOVE(...) static_cast<_neo_typeof_noref(__VA_ARGS__) &&>(__VA_ARGS__)

/**
 * @brief Equivalent to std::forward<decltype(Expr)&&>(expr)
 */
#define NEO_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
