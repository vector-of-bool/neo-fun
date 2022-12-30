#pragma once

#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./declval.hpp"
#include "./fwd.hpp"
#include "./meta.hpp"
#include "./pick_type.hpp"
#include "./tag.hpp"

#include <compare>
#include <concepts>
#include <utility>

namespace neo {

// Forward-decl
template <typename...>
class tuple;

// Deduction guide: Decay all arguments
template <typename... Ts>
tuple(Ts&&... ts) -> tuple<decay_t<Ts>...>;

struct tuple_construct_tag {};
struct tuple_convert_tag {};

namespace _tup_detail {

template <auto>
void get() = delete;

template <typename T, std::size_t N>
concept has_member_get_n = requires(T&& v) { NEO_FWD(v).template get<N>(); };

template <typename T, std::size_t N>
concept has_adl_get_n = requires(T&& t) { get<N>(NEO_FWD(t)); };

}  // namespace _tup_detail

template <typename Tpl, std::size_t Index>
concept can_get_nth = requires {
                          requires meta::len_v<remove_cvref_t<Tpl>> > Index;
                          requires _tup_detail::has_adl_get_n<Tpl, Index>
                              or _tup_detail::has_member_get_n<Tpl, Index>;
                      };

/**
 * @brief Invocable object template that attempts to access the Nth element of the given argument
 *
 * @tparam N The index to access.
 *
 * The object must have a member function template get<N>(), or an ADL-visitle
 * get<N>(t);
 *
 * This works for std::tuple, std::variant, neo::tuple, nad neo::nano_tuple
 */
template <std::size_t N>
struct get_nth_fn {
    template <can_get_nth<N> T>
    constexpr decltype(auto) operator()(T&& t) const noexcept {
        if constexpr (_tup_detail::has_member_get_n<T, N>) {
            return NEO_FWD(t).template get<N>();
        } else {
            using _tup_detail::get;
            return get<N>(NEO_FWD(t));
        }
    }
};

template <std::size_t N>
constexpr inline auto get_nth = get_nth_fn<N>{};

/**
 * @brief Obtain the return type of calling get_nth<N>() with the given tuple type
 *
 * @tparam Tuple A tuple type
 * @tparam N An index that is valid for Tuple
 */
template <typename Tuple, std::size_t N>
    requires can_get_nth<Tuple, N>
using tuple_get_t = decltype(get_nth<N>(NEO_DECLVAL(Tuple)));

namespace _tup_detail {

/// An indexed box to hold tuple elements
template <std::size_t, typename T>
struct nth_box {
    // The value for this element
    NEO_NO_UNIQUE_ADDRESS T item;

    constexpr nth_box() = default;
    constexpr explicit nth_box(tuple_construct_tag, auto&& t)
        : item(NEO_FWD(t)) {}

    bool operator==(nth_box const&) const  = default;
    auto operator<=>(nth_box const&) const = default;
};

template <typename Seq, typename Tpl>
struct boxes;

template <std::size_t... Is, neo_ttparam Tpl, typename... Ts>
struct boxes<std::index_sequence<Is...>, Tpl<Ts...>> : nth_box<Is, Ts>... {
    constexpr boxes() = default;

    template <typename... Args>
    constexpr explicit boxes(tuple_construct_tag tag, Args&&... args)  //
        : nth_box<Is, Ts>(tag, NEO_FWD(args))... {}

    bool operator==(boxes const&) const  = default;
    auto operator<=>(boxes const&) const = default;
};

template <std::size_t... Ns>
constexpr bool equal(const auto& left, const auto& right, std::index_sequence<Ns...>) noexcept {
    return (static_cast<bool>(left.template get<Ns>() == right.template get<Ns>()) and ...);
}

constexpr auto compare_three_way(const auto&, const auto&, std::index_sequence<>) noexcept {
    return std::strong_ordering::equal;
}

template <std::size_t I, std::size_t... Ns>
constexpr std::strong_ordering
compare_three_way(const auto& lhs, const auto& rhs, std::index_sequence<I, Ns...>) noexcept {
    const auto result
        = std::compare_strong_order_fallback(lhs.template get<I>(), rhs.template get<I>());
    if (not std::is_eq(result)) {
        return result;
    }
    auto next = _tup_detail::compare_three_way(lhs, rhs, std::index_sequence<Ns...>{});
    return next;
}

template <std::size_t I, std::size_t J, std::size_t K, std::size_t... Ns>
constexpr std::strong_ordering
compare_three_way(const auto& lhs, const auto& rhs, std::index_sequence<I, J, K, Ns...>) noexcept {
    const auto i = std::compare_strong_order_fallback(lhs.template get<I>(), rhs.template get<I>());
    if (i != 0) {
        return i;
    }
    const auto j = std::compare_strong_order_fallback(lhs.template get<J>(), rhs.template get<J>());
    if (j != 0) {
        return j;
    }
    const auto k = std::compare_strong_order_fallback(lhs.template get<K>(), rhs.template get<K>());
    if (k != 0) {
        return k;
    }
    auto next = _tup_detail::compare_three_way(lhs, rhs, std::index_sequence<Ns...>{});
    return next;
}

}  // namespace _tup_detail

template <typename... Ts>
class nano_tuple {
public:
    using index_sequence = std::make_index_sequence<sizeof...(Ts)>;

private:
    using boxes = _tup_detail::boxes<index_sequence, nano_tuple>;

    NEO_NO_UNIQUE_ADDRESS boxes _boxes;

    template <std::size_t N>
    using _nth_box = _tup_detail::nth_box<N, meta::at<nano_tuple, N>>;

    template <std::size_t N>
    using _nth_type = meta::at<nano_tuple, N>;

public:
    nano_tuple() = default;

    template <typename... Args>
    constexpr nano_tuple(tuple_construct_tag,
                         Args&&... args)  //
        noexcept((nothrow_constructible_from<Ts, Args> and ...))
        : _boxes(tuple_construct_tag{}, static_cast<Ts>(NEO_FWD(args))...) {}

    template <typename Tpl, auto... N>
    constexpr nano_tuple(tuple_convert_tag,
                         Tpl&& tpl,
                         std::index_sequence<N...>)  //
        noexcept((nothrow_constructible_from<Ts, tuple_get_t<Tpl, N>> and ...))
        : _boxes(tuple_construct_tag{}, static_cast<Ts>(NEO_FWD(tpl).template get<N>())...) {}

    template <std::size_t N>
    constexpr _nth_type<N>& get() & noexcept {
        auto& box = static_cast<_nth_box<N>&>(_boxes);
        return static_cast<_nth_type<N>&>(box.item);
    }

    template <std::size_t N>
    constexpr _nth_type<N> const& get() const& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_boxes);
        return static_cast<_nth_type<N> const&>(box.item);
    }

    template <std::size_t N>
    constexpr _nth_type<N>&& get() && noexcept {
        auto& box = static_cast<_nth_box<N>&>(_boxes);
        return static_cast<_nth_type<N>&&>(box.item);
    }

    template <std::size_t N>
    constexpr _nth_type<N> const&& get() const&& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_boxes);
        return static_cast<_nth_type<N> const&&>(box.item);
    }

    bool operator==(nano_tuple const&) const
        requires(equality_comparable<Ts> and ...)
    = default;
    auto operator<=>(nano_tuple const&) const
        requires(totally_ordered<Ts> and ...)
    = default;
};

template <typename... Ts>
class tuple : public nano_tuple<Ts...> {
    using _base = nano_tuple<Ts...>;

public:
    using index_sequence = typename _base::index_sequence;
    using _base::get;
    using _base::nano_tuple;

    // clang-format off
    tuple(tuple&) = default;
    tuple(const tuple&) = default;
    tuple(tuple&&) = default;
    tuple& operator=(tuple&) = default;
    tuple& operator=(const tuple&) = default;
    tuple& operator=(tuple&&) = default;

    /// XXX: MSVC has trouble with concepts in the conditional-explicit. Use an intrinsic instead:
    explicit(not (neo_is_convertible_to(const Ts&, Ts) and ...))
    constexpr tuple(const Ts&... ts)
        noexcept((nothrow_constructible_from<Ts, const Ts&> and ...))
        requires (sizeof...(Ts) != 0)
             and (constructible_from<Ts, const Ts&> and ... )
        : _base(tuple_construct_tag{}, ts...)
        {}

    template <explicit_convertible_to<Ts>... Args>
        requires (sizeof...(Ts) > 0)
    explicit(not (neo_is_convertible_to(Args&&, Ts) and ...))
    constexpr tuple(Args&&... args)
        noexcept((nothrow_constructible_from<Ts, Args> and ...))
        : _base(tuple_construct_tag{}, static_cast<Ts>(NEO_FWD(args))...)
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, const Us&> and ...)
    explicit(not (neo_is_convertible_to(const Us&, Ts) and ...))
    constexpr tuple(const tuple<Us...>& other)
        noexcept((nothrow_constructible_from<Ts, const Us&> and ...))
        : _base(tuple_convert_tag{}, other, index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, Us&> and ...)
    explicit(not (neo_is_convertible_to(Us&, Ts) and ...))
    constexpr tuple(tuple<Us...>& other)
        noexcept((nothrow_constructible_from<Ts, Us&> and ...))
        : _base(tuple_convert_tag{}, other, index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, Us&&> and ...)
    explicit(not (neo_is_convertible_to(Us&&, Ts) and ...))
    constexpr tuple(tuple<Us...>&& other)
        noexcept((nothrow_constructible_from<Ts, Us&&> and ...))
        : _base(tuple_convert_tag{}, NEO_FWD(other), index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, const Us&&> and ...)
    explicit(not (neo_is_convertible_to(const Us&&, Ts) and ...))
    constexpr tuple(const tuple<Us...>&& other)
        noexcept((nothrow_constructible_from<Ts, const Us&&> and ...))
        : _base(tuple_convert_tag{}, NEO_FWD(other), index_sequence{})
        {}
    // clang-format on

    bool operator==(tuple const&) const
        requires(equality_comparable<Ts> and ...)
    = default;
    auto operator<=>(tuple const&) const
        requires(totally_ordered<Ts> and ...)
    = default;

    template <equality_comparable_with<Ts>... Us>
    constexpr bool operator==(tuple<Us...> const& rhs) const noexcept {
        return _tup_detail::equal(*this, rhs, index_sequence{});
    }

    template <totally_ordered_with<Ts>... Us>
    constexpr typename std::common_comparison_category<
        typename std::compare_three_way_result<Ts, Us>::type...>::type
    operator<=>(const tuple<Us...>& other) const noexcept {
        return _tup_detail::compare_three_way(*this, other, index_sequence{});
    }
};

template <>
class tuple<> : nano_tuple<> {
    using _base = nano_tuple<>;

public:
    using index_sequence = typename _base::index_sequence;
    using _base::get;
    using _base::nano_tuple;

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;
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
    : neo::meta::tag<neo::meta::at<neo::tuple<Ts...>, N>> {};

template <typename... Ts>
struct std::tuple_size<neo::nano_tuple<Ts...>> {
    constexpr static inline std::size_t value = sizeof...(Ts);
};

template <std::size_t N, typename... Ts>
struct std::tuple_element<N, neo::nano_tuple<Ts...>>
    : neo::meta::tag<neo::meta::at<neo::nano_tuple<Ts...>, N>> {};

// clang-format off
template <typename... Ts, typename... Us>
requires (sizeof...(Ts) == sizeof...(Us))
     and (neo::has_common_type<Ts, Us> and ...)
struct neo::basic_common_type<neo::tuple<Ts...>, neo::tuple<Us...>> {
    using type = neo::tuple<neo::common_type_t<Ts, Us>...>;
};

template <typename... Ts, typename... Us, neo_tt1param Tq, neo_tt1param Uq>
requires (sizeof...(Ts) == sizeof...(Us))
     and (neo::has_common_reference<Ts, Us> and ...)
struct std::basic_common_reference<neo::tuple<Ts...>, neo::tuple<Us...>, Tq, Uq> {
    using type = neo::tuple<neo::common_reference_t<Tq<Ts>, Uq<Us>>...>;
};

// clang-format on
