#pragma once

#include <neo/platform.hpp>
#include <neo/pp.hpp>

// clang-format off
#if NEO_COMPILER(MSVC)
#  define NEO_ALWAYS_INLINE __forceinline
#  define NEO_PRETTY_FUNC __FUNCSIG__

#elif NEO_COMPILER(GNU, Clang)
#  define NEO_ALWAYS_INLINE [[gnu::always_inline]] inline
#  define NEO_PRETTY_FUNC __PRETTY_FUNCTION__

#else
#  define NEO_ALWAYS_INLINE                                                                        \
    [[deprecated("NEO_ALWAYS_INLINE is not defined for this compiler. Please submit an issue.")]] inline
#  define NEO_PRETTY_FUNC __func__

#endif

#if __cpp_constexpr_dynamic_alloc
# define NEO_CONSTEXPR_DESTRUCTOR constexpr
#else
# define NEO_CONSTEXPR_DESTRUCTOR inline
#endif

#if NEO_COMPILER(MSVC)
#   define NEO_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#   define NEO_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#define NEO_MSVC_HAS_BUILTIN__has_nothrow_assign
#define NEO_MSVC_HAS_BUILTIN__has_nothrow_move_assign
#define NEO_MSVC_HAS_BUILTIN__has_nothrow_copy
#define NEO_MSVC_HAS_BUILTIN__has_nothrow_constructor
#define NEO_MSVC_HAS_BUILTIN__has_trivial_assign
#define NEO_MSVC_HAS_BUILTIN__has_trivial_move_assign
#define NEO_MSVC_HAS_BUILTIN__has_trivial_copy
#define NEO_MSVC_HAS_BUILTIN__has_trivial_constructor
#define NEO_MSVC_HAS_BUILTIN__has_trivial_move_constructor
#define NEO_MSVC_HAS_BUILTIN__has_trivial_destructor
#define NEO_MSVC_HAS_BUILTIN__has_virtual_destructor
#define NEO_MSVC_HAS_BUILTIN__is_abstract
#define NEO_MSVC_HAS_BUILTIN__is_aggregate
#define NEO_MSVC_HAS_BUILTIN__is_assignable
#define NEO_MSVC_HAS_BUILTIN__is_base_of
#define NEO_MSVC_HAS_BUILTIN__is_class
#define NEO_MSVC_HAS_BUILTIN__is_constructible
#define NEO_MSVC_HAS_BUILTIN__is_convertible_to
#define NEO_MSVC_HAS_BUILTIN__is_corresponding_member
#define NEO_MSVC_HAS_BUILTIN__is_destructible
#define NEO_MSVC_HAS_BUILTIN__is_empty
#define NEO_MSVC_HAS_BUILTIN__is_enum
#define NEO_MSVC_HAS_BUILTIN__is_final
#define NEO_MSVC_HAS_BUILTIN__is_layout_compatible
#define NEO_MSVC_HAS_BUILTIN__is_literal_type
#define NEO_MSVC_HAS_BUILTIN__is_nothrow_assignable
#define NEO_MSVC_HAS_BUILTIN__is_nothrow_constructible
#define NEO_MSVC_HAS_BUILTIN__is_nothrow_destructible
#define NEO_MSVC_HAS_BUILTIN__is_pod
#define NEO_MSVC_HAS_BUILTIN__is_pointer_interconvertible_with_class
#define NEO_MSVC_HAS_BUILTIN__is_polymorphic
#define NEO_MSVC_HAS_BUILTIN__is_referencable
#define NEO_MSVC_HAS_BUILTIN__is_scoped_enum
#define NEO_MSVC_HAS_BUILTIN__is_standard_layout
#define NEO_MSVC_HAS_BUILTIN__is_trivial
#define NEO_MSVC_HAS_BUILTIN__is_trivially_assignable
#define NEO_MSVC_HAS_BUILTIN__is_trivially_constructible
#define NEO_MSVC_HAS_BUILTIN__is_trivially_copyable
#define NEO_MSVC_HAS_BUILTIN__is_trivially_destructible
#define NEO_MSVC_HAS_BUILTIN__is_unbounded_array
#define NEO_MSVC_HAS_BUILTIN__is_union
#define NEO_MSVC_HAS_BUILTIN__underlying_type

#define NEO_MSVC_HAS_BUILTIN__make_integer_seq

#ifdef __has_builtin
#define NEO_HAS_BUILTIN(X) __has_builtin(X)
#elif defined(_MSC_VER)
#define NEO_HAS_BUILTIN(X) NEO_IS_EMPTY(NEO_CONCAT(NEO_MSVC_HAS_BUILTIN, X))
#else
#define NEO_HAS_BUILTIN(X) 0
#endif

// clang-format on
