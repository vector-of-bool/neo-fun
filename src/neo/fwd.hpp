#pragma once

/**
 * @brief Equivalent to std::forward<decltype(Expr)&&>(expr)
 */
#define NEO_FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
