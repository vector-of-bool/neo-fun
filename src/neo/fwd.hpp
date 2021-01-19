#pragma once

namespace neo {

template <typename T>
struct as_rref {
    using type = T&&;
};

template <typename T>
struct as_rref<T&> {
    using type = T&&;
};

template <typename T>
using as_rref_t = typename as_rref<T>::type;

}  // namespace neo

/**
 * @brief Like std::move, but less overhead
 */
#define NEO_MOVE(...) static_cast<::neo::as_rref_t<decltype(__VA_ARGS__)>>(__VA_ARGS__)

/**
 * @brief Equivalent to std::forward<decltype(Expr)&&>(expr)
 */
#define NEO_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
