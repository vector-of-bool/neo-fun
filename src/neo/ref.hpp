#pragma once

#include "./addressof.hpp"
#include "./attrib.hpp"
#include "./concepts.hpp"
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

template <reference_type T>
struct wrap_refs<T> {
    using type = std::reference_wrapper<remove_reference_t<T>>;
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
    constexpr auto&& FuncName(Type& arg) noexcept                                                  \
        requires(reference_typeType >)                                                             \
    {                                                                                              \
        MemName = std::ref(arg);                                                                   \
        return MemName.get();                                                                      \
    }                                                                                              \
    static_assert(true)

/**
 * @brief Defines an object-type that represents a non-nullable reference to another type. Acts
 * as a non-nullable pointer in that assignment and comparison acts on the reference rather than
 * the referred-to object.
 *
 * To get the underlying referred-to object, use a `static_cast` to get back to a C++ language
 * reference type.
 *
 * @tparam T A reference type that we represent
 */
template <reference_type T>
class reference_object {
public:
    using reference_type = T;
    using pointer_type   = add_pointer_t<T>;

public:
    // "Bind" the new reference
    constexpr reference_object(T ref) noexcept
        : _pointer(NEO_ADDRESSOF(ref)) {}

    // Return to the language-level reference type that we represent
    NEO_ALWAYS_INLINE constexpr explicit operator T() const noexcept { return *_pointer; }

    // Compare two references by comparing the wrapped object addresses
    bool operator==(const reference_object&) const  = default;
    auto operator<=>(const reference_object&) const = default;

private:
    pointer_type _pointer;
};

}  // namespace neo
