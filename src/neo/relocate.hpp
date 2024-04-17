#pragma once

#include "./enable_trivially_relocatable.hpp"

#include <neo/addressof.hpp>
#include <neo/concepts.hpp>
#include <neo/platform.hpp>
#include <neo/scope.hpp>
#include <neo/type_traits.hpp>

#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <vector>
#include <version>

namespace neo {

template <typename T>
concept relocatable = move_constructible<T>;

/**
 * @brief Match any type that is trivially relocatable
 */
template <typename T>
concept trivially_relocatable = relocatable<T> and enable_trivially_relocatable<T>;

/**
 * @brief In-place construct a new instance of `T` by move-constructing from the pointed-to `src`
 * object, and then destroy `src`.
 *
 * The pointer `dest` must point to un-alive storage for `T`. Upon returning,
 * the pointer `src` points to no-longer-alive storage for `T`.
 *
 * Returns a pointer to the constructed object.
 *
 * If the move constructor throws an exception, then `src` will still be destroyed.
 */
template <typename T>
    requires constructible_from<T, T&&>
constexpr T* relocate_at(T* dest, T* src) noexcept(nothrow_constructible_from<T, T&&>) {
    if (std::is_constant_evaluated() or not trivially_relocatable<T>) {
        struct guard {
            T* tmp;
            constexpr ~guard() { std::destroy_at(tmp); }
        } _{src};
        return std::construct_at(dest, static_cast<T&&>(*src));
    } else {
        std::memmove(dest, src, sizeof(T));
        return std::launder(dest);
    }
}

namespace _relocate_detail {

// Enable the magic manual-RVO trick below
template <typename T>
concept object_returned_via_invisible_first_param_pointer
    // Not applicable on Windows (XXX: Maybe it is?)
    = not os_is_windows
    // Require certain attributes:
    and (
        // Non-trivial objects are put into hidden by-ref returns
        not neo_is_trivially_destructible(T)
        // Large objects are returned by hidden reference
        or sizeof(T) > sizeof(void*) * 8);

}  // namespace _relocate_detail

template <typename T>
    requires constructible_from<T, T&&>
constexpr remove_cvref_t<T> relocate(T* src) noexcept(nothrow_constructible_from<T, T&&>) {
    if (std::is_constant_evaluated() or not trivially_relocatable<T>
        or not _relocate_detail::object_returned_via_invisible_first_param_pointer<T>) {
        struct guard {
            T* tmp;
            constexpr ~guard() { std::destroy_at(tmp); }
        } _{src};
        return static_cast<T&&>(*src);
    } else {
        /**
         * Absolute madness: In some conditions on some ABIs, the return value is
         * handled via an invisible pointer passed as-if by the first function parameter, and
         * also that pointer being the return value of the function. We can make use of this
         * fact to manually force RVO by calling `memcpy` via a function pointer type
         * that coincidentally aligns the stars such that we get the effect of
         * return-by-value with memcpying `src` into the return value destination.
         * This is absolute vile UB, but it works in these very limited circumstances.
         */
        NEO_PRAGMA_WARNING_PUSH();
        NEO_GNU_LIKE_PRAGMA(GCC diagnostic ignored "-Wcast-function-type");
        auto itanium_magic = reinterpret_cast<T (*)(T*, size_t)>(memcpy);
        NEO_PRAGMA_WARNING_POP();
        return itanium_magic(src, sizeof(T));
    }
}

template <typename T, typename D>
constexpr bool enable_trivially_relocatable<std::unique_ptr<T, D>> = trivially_relocatable<D>;
template <typename T>
constexpr bool enable_trivially_relocatable<std::shared_ptr<T>> = true;
template <typename T>
constexpr bool enable_trivially_relocatable<std::weak_ptr<T>> = true;
template <typename T>
constexpr bool enable_trivially_relocatable<std::allocator<T>> = true;
template <>
constexpr inline bool enable_trivially_relocatable<std::exception_ptr> = true;
template <typename T, std::size_t N>
constexpr inline bool enable_trivially_relocatable<std::array<T, N>> = trivially_relocatable<T>;
#if !defined(_LIBCPP_DEBUG_LEVEL) || (_LIBCPP_DEBUG_LEVEL < 2)
template <typename T, typename Alloc>
constexpr inline bool enable_trivially_relocatable<std::vector<T, Alloc>>
    = trivially_relocatable<T> and trivially_relocatable<Alloc>;
#endif

}  // namespace neo
