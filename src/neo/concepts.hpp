#pragma once

#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./type_traits.hpp"
#include "./version.hpp"

#include <concepts>
#include <type_traits>

// clang-format off
namespace neo {

template <typename T, typename U>
concept same_as = weak_same_as<T, U> and weak_same_as<U, T>;

template <typename B>
concept simple_boolean = requires(B b) {
    b ? 0 : 0;
};

namespace detail {

template <typename T>
concept destructible_nocvr = requires (T& v) {
    { v.~T() } noexcept;
};

} // namespace detail

/// Faster variant of std::destructible
template <typename T>
concept destructible = requires {
#if NEO_HAS_BUILTIN(__is_nothrow_destructible)
    requires __is_nothrow_destructible(T);
#else
    requires neo_is_reference(T)
          or neo_is_bounded_array(T)
          or detail::destructible_nocvr<remove_cvref_t<T>>;
#endif
};

/// Faster variant of std::constructible_from
template <typename T, typename... Args>
concept constructible_from =
        destructible<T>
    and requires(T value, Args&&... args) { T(NEO_FWD(args)...); }
    ;

/// Check whether one can static_cast<To>(From)
template <typename From, typename To>
concept explicit_convertible_to =
    requires(add_rvalue_reference_t<From>(&fn)()) {
        static_cast<To>(fn());
    };

template <typename From, typename To>
concept convertible_to =
        explicit_convertible_to<From, To>
    and requires {
        requires
            neo_is_void(To) or
            #if NEO_HAS_BUILTIN(__is_convertible_to) || _MSC_VER
                // Failure here indicates that the object can be explicitly converted to
                // the target, but not *implicitly* converted:
                __is_convertible_to(From, To)
            #else
                requires (void(&to_fn)(To arg), add_rvalue_reference_t<From>(&from_fn)()) {
                    to_fn(from_fn());
                }
            #endif
            ;
    };

#define neo_is_convertible_to NEO_TTRAIT_BUILTIN_OR_VARTMPL(__is_convertible_to, ::neo::convertible_to)

template <typename Derived, typename Base>
concept derived_from = requires { requires
        neo_is_base_of(Base, Derived)
    and convertible_to<const volatile Derived*, const volatile Base*>;
};

namespace detail {

template <typename T, typename U, typename CommonRef>
concept common_reference_with_1 = requires { requires
        same_as<CommonRef, std::common_reference_t<U, T>>
    and convertible_to<T, CommonRef>
    and convertible_to<U, CommonRef>;
};

} // namespace detail

template <typename T, typename U>
concept common_reference_with = requires { requires
       neo_is_same(T, U)
    or alike<T, U>
    or detail::common_reference_with_1<T, U, std::common_reference_t<T, U>>;
};

namespace detail {

template <typename T, typename U, typename Common>
concept common_with_1 = requires { requires
        same_as<Common, std::common_type_t<U, T>>
    and convertible_to<T, Common>
    and convertible_to<U, Common>
    and common_reference_with<
        add_lvalue_reference_t<const T>,
        add_lvalue_reference_t<const U>>
    and common_reference_with<
        add_lvalue_reference_t<Common>,
        std::common_reference_t<
            add_lvalue_reference_t<const T>,
            add_lvalue_reference_t<const U>>>;
};

} // namespace detail

template <typename T, typename U>
concept common_with = requires { requires
       alike<T, U>
    or detail::common_with_1<T, U, std::common_type_t<T, U>>;
};

template <typename T>
concept integral = neo_is_integral(T);

template <typename T>
concept signed_integral = integral<T> and neo_is_signed(T);

template <typename T>
concept unsigned_integral = integral<T> and neo_is_unsigned(T);

template <typename T>
concept floating_point = neo_is_floating_point(T);

template <typename LHS, typename RHS>
concept assignable_from =
    neo_is_lvalue_reference(LHS)
    and requires(LHS lhs, RHS&& rhs) {
        { NEO_FWD(lhs) = NEO_FWD(rhs) } -> weak_same_as<LHS>;
    }
    ;

template <typename T>
concept default_initializable =
        constructible_from<T>
    and requires { T{}; }
    // The default constructor must not be 'explicit':
    and requires { ::new(static_cast<void*>(nullptr)) T; };

template <typename T>
concept move_constructible =
        constructible_from<T, T>
    and convertible_to<T, T>;

template <typename T>
concept copy_constructible =
        move_constructible<T>
    and requires {
        requires convertible_to<T&, T>;
        requires convertible_to<const T&, T>;
        requires convertible_to<const T, T>;
    };

namespace detail {

namespace swap_adl_ns {

template <typename T>
void swap(T&, T&) = delete;

template <typename T, typename U>
concept adl_swap = requires(T t, U u) {
    static_cast<void>(swap(t, u));
};

} // namespace swap_adl_ns

template <typename T, typename U>
concept quick_can_swap = requires {
    requires alike<T, U>;
    requires neo_is_lvalue_reference(T);
    requires neo_is_lvalue_reference(U);
    requires assignable_from<T, U>;
    requires assignable_from<U, T>;
};

template <typename T, typename U, typename Tv, typename Uv>
concept can_swap_prog_defined_adl = requires {
    requires neo_is_program_defined(Tv) or neo_is_program_defined(Uv);
    requires neo_is_lvalue_reference(T);
    requires neo_is_lvalue_reference(U);
    requires swap_adl_ns::adl_swap<T, U>;
};

template <typename T, typename U, typename Tv, typename Uv>
concept can_shuffle_swap = requires {
    requires neo_is_lvalue_reference(T);
    requires neo_is_lvalue_reference(U);
    requires weak_same_as<Tv, Uv>;
    requires move_constructible<Tv>;
    requires assignable_from<Tv&, Tv>;
};

template <typename T,
          typename U,
          typename Tv = remove_cvref_t<T>,
          typename Uv = remove_cvref_t<U>>
constexpr bool can_std_ranges_swap = (
    // Check for quick swapp-ability:
       can_shuffle_swap<T, U, Tv, Uv>
    or can_swap_prog_defined_adl<T, U, Tv, Uv>
    // Otherwise, if it is two arrays of equal extent:
    or requires(T&& t, U&& u) {
        requires neo_is_bounded_array(Tv) and neo_is_bounded_array(Uv);
        requires neo_array_rank(Tv) == neo_array_rank(Uv);
        requires std::extent_v<Tv> == std::extent_v<Uv>;
        // Check that elements are swappable:
        requires  can_std_ranges_swap<decltype(t[0]), decltype(u[0])>;
    }
);

template <typename T, typename U>
concept swappable_impl =
       quick_can_swap<T, U>
    // Do a more thorough check:
    or requires {
        requires can_std_ranges_swap<T, T>;
        requires can_std_ranges_swap<T, U>;
        requires can_std_ranges_swap<U, U>;
        requires can_std_ranges_swap<U, T>;
    };

} // namespace detail

/**
 * @brief Test whether a type is std::ranges::swap()'able with some other type
 */
template <typename T, typename U>
concept swappable_with = detail::swappable_impl<T, U>;

/**
 * @brief Test whether a type is swappable with itself via std::ranges::swap
 */
template <typename T>
concept swappable = detail::can_std_ranges_swap<T&, T&>;

namespace detail {

template <typename T, typename U, bool Alike = alike<T, U>>
concept weakly_equality_comparable_with =
    (Alike and requires(const_reference_t<T> t) {
        { t == t } -> simple_boolean;
        { t != t } -> simple_boolean;
    })
    or (not Alike and requires (const_reference_t<T> t,
                                const_reference_t<U> u) {
        { t == u } -> simple_boolean;
        { t != u } -> simple_boolean;
        { u == t } -> simple_boolean;
        { u != t } -> simple_boolean;
    });

template <typename T, typename U>
concept partially_ordered_with =
    requires (const_reference_t<T> t,
              const_reference_t<U> u) {
        { t <  u } -> simple_boolean;
        { t >  u } -> simple_boolean;
        { t <= u } -> simple_boolean;
        { t >= u } -> simple_boolean;
        { u <  t } -> simple_boolean;
        { u >  t } -> simple_boolean;
        { u <= t } -> simple_boolean;
        { u >= t } -> simple_boolean;
    };

template <typename T, typename Cat>
concept compares_as = weak_same_as<std::common_comparison_category_t<T, Cat>, Cat>;

} // namespace detail

template <typename T>
concept equality_comparable = detail::weakly_equality_comparable_with<T, T>;

template <typename T, typename U>
concept equality_comparable_with =
        detail::weakly_equality_comparable_with<T, U>
    and equality_comparable<T>
    and equality_comparable<U>
    and common_reference_with<T, U>
    and equality_comparable<std::common_reference_t<T, U>>
    ;

template <typename T>
concept totally_ordered =
        detail::partially_ordered_with<T, T>
    and equality_comparable<T>;

template <typename T, typename U>
concept totally_ordered_with =
        detail::partially_ordered_with<T, U>
    and equality_comparable_with<T, U>
    and totally_ordered<T>
    and totally_ordered<U>
    and common_reference_with<T, U>
    and totally_ordered<
        std::common_reference_t<const_reference_t<T>,
                                const_reference_t<U>>>;

template <typename T, typename Cat = std::partial_ordering>
concept three_way_comparable =
        detail::weakly_equality_comparable_with<T, T>
    and detail::partially_ordered_with<T, T>
    and requires (const_reference_t<T> a, const_reference_t<T> b) {
        { a <=> b } -> detail::compares_as<Cat>;
    };

template <typename T, typename U, typename Cat = std::partial_ordering>
concept three_way_comparable_with =
        detail::weakly_equality_comparable_with<T, U>
    and detail::partially_ordered_with<T, U>
    and requires (const_reference_t<T> t, const_reference_t<U> u) {
        { t <=> u } -> detail::compares_as<Cat>;
        { u <=> t } -> detail::compares_as<Cat>;
    }
    and three_way_comparable<T, Cat>
    and three_way_comparable<U, Cat>
    and common_reference_with<const_reference_t<T>, const_reference_t<U>>
    and three_way_comparable<common_reference_t<const_reference_t<T>, const_reference_t<U>>>;

template <typename T>
concept movable =
        neo_is_object(T)
    and move_constructible<T>
    and assignable_from<T&, T>
    and swappable<T>
    ;

template<typename T>
concept copyable = true
    and copy_constructible<T>
    and movable<T>
    and assignable_from<T&, T&>
    and assignable_from<T&, const T&>
    and assignable_from<T&, const T>
    ;

template <typename T>
concept semiregular = copyable<T> and default_initializable<T>;

template <typename T>
concept regular = equality_comparable<T> and semiregular<T>;


template <typename F, typename... Args>
concept invocable = invocable2<F, Args...>;

template <typename F, typename... Args>
concept regular_invocable = invocable<F, Args...>;

template <typename F, typename... Args>
concept predicate =
        regular_invocable<F, Args...>
    and simple_boolean<invoke_result_t<F, Args...>>;

template <typename R, typename T, typename U>
concept relation =
        predicate<R, T, T>
    and predicate<R, T, U>
    and predicate<R, U, U>
    and predicate<R, U, T>
    ;

template <typename R, typename T, typename U>
concept equivalence_relation = relation<R, T, U>;

template <typename R, typename T, typename U>
concept strict_weak_order = relation<R, T, U>;

/*
######## ##     ## ######## ######## ##    ##  ######  ####  #######  ##    ##  ######
##        ##   ##     ##    ##       ###   ## ##    ##  ##  ##     ## ###   ## ##    ##
##         ## ##      ##    ##       ####  ## ##        ##  ##     ## ####  ## ##
######      ###       ##    ######   ## ## ##  ######   ##  ##     ## ## ## ##  ######
##         ## ##      ##    ##       ##  ####       ##  ##  ##     ## ##  ####       ##
##        ##   ##     ##    ##       ##   ### ##    ##  ##  ##     ## ##   ### ##    ##
######## ##     ##    ##    ######## ##    ##  ######  ####  #######  ##    ##  ######
*/

/// Equivalent to std::constructible_from, but checks that construction is noexcept
template <typename T, typename... Args>
concept nothrow_constructible_from =
    constructible_from<T, Args...> and
#if NEO_HAS_BUILTIN(__is_nothrow_constructible)
    // If this check fails, we've already established that it *is* constructible
    // from the given arguments, so the only issue is that the selected constructor
    // is not noexcept(true)
    __is_nothrow_constructible(T, Args&&...)
#else
    requires(Args&&... args) { { T(NEO_FWD(args)...) } noexcept; }
#endif
    ;

template <typename T, typename... Args>
concept trivially_constructible =
        constructible_from<T, Args...>
    and neo_is_trivially_constructible(T, Args...);

template <typename T>
concept trivially_default_constructible =
        default_initializable<T>
    and trivially_constructible<T>;

template <typename T>
concept trivially_destructible =
    destructible<T> and neo_is_trivially_destructible(T)
    ;

template <typename T>
concept trivially_copyable =
        copyable<T>
    and neo_is_trivially_copyayable(T)
    ;

template <typename To, typename From>
concept trivially_assignable =
        assignable_from<To, From>
    and neo_is_trivially_assignable(To, From);

template <typename T>
concept trivially_movable =
        movable<T>
    and trivially_constructible<T, T&&>
    and trivially_assignable<T&, T&&>;

template <typename T>
concept trivial_type = trivially_copyable<T> && neo_is_trivial(T);

template <typename Func, typename... Args>
concept nothrow_invocable =
    invocable<Func, Args...> &&
    requires(Func fn, Args... args) {
        { neo::invoke(NEO_FWD(fn), NEO_FWD(args)...) } noexcept;
    };

// clang-format on

}  // namespace neo
