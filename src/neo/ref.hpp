#pragma once

#include "./attrib.hpp"
#include "./map_ref.hpp"
#include "./mutref.hpp"
#include "./unref.hpp"

#include <functional>
#include <type_traits>

namespace neo {

/**
 * If `T` is a reference type, the `type` member names a
 * std::reference_wrapper<U>, where `U` is the referred-to type of `T`
 * (including cv-qualifierd). For all other types, the `type` member is `T`
 * unmodified.
 */
template <typename T>
struct wrap_refs {
    using type = T;
};

template <typename T>
requires std::is_reference_v<T>  //
struct wrap_refs<T> {
    using type = std::reference_wrapper<std::remove_reference_t<T>>;
};

/**
 * If the given `T` is a reference, becomes a `std::reference_wrapper<U>` where
 * `U` is the referred-to type. Otherwise, becomes `T` unmodified.
 */
template <typename T>
using wrap_refs_t [[deprecated("Use wrap_ref_member_t and ref_member")]] =
    typename wrap_refs<T>::type;

template <typename T>
using wrap_if_reference_t [[deprecated("Use wrap_ref_member_t and ref_member")]] =
    typename wrap_refs<T>::type;

/**
 * @brief Declare a method 'FuncName' that rebinds a reference-wrapper member 'MemName'
 *
 * 'MemName' should be a datamember declared with wrap_refs_t, and 'Type' should be given
 * as the same argument that was given to wrap_refs_t. The declared method will only be valid
 * if 'Type' is a reference type.
 *
 * @param FuncName The name of the method to declare
 * @param Type The type parameters that was passed to wrap_refs_t for MemName
 * @param MemName The member variable declared with wrap_refs_t to rebind
 */
#define NEO_DECL_REF_REBINDER(FuncName, Type, MemName)                                             \
    constexpr auto&& FuncName(Type& arg) noexcept requires(std::is_reference_v<Type>) {            \
        MemName = std::ref(arg);                                                                   \
        return MemName.get();                                                                      \
    }                                                                                              \
    static_assert(true)

}  // namespace neo
