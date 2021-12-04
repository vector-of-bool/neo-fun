#pragma once

namespace neo {

template <typename T>
class opt_ref;

template <typename T>
constexpr bool is_opt_ref_v = false;

template <typename T>
constexpr bool is_opt_ref_v<opt_ref<T>> = true;

}  // namespace neo