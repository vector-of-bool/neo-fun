#pragma once
#include <type_traits>

namespace neo {

template <typename T>
extern T&& ref_v;

template <typename T>
extern const T&& cref_v;

template <typename T>
extern const T& cref_v<T&>;

template <typename T>
extern const T&& cref_v<T&&>;

}  // namespace neo
