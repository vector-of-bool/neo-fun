#pragma once

namespace neo {

template <typename T>
T deduce_typeof_f(const T&);

template <typename T>
T deduce_typeof_f(const volatile T&);

}  // namespace neo

#if defined __GNUC__ || defined __clang__
#define NEO_TYPEOF(X) __typeof__((X))
#else
#define NEO_TYPEOF(X) decltype(::neo::deduce_typeof_f((X)))
#endif

/**
 * @brief Like std::move, but less overhead
 */
#define NEO_MOVE(...) static_cast<NEO_TYPEOF(__VA_ARGS__) &&>(__VA_ARGS__)

/**
 * @brief Equivalent to std::forward<decltype(Expr)&&>(expr)
 */
#define NEO_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
