#pragma once

#include <neo/concepts.hpp>
#include <neo/type_traits.hpp>

#include <version>

namespace neo {

#ifdef __cpp_lib_trivially_relocatable
template <typename T>
constexpr bool std_is_trivially_relocatable_v = std::is_trivially_relocatable_v<T>;
#else
template <typename T>
constexpr bool std_is_trivially_relocatable_v = false;
#endif

/**
 * @brief Variable template that can be specialized to enable trivial relocation
 *
 * @tparam T
 */
template <typename T>
constexpr bool enable_trivially_relocatable  //
    = neo_is_trivially_copyayable(T)         //
    or neo_is_reference(T)                   //
    or requires { requires static_cast<bool>(T::enable_trivially_relocatable); }
    or std_is_trivially_relocatable_v<T>;

}  // namespace neo