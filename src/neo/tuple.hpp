#pragma once

#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./get.hpp"
#include "./meta.hpp"
#include "./type_traits.hpp"

#include <neo/core/tuple.hpp>

#include <compare>
#include <utility>

namespace neo {

// Forward-decl
template <typename...>
class tuple;

// Deduction guide: Decay all arguments
template <typename... Ts>
tuple(Ts&&... ts) -> tuple<decay_t<Ts>...>;

/**
 * @brief Obtain the return type of calling get_nth<N>() with the given tuple type
 *
 * @tparam Tuple A tuple type
 * @tparam N An index that is valid for Tuple
 */
template <typename Tuple, std::size_t N>
    requires can_get_nth<Tuple, N>
using tuple_get_t = get_nth_t<Tuple, N>;

template <typename... Ts>
class tuple : public core::tuple<Ts...> {
    using _base = core::tuple<Ts...>;

public:
    using index_sequence = typename _base::index_sequence;
    using _base::tuple;

    // clang-format off
    tuple(tuple&) = default;
    tuple(const tuple&) = default;
    tuple(tuple&&) = default;
    tuple& operator=(tuple&) = default;
    tuple& operator=(const tuple&) = default;
    tuple& operator=(tuple&&) = default;

    /// XXX: MSVC has trouble with concepts in the conditional-explicit. Use an intrinsic instead:
    /**
     * @brief Per-element copying constructor.
     *
     * Since the types of the arguments here are non-deduced, this constructor will cause
     * construction at call sites if the corresponding argument cannot be deduced (e.g. is a bare
     * braced initializer).
     */
    explicit(not (neo_is_convertible_to(nonvoid_t<Ts> const&, nonvoid_t<Ts>) and ...))
    constexpr tuple(nonvoid_t<Ts> const&... ts)
        noexcept((nothrow_constructible_from<nonvoid_t<Ts>, nonvoid_t<Ts> const&> and ...))
        requires (sizeof...(Ts) != 0)
             and (constructible_from<nonvoid_t<Ts>, nonvoid_t<Ts> const&> and ... )
        : _base(tuple_construct_tag{}, ts...)
        {}

    /**
     * @brief Per-element converting constructor. Each argument given here is
     * passed through to the corresponding element of the tuple.
     *
     * Each argument must be explicit-convertible to the corresponding underlying element type. This
     * constructor is `explicit` if any sub-element requires an explicit conversion from
     * the corresponding argument
     */
    template <explicit_convertible_to<nonvoid_t<Ts>>... Args>
        // Don't let this be visible when Ts is empty. That confuses some compilers
        requires (sizeof...(Ts) > 0)
    // We are an explicit constructor if any sub-element requires an explicit conversion
    explicit(not (neo_is_convertible_to(Args&&, nonvoid_t<Ts>) and ...))
    constexpr tuple(Args&&... args)
        // We are no-throw if all element conversions are also no-throw
        noexcept((noexcept(static_cast<nonvoid_t<Ts>>(NEO_FWD(args))) and ...))
        // Delegate to the base constructor:
        : _base(tuple_construct_tag{}, NEO_FWD(args)...)
        {}

    /**
     * @brief Tuple conversion constructor. Allows constructing a tuple from another
     * tuple type that may have different elements, provided that there exists valid
     * conversions between those elements.
     *
     * This constructor requires that the argument be a tuple-like type with the same
     * number of elements, and that each element from the other tuple is explicit-convertible-to
     * the corresponding element in this tuple. This constructor is explicit if any of
     * the sub-element conversions requires an explicit conversion.
     *
     * The tuple is perfect-forwarded through during construction.
     */
    template <unalike<tuple> OtherTpl>
        requires core::_tuple_detail::tuple_constructible<tuple, OtherTpl>
    explicit(not core::_tuple_detail::tuple_convertible<OtherTpl, tuple>)
    constexpr tuple(OtherTpl&& other)
        noexcept(core::_tuple_detail::tuple_construct_nothrow<tuple, OtherTpl>)
        : _base(tuple_convert_tag{}, NEO_FWD(other), index_sequence{})
        {}

    // clang-format on

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;

    template <typename... Us>
        requires(equality_comparable_with<object_t<Us>, object_t<Ts>> and ...)
    constexpr bool operator==(tuple<Us...> const& rhs) const noexcept {
        return core::_tuple_detail::equal(*this, rhs, index_sequence{});
    }

    template <typename... Us>
        requires(totally_ordered_with<object_t<Ts>, object_t<Us>> and ...)
    constexpr typename std::common_comparison_category<
        typename std::compare_three_way_result<object_t<Ts>, object_t<Us>>::type...>::type
    operator<=>(const tuple<Us...>& other) const noexcept {
        return core::_tuple_detail::compare_three_way(*this, other, index_sequence{});
    }
};

template <typename Tuple>
using index_sequence_t = typename remove_reference_t<Tuple>::index_sequence;

namespace _tup_detail {

template <typename... Tuples>
struct concat {
    using result_type = meta::append<remove_cvref_t<Tuples>...>;

    template <typename... Indices>
    struct inner;

    template <auto... Ns>
    struct inner<std::index_sequence<Ns...>> {
        template <typename F>
        static constexpr result_type go(F&& fin, auto&&... elems) {
            return result_type(NEO_FWD(elems)..., NEO_FWD(fin).template get<Ns>()...);
        }
    };

    template <auto... Nt, auto... Nu>
    struct inner<std::index_sequence<Nt...>, std::index_sequence<Nu...>> {
        template <typename T, typename U>
        static constexpr result_type go(T&& t, U&& u, auto&&... elems) {
            return result_type(NEO_FWD(elems)...,
                               NEO_FWD(t).template get<Nt>()...,
                               NEO_FWD(u).template get<Nu>()...);
        }
    };

    template <auto... Nt, auto... Nu, auto... Nv>
    struct inner<std::index_sequence<Nt...>,
                 std::index_sequence<Nu...>,
                 std::index_sequence<Nv...>> {
        template <typename T, typename U, typename V>
        static constexpr result_type go(T&& t, U&& u, V&& v, auto&&... elems) {
            return result_type(NEO_FWD(elems)...,
                               NEO_FWD(t).template get<Nt>()...,
                               NEO_FWD(u).template get<Nu>()...,
                               NEO_FWD(v).template get<Nv>()...);
        }
    };

    template <auto... Nt, auto... Nu, auto... Nv, auto... Nw>
    struct inner<std::index_sequence<Nt...>,
                 std::index_sequence<Nu...>,
                 std::index_sequence<Nv...>,
                 std::index_sequence<Nw...>> {
        template <typename T, typename U, typename V, typename W>
        static constexpr result_type go(T&& t, U&& u, V&& v, W&& w, auto&&... elems) {
            return result_type(NEO_FWD(elems)...,
                               NEO_FWD(t).template get<Nt>()...,
                               NEO_FWD(u).template get<Nu>()...,
                               NEO_FWD(v).template get<Nv>()...,
                               NEO_FWD(w).template get<Nw>()...);
        }
    };

    template <auto... Nt, auto... Nu, auto... Nv, auto... Nw, auto... Nx>
    struct inner<std::index_sequence<Nt...>,
                 std::index_sequence<Nu...>,
                 std::index_sequence<Nv...>,
                 std::index_sequence<Nw...>,
                 std::index_sequence<Nx...>> {
        template <typename T, typename U, typename V, typename W, typename X>
        static constexpr result_type go(T&& t, U&& u, V&& v, W&& w, X&& x, auto&&... elems) {
            return result_type(NEO_FWD(elems)...,
                               NEO_FWD(t).template get<Nt>()...,
                               NEO_FWD(u).template get<Nu>()...,
                               NEO_FWD(v).template get<Nv>()...,
                               NEO_FWD(w).template get<Nw>()...,
                               NEO_FWD(x).template get<Nx>()...);
        }
    };

    template <auto... Nt, auto... Nu, auto... Nv, auto... Nw, auto... Nx, typename... Seq>
    struct inner<std::index_sequence<Nt...>,
                 std::index_sequence<Nu...>,
                 std::index_sequence<Nv...>,
                 std::index_sequence<Nw...>,
                 std::index_sequence<Nx...>,
                 Seq...> {
        template <typename T, typename U, typename V, typename W, typename X, typename... More>
        static constexpr result_type
        go(T&& t, U&& u, V&& v, W&& w, X&& x, More&&... more, auto&&... elems) {
            return inner<Seq...>::template go<More...>(NEO_FWD(more)...,
                                                       NEO_FWD(elems)...,
                                                       NEO_FWD(t).template get<Nt>()...,
                                                       NEO_FWD(u).template get<Nu>()...,
                                                       NEO_FWD(v).template get<Nv>()...,
                                                       NEO_FWD(w).template get<Nw>()...,
                                                       NEO_FWD(x).template get<Nx>()...);
        }
    };
};

}  // namespace _tup_detail

// clang-format off
/**
 * @brief Construct a new tuple that is a concatenation of every given tuple.
 */
template <typename... Tups>
constexpr auto tuple_cat(Tups&&... tups)
    noexcept((noexcept(remove_cvref_t<Tups>(NEO_FWD(tups))) and ...))
    requires requires {
        (static_cast<remove_cvref_t<Tups>>(NEO_FWD(tups)), ...);
    }
{
    // clang-format on
    using catter = _tup_detail::concat<Tups...>;
    return catter::template inner<index_sequence_t<Tups>...>::template go<Tups...>(
        NEO_FWD(tups)...);
}

}  // namespace neo

template <typename... Ts>
struct std::tuple_size<neo::tuple<Ts...>> {
    constexpr static inline std::size_t value = sizeof...(Ts);
};

template <std::size_t N, typename... Ts>
struct std::tuple_element<N, neo::tuple<Ts...>>
    : neo::meta::tag<neo::nonvoid_t<neo::meta::pack_at<N, Ts...>>> {};

template <typename... Ts, typename... Us>
    requires(sizeof...(Ts) == sizeof...(Us)) and (neo::has_common_type<Ts, Us> and ...)
struct neo::basic_common_type<neo::tuple<Ts...>, neo::tuple<Us...>> {
    using type = neo::tuple<neo::common_type_t<Ts, Us>...>;
};

template <typename... Ts, typename... Us, neo_tt1param Tq, neo_tt1param Uq>
    requires(sizeof...(Ts) == sizeof...(Us)) and (neo::has_common_reference<Ts, Us> and ...)
struct std::basic_common_reference<neo::tuple<Ts...>, neo::tuple<Us...>, Tq, Uq> {
    using type = neo::tuple<neo::common_reference_t<Tq<Ts>, Uq<Us>>...>;
};
