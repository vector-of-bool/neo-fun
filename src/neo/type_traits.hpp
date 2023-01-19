#pragma once

#include "./attrib.hpp"
#include "./declval.hpp"
#include "./fwd.hpp"
#include "./pp.hpp"

#include <concepts>

namespace neo {

#define _neo_tvar_args_rhs(...) <__VA_ARGS__> NEO_RPAREN

#define NEO_CALLS_TVAR(...) NEO_LPAREN __VA_ARGS__ _neo_tvar_args_rhs

#define NEO_TTRAIT_BUILTIN_OR_VARTMPL(Builtin, VarTmpl)                                            \
    NEO_EVAL_2(                                                                                    \
        NEO_IIF(NEO_HAS_BUILTIN(Builtin), _neoTTraitBuiltin, _neoTTraitVarTmpl)(Builtin, VarTmpl))
#define _neoTTraitBuiltin(Builtin, VarTmpl) NEO_DISCARD(#VarTmpl) Builtin
#define _neoTTraitVarTmpl(Builtin, VarTmpl) NEO_DISCARD(#Builtin) NEO_CALLS_TVAR(VarTmpl)

template <typename T, typename U>
constexpr bool is_same_v = false;

template <typename T>
constexpr bool is_same_v<T, T> = true;

/**
 * @brief Determine whether two types are the same type
 */
#define neo_is_same NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_same, ::neo::is_same_v)

/**
 * @brief Test whether the type T is the same as any type given in U
 *
 * "Weak" because it does not check the symmetric case, as done by same_as
 */
template <typename T, typename... U>
concept weak_same_as = (neo_is_same(T, U) or ...);

namespace detail {

#define BOOL(T)                                                                                    \
    template <typename T>                                                                          \
    constexpr inline bool

#define BOOLX()                                                                                    \
    template <>                                                                                    \
    constexpr inline bool

template <typename T, typename U>
constexpr bool is_same_v = false;
BOOL(T) is_same_v<T, T>  = true;

BOOL(T) is_void_v                      = false;
BOOLX() is_void_v<void>                = true;
BOOLX() is_void_v<const void>          = true;
BOOLX() is_void_v<volatile void>       = true;
BOOLX() is_void_v<const volatile void> = true;

BOOL(T) is_array_v      = false;
BOOL(T) is_array_v<T[]> = true;
template <typename T, std::size_t N>
constexpr bool is_array_v<T[N]> = true;

BOOL(T) is_bounded_array_v = false;
template <typename T, std::size_t N>
constexpr bool is_bounded_array_v<T[N]> = true;

BOOL(T) is_unbounded_array_v      = false;
BOOL(T) is_unbounded_array_v<T[]> = true;

BOOL(T) is_const_v          = false;
BOOL(T) is_const_v<const T> = true;

BOOL(T) is_volatile_v             = false;
BOOL(T) is_volatile_v<volatile T> = true;

BOOL(T) is_lref_v     = false;
BOOL(T) is_lref_v<T&> = true;

BOOL(T) is_rref_v      = false;
BOOL(T) is_rref_v<T&&> = true;

BOOL(T) is_ref_v      = false;
BOOL(T) is_ref_v<T&>  = true;
BOOL(T) is_ref_v<T&&> = true;

BOOL(T) is_pointer_v                    = false;
BOOL(T) is_pointer_v<T*>                = false;
BOOL(T) is_pointer_v<T* const>          = false;
BOOL(T) is_pointer_v<T* const volatile> = false;
BOOL(T) is_pointer_v<T* volatile>       = false;

BOOL(T) is_null_pointer_v                                = false;
BOOLX() is_null_pointer_v<std::nullptr_t>                = true;
BOOLX() is_null_pointer_v<std::nullptr_t const>          = true;
BOOLX() is_null_pointer_v<std::nullptr_t volatile>       = true;
BOOLX() is_null_pointer_v<std::nullptr_t const volatile> = true;

}  // namespace detail

#define DECL_TRAIT_CONCEPT(Name, TraitMacro)                                                       \
    template <typename T>                                                                          \
    concept Name = TraitMacro(T)

/**
 * @brief Expand to the fastext implementation of is_void available
 */
// note: __is_void is weird on Clang, and needs to be forced to bool
#define neo_is_void !!NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_void, ::neo::detail::is_void_v)

/**
 * @brief Match a (cv-qualified) void type
 */
DECL_TRAIT_CONCEPT(void_type, neo_is_void);

#define neo_is_null_pointer(...)                                                                   \
    NEO_IIF(NEO_HAS_BUILTIN(__is_nullptr), __is_nullptr(__VA_ARGS__), _neo_is_nullptr(__VA_ARGS__))
#define _neo_is_nullptr(...)                                                                       \
    NEO_IIF(NEO_HAS_BUILTIN(__is_same),                                                            \
            (neo_is_same(__VA_ARGS__, decltype(nullptr))                                           \
             or neo_is_same(__VA_ARGS__, decltype(nullptr) const)                                  \
             or neo_is_same(__VA_ARGS__, decltype(nullptr) const volatile)                         \
             or neo_is_same(__VA_ARGS__, decltype(nullptr) volatile)),                             \
            NEO_IIF(NEO_HAS_BUILTIN(__remove_cv),                                                  \
                    neo_is_same(__remove_cv(__VA_ARGS__), decltype(nullptr)),                      \
                    ::neo::detail::is_null_pointer_v<__VA_ARGS__>))
DECL_TRAIT_CONCEPT(null_pointer, neo_is_null_pointer);

#define neo_is_array NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_array, ::neo::detail::is_array_v)
DECL_TRAIT_CONCEPT(array_type, neo_is_array);

#define neo_is_const NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_const, ::neo::detail::is_const_v)
DECL_TRAIT_CONCEPT(const_type, neo_is_const);

#define neo_is_volatile NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_volatile, ::neo::detail::is_volatile_v)
DECL_TRAIT_CONCEPT(volatile_type, neo_is_volatile);

#define neo_is_trivial NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_trivial, ::std::is_trivial_v)
#define neo_is_trivially_copyayable                                                                \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_trivially_copyable, ::std::is_trivially_copyable_v)
#define neo_is_standard_layout                                                                     \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_standard_layout, ::std::is_standard_layout_v)
#define neo_is_polymorphic NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_polymorphic, ::std::is_polymorphic_v)
#define neo_is_abstract NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_abstract, ::std::is_abstract_v)
#define neo_is_final NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_final, ::std::is_final_v)
#define neo_is_aggregate NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_aggregate, ::std::is_aggregate_v)
#define neo_is_signed NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_signed, ::std::is_signed_v)
#define neo_is_unsigned NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_unsigned, ::std::is_unsigned_v)
#define neo_is_scoped_enum NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_scoped_enum, ::std::is_scoped_enum_v)
#define neo_is_bounded_array                                                                       \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_bounded_array, ::neo::detail::is_bounded_array_v)
#define neo_is_unbounded_array                                                                     \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_unbounded_array, ::neo::detail::is_unbounded_array_v)

#define neo_is_enum NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_enum, ::std::is_enum_v)
DECL_TRAIT_CONCEPT(enum_type, neo_is_enum);

#define neo_is_union NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_union, ::std::is_union_v)
DECL_TRAIT_CONCEPT(union_type, neo_is_union);

#define neo_is_class NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_class, ::std::is_class_v)
DECL_TRAIT_CONCEPT(class_type, neo_is_class);

#define neo_is_lvalue_reference                                                                    \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_lvalue_reference, ::neo::detail::is_lref_v)
DECL_TRAIT_CONCEPT(lvalue_reference_type, neo_is_lvalue_reference);

#define neo_is_rvalue_reference                                                                    \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_rvalue_reference, ::neo::detail::is_rref_v)
DECL_TRAIT_CONCEPT(rvalue_reference_type, neo_is_rvalue_reference);

#define neo_is_reference(...)                                                                      \
    neo_is_lvalue_reference(__VA_ARGS__) or neo_is_rvalue_reference(__VA_ARGS__)

template <typename T>
concept reference_type = lvalue_reference_type<T> or rvalue_reference_type<T>;

#define neo_is_pointer NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_pointer, ::neo::detail::is_pointer_v)
DECL_TRAIT_CONCEPT(pointer_type, neo_is_pointer);

#define neo_is_program_defined(...)                                                                \
    (neo_is_class(__VA_ARGS__) or neo_is_union(__VA_ARGS__) or neo_is_enum(__VA_ARGS__))

#if NEO_HAS_BUILTIN(__is_function)
#define neo_is_function __is_function
#elif defined(_MSC_VER)
// MSVC emits a warning about our trickery with 'const', so just defined it in terms of the actual
// stdlib impl
#define neo_is_function(...) (::std::is_function_v<__VA_ARGS__>)
#else
#define neo_is_function(...)                                                                       \
    (not neo_is_const(__VA_ARGS__ const) and not neo_is_reference(__VA_ARGS__))
#endif

#define DECL_UNARY_TRANSFORM(Name) declUnaryTransform(NEO_CONCAT(__, Name), NEO_CONCAT(Name, _t))
#define declUnaryTransform(Builtin, Trait)                                                         \
    template <typename T>                                                                          \
    using Trait = NEO_IF_ELSE(NEO_HAS_BUILTIN(Builtin))(Builtin(T))(::std::Trait<T>)

DECL_UNARY_TRANSFORM(add_lvalue_reference);
DECL_UNARY_TRANSFORM(add_pointer);
DECL_UNARY_TRANSFORM(add_rvalue_reference);
DECL_UNARY_TRANSFORM(decay);
// DECL_UNARY_TRANSFORM(make_signed);    // Clang reports available, but they're not?
// DECL_UNARY_TRANSFORM(make_unsigned);  // ''
DECL_UNARY_TRANSFORM(remove_all_extents);
DECL_UNARY_TRANSFORM(remove_const);
DECL_UNARY_TRANSFORM(remove_cv);
DECL_UNARY_TRANSFORM(remove_cvref);
DECL_UNARY_TRANSFORM(remove_extent);
DECL_UNARY_TRANSFORM(remove_pointer);
DECL_UNARY_TRANSFORM(remove_reference);
DECL_UNARY_TRANSFORM(remove_volatile);
DECL_UNARY_TRANSFORM(underlying_type);

#undef declUnaryTransform
#undef DECL_UNARY_TRANSFORM

template <typename T, typename Target>
concept alike = weak_same_as<remove_cvref_t<T>, remove_cvref_t<Target>>;

template <typename T, typename Target>
concept unalike = not
weak_same_as<remove_cvref_t<T>, remove_cvref_t<Target>>;

namespace detail {

template <typename T>
constexpr bool integral_v = requires {
                                requires not neo_is_class(T);
                                requires weak_same_as<decay_t<T>,
                                                      bool,
                                                      char,
                                                      signed char,
                                                      unsigned char,
                                                      wchar_t,
                                                      char8_t,
                                                      char16_t,
                                                      char32_t,
                                                      short,
                                                      unsigned short,
                                                      int,
                                                      unsigned int,
                                                      long,
                                                      unsigned long,
                                                      long long,
                                                      unsigned long long>;
                            };

template <typename T>
constexpr bool floating_point_v = requires {
                                      requires not neo_is_class(T);
                                      requires weak_same_as<decay_t<T>, float, double, long double>;
                                  };

}  // namespace detail

#define neo_is_integral NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_integral, ::neo::detail::integral_v)
#define neo_is_floating_point                                                                      \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_floating_point, ::neo::detail::floating_point_v)

#if NEO_HAS_BUILTIN(__is_arithmetic)
#define neo_is_arithmetic __is_arithmetic
#else
#define neo_is_arithmetic(...) (neo_is_integral(__VA_ARGS__) or neo_is_floating_point(__VA_ARGS__))
#endif

namespace detail {

BOOL(T) is_memptr_v = false;
template <typename T, typename U>
constexpr inline bool is_memptr_v<T U::*> = true;

BOOL(T) is_memfnptr_v = false;
template <typename T, typename U>
constexpr inline bool is_memfnptr_v<T U::*> = neo_is_function(T);

}  // namespace detail

#define neo_is_member_pointer                                                                      \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_member_pointer, ::neo::detail::is_memptr_v)

#define neo_is_member_function_pointer                                                             \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_member_function_pointer, ::neo::detail::is_memfnptr_v)

#if NEO_HAS_BUILTIN(__is_member_object_pointer)
#define neo_is_member_object_pointer __is_member_object_pointer
#else
#define neo_is_member_object_pointer(...)                                                          \
    (neo_is_member_pointer(__VA_ARGS__) and not neo_is_member_function_pointer(__VA_ARGS__))
#endif

#define neo_is_funamental(...)                                                                     \
    NEO_IIF(NEO_HAS_BUILTIN(__is_fundamental),                                                     \
            __is_fundamental(__VA_ARGS__),                                                         \
            (neo_is_arithmetic(__VA_ARGS__) or neo_is_void(__VA_ARGS__)                            \
             or neo_is_null_pointer(__VA_ARGS__)))

#if NEO_HAS_BUILTIN(__is_scalar)
#define neo_is_scalar __is_scalar
#else
#define neo_is_scalar(...)                                                                         \
    (neo_is_arithmetic(__VA_ARGS__) or neo_is_enum(__VA_ARGS__) or neo_is_pointer(__VA_ARGS__)     \
     or neo_is_member_pointer(__VA_ARGS__) or neo_is_null_pointer(__VA_ARGS__))
#endif
DECL_TRAIT_CONCEPT(scalar_type, neo_is_scalar);

#if NEO_HAS_BUILTIN(__is_object)
#define neo_is_object __is_object
#else
#define neo_is_object(...)                                                                         \
    (not(neo_is_function(__VA_ARGS__) or neo_is_void(__VA_ARGS__) or neo_is_reference(__VA_ARGS__)))
#endif
DECL_TRAIT_CONCEPT(object_type, neo_is_object);

#define neo_is_base_of NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_base_of, ::std::is_base_of_v)

#define neo_is_trivially_constructible                                                             \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_trivially_constructible, ::std::is_trivially_constructible_v)
#define neo_is_trivially_destructible                                                              \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_trivially_destructible, ::std::is_trivially_destructible_v)
#define neo_is_trivially_assignable                                                                \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_trivially_assignable, ::std::is_trivially_assignable_v)

namespace detail {

template <typename T>
constexpr std::size_t rank_v = 0;

template <typename T>
constexpr std::size_t rank_v<T[]> = rank_v<T> + 1;

template <typename T, std::size_t N>
constexpr std::size_t rank_v<T[N]> = rank_v<T> + 1;

}  // namespace detail

#if NEO_HAS_BUILTIN(__array_rank) && 0  // Disable: __array_rank doesn't handle unbounded arrays?
#define neo_array_rank __array_rank
#else
#define neo_array_rank(...) ::neo::detail::rank_v<__VA_ARGS__>
#endif

namespace detail {

template <bool B>
struct conditional_ {
    template <typename, typename Else>
    using f = Else;
};

template <>
struct conditional_<true> {
    template <typename Then, typename>
    using f = Then;
};

}  // namespace detail

template <bool Cond, typename Then, typename Else>
using conditional_t = typename detail::conditional_<Cond>::template f<Then, Else>;

namespace detail {

template <typename T>
using with_reference_t = T&;

template <typename T>
concept can_reference = requires { typename with_reference_t<T>; };

}  // namespace detail

#define neo_is_referencable                                                                        \
    NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_referencable, ::neo::detail::can_reference)

template <typename T>
using const_reference_t = add_lvalue_reference_t<const remove_reference_t<T>>;

namespace detail {

template <std::size_t N>
struct common_type;

}

template <typename... Ts>
using common_type_t = typename detail::common_type<sizeof...(Ts)>::template impl<Ts...>::type;

template <typename... Ts>
concept has_common_type = requires { typename common_type_t<Ts...>; };

template <>
struct detail::common_type<0> {};

template <typename T, typename U>
struct basic_common_type {};

template <>
struct detail::common_type<2> {
    template <typename T, typename U, typename Td = decay_t<T>, typename Ud = decay_t<U>>
    struct impl_try_ternary : impl_try_ternary<Td, Ud, Td, Ud> {};

    template <typename T, typename U>
    struct impl_try_basic;

    template <typename T, typename U>
        requires requires { 0 ? NEO_DECLVAL(T) : NEO_DECLVAL(U); }
    struct impl_try_ternary<T, U, T, U> {
        // T and U are already decayed and have a language-level common type
        using type = decay_t<decltype(0 ? NEO_DECLVAL(T) : NEO_DECLVAL(U))>;
    };

    template <typename T, typename U>
    // Fallback to the next level:
    struct impl_try_ternary<T, U, T, U> : impl_try_basic<T, U> {};

    // Base case: Fall-back to basic_common_type
    template <typename T, typename U>
    struct impl_try_basic : basic_common_type<T, U> {};

    // C++20: Check if we get a type with const_reference binding:
    template <typename T, typename U>
        requires requires {
                     0 ? NEO_DECLVAL(const_reference_t<T>) : NEO_DECLVAL(const_reference_t<U>);
                 }
    struct impl_try_basic<T, U> {
        using type = decay_t<decltype(0 ? NEO_DECLVAL(const_reference_t<T>)
                                        : NEO_DECLVAL(const_reference_t<U>))>;
    };

    template <typename T, typename U>
    struct impl : impl_try_ternary<T, U> {};
};

template <>
struct detail::common_type<1> {
    template <typename T>
    struct impl {
        using type = T;
    };
};

template <std::size_t N>
struct detail::common_type {
    template <typename T, typename U, typename... V>
        requires has_common_type<T, U> and has_common_type<V...>
    struct impl {
        using type = common_type_t<common_type_t<T, U>, common_type_t<V...>>;
    };
};

enum class reference_kind { none, lvalue, rvalue };

enum class cvref_kind {
    none,

    lref      = 1 << 0,
    rref      = 1 << 1,
    const_    = 1 << 2,
    volatile_ = 1 << 3,

    const_volatile = const_ | volatile_,

    const_lref = const_ | lref,
    const_rref = const_ | rref,

    volatile_lref = volatile_ | lref,
    volatile_rref = volatile_ | rref,

    const_volatile_lref = const_ | volatile_ | lref,
    const_volatile_rref = const_ | volatile_ | rref,
};

constexpr cvref_kind operator&(cvref_kind l, cvref_kind r) noexcept {
    int ret = int(l) & int(r);
    return cvref_kind(ret);
}

constexpr cvref_kind operator|(cvref_kind l, cvref_kind r) noexcept {
    int ret = int(l) | int(r);
    if (ret & int(cvref_kind::lref)) {
        ret &= ~int(cvref_kind::rref);
    }
    return cvref_kind(ret);
}

template <typename T>
constexpr cvref_kind cvref_kind_v = cvref_kind::none;

template <typename T>
constexpr cvref_kind cvref_kind_v<const T> = cvref_kind::const_;

template <typename T>
constexpr cvref_kind cvref_kind_v<volatile T> = cvref_kind::volatile_;

template <typename T>
constexpr cvref_kind cvref_kind_v<const volatile T> = cvref_kind::const_volatile;

template <typename T>
constexpr cvref_kind cvref_kind_v<T&> = cvref_kind::lref | cvref_kind_v<T>;

template <typename T>
constexpr cvref_kind cvref_kind_v<T&&> = cvref_kind::lref | cvref_kind_v<T>;

template <cvref_kind K>
struct cvref_apply;

#define DECL_APPLY(Kind, X)                                                                        \
    template <>                                                                                    \
    struct cvref_apply<cvref_kind::Kind> {                                                         \
        template <typename T>                                                                      \
        using f = X;                                                                               \
    }

DECL_APPLY(none, T);
DECL_APPLY(lref, T&);
DECL_APPLY(rref, T&&);
DECL_APPLY(const_, const T);
DECL_APPLY(volatile_, volatile T);
DECL_APPLY(const_volatile, const volatile T);
DECL_APPLY(const_lref, const T&);
DECL_APPLY(volatile_lref, volatile T&);
DECL_APPLY(const_volatile_lref, const volatile T&);
DECL_APPLY(const_rref, const T&&);
DECL_APPLY(volatile_rref, volatile T&&);
DECL_APPLY(const_volatile_rref, const volatile T&&);

#undef DECL_APPLY

template <typename T>
using copy_cvref_from = cvref_apply<cvref_kind_v<T>>;

namespace detail {

template <std::size_t N>
struct common_reference;

}  // namespace detail

template <typename... Ts>
using common_reference_t =
    typename detail::common_reference<sizeof...(Ts)>::template impl<Ts...>::type;

template <typename... Ts>
concept has_common_reference = requires { typename common_reference_t<Ts...>; };

template <typename T,
          typename U,
          template <class> class Tq = copy_cvref_from<T>::template f,
          template <class> class Uq = copy_cvref_from<U>::template f>
using basic_common_reference_t =
    typename std::basic_common_reference<remove_cvref_t<T>, remove_cvref_t<U>, Tq, Uq>::type;

template <>
struct detail::common_reference<0> {};

template <>
struct detail::common_reference<1> {
    template <typename T>
    struct impl {
        using type = T;
    };
};

template <>
struct detail::common_reference<2> {
    template <typename T, typename U, template <class> class Q>
    struct impl_try_simple;

    template <typename T, typename U>
    struct impl_try_basic;

    template <typename T, typename U>
    struct impl_try_common_type;

    template <typename T, typename U, template <class> class Q>
    using impl_t = typename impl_try_simple<T, U, Q>::type;

    //* Simple common (both references):

    // 1: Both lrefs:
    template <lvalue_reference_type T, lvalue_reference_type U, template <class> class Q>
    // Require a common reference according to ternary:
        requires requires(Q<T> t, Q<U> u) { 0 ? t : u; }
    struct impl_try_simple<T, U, Q> {
        using type = decltype(0 ? NEO_DECLVAL(Q<T>) : NEO_DECLVAL(Q<U>));
    };

    // 2: Both rrefs:
    template <rvalue_reference_type T, rvalue_reference_type U, template <class> class Q>
    // Both must be convertible to the base type:
        requires requires(void (*fn)(remove_reference_t<impl_t<T&, U&, Q>>&&), T t, U u) {
                     fn(NEO_FWD(t));
                     fn(NEO_FWD(u));
                 }
    struct impl_try_simple<T, U, Q> {
        using type = remove_reference_t<impl_t<T&, U&, Q>>&&;
    };

    // 3: Left is lref, Right is rref:
    template <lvalue_reference_type T, rvalue_reference_type U, template <class> class Q>
    struct impl_try_simple<T, U, Q> : impl_try_simple<T&, const U&, Q> {};

    // 3: Swap left/right:
    template <rvalue_reference_type T, lvalue_reference_type U, template <class> class Q>
    struct impl_try_simple<T, U, Q> : impl_try_simple<U&, T const&, Q> {};

    // 5: Fallback, no common reference type between two references, or one operand is not a
    // reference
    template <typename T, typename U, template <class> class Q>
    struct impl_try_simple : impl_try_basic<T, U> {};

    // 5: Use a specialization of basic_common_reference:
    template <typename T, typename U>
        requires requires { typename basic_common_reference_t<T, U>; }
    struct impl_try_basic<T, U> {
        using type = basic_common_reference_t<T, U>;
    };

    // 6: Fallback, no basic_common_reference, so use common_type:
    template <typename T, typename U>
    struct impl_try_basic : impl_try_common_type<T, U> {};

    // 6: Try to use common_type_t
    template <typename T, typename U>
        requires has_common_type<T, U>
    struct impl_try_common_type<T, U> {
        using type = common_type_t<T, U>;
    };

    template <typename T, typename U>
    // 7: Fail: No common_type either, so no reference is available
    struct impl_try_common_type {};

    template <typename T, typename U>
    using shared_cv
        = cvref_apply<cvref_kind_v<remove_reference_t<T>> | cvref_kind_v<remove_reference_t<U>>>;

    template <typename T, typename U>
    struct impl : impl_try_simple<T, U, shared_cv<T, U>::template f> {};
};

template <std::size_t>
struct detail::common_reference {
    template <typename T, typename U, typename... Vs>
        requires has_common_reference<T, U> and has_common_reference<Vs...>
    struct impl : common_reference<1>::template impl<
                      common_reference_t<common_reference_t<T, U>, common_reference_t<Vs...>>> {};
};

}  // namespace neo

#undef BOOL
#undef BOOLX
#undef DECL_TRAIT_CONCEPT
