#pragma once

#include <cstdlib>

namespace neo {

/**
 * A simple tag type that holds an arbitrary number of types.
 */
template <typename...>
struct tag {};

/**
 * Variable template of a tag type
 */
template <typename... Ts>
constexpr tag<Ts...> tag_v;

/**
 * Get the number of elements in a tag type.
 */
template <typename Tag>
constexpr std::size_t tag_size_v;

template <typename... Ts>
constexpr std::size_t tag_size_v<tag<Ts...>> = sizeof...(Ts);

} // namespace neo