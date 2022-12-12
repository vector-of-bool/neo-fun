#pragma once

#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./declval.hpp"
#include "./fwd.hpp"
#include "./pick_type.hpp"
#include "./tag.hpp"

#include <compare>
#include <concepts>
#include <memory>
#include <utility>

namespace neo {

// Forward-decl
template <typename...>
class tuple;

// Deduction guide: Decay all arguments
template <typename... Ts>
tuple(const Ts&... ts) -> tuple<Ts...>;

namespace _tup_detail {

template <auto>
void get();

template <typename T, std::size_t N>
concept has_member_get_n = requires(T&& v) {
    NEO_FWD(v).template get<N>();
};

template <typename T, std::size_t N>
concept has_adl_get_n = requires(T&& t) {
    get<N>(NEO_FWD(t));
};

}  // namespace _tup_detail

template <typename Tpl, std::size_t Index>
concept can_tuple_get_nth = requires {
    requires std::tuple_size_v<std::remove_cvref_t<Tpl>> > Index;
    requires _tup_detail::has_adl_get_n<Tpl, Index> or _tup_detail::has_member_get_n<Tpl, Index>;
};

/**
 * @brief Invocable object that attempts to access the Nth element of the given tuple
 *
 * @tparam N
 */
template <std::size_t N>
struct tuple_get_fn {
    template <can_tuple_get_nth<N> Tuple>
    constexpr decltype(auto) operator()(Tuple&& tup) const noexcept {
        if constexpr (_tup_detail::has_member_get_n<Tuple, N>) {
            return NEO_FWD(tup).template get<N>();
        } else {
            using _tup_detail::get;
            return get<N>(NEO_FWD(tup));
        }
    }
};

template <std::size_t N>
constexpr inline auto tuple_get = tuple_get_fn<N>{};

/**
 * @brief Obtain the return type of calling tuple_get<N>() with the given tuple type
 *
 * @tparam Tuple A tuple type
 * @tparam N An index that is valid for Tuple
 */
template <typename Tuple, std::size_t N>
requires can_tuple_get_nth<Tuple, N>
using tuple_get_t = decltype(tuple_get<N>(NEO_DECLVAL(Tuple)));

namespace _tup_detail {

/// Holds the actual tuple data element
template <typename T>
struct box {
    // The value for this element
    NEO_NO_UNIQUE_ADDRESS T item;

    constexpr box() = default;
    constexpr explicit box(T&& t) noexcept(nothrow_constructible_from<T, T&&>)
        : item(NEO_FWD(t)) {}
};

/// An indexed box to hold tuple elements
template <std::size_t, typename T>
struct nth_box : box<T> {
    // Just inherit from box, which has the correct constructors
    using box<T>::box;
};

template <typename Seq, typename Tpl>
struct boxes;

template <std::size_t... Is, typename... Ts>
struct boxes<std::index_sequence<Is...>, tuple<Ts...>> : nth_box<Is, Ts>... {
    constexpr boxes() = default;

    constexpr explicit boxes(Ts&&... args)                        //
        noexcept((nothrow_constructible_from<Ts, Ts&&> and ...))  //
        requires(sizeof...(args) > 0)
        : nth_box<Is, Ts>(NEO_FWD(args))... {}
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

template <>
class tuple<> {
public:
    using index_sequence = std::index_sequence<>;

    constexpr auto operator<=>(const tuple&) const noexcept { return std::strong_ordering::equal; }
    constexpr bool operator==(const tuple&) const noexcept { return true; }
};

template <typename... Ts>
class tuple {
public:
    using index_sequence = std::make_index_sequence<sizeof...(Ts)>;

private:
    using container = _tup_detail::boxes<index_sequence, tuple>;

    template <typename...>
    friend class tuple;

    // The actual closure: Our values live here:
    NEO_NO_UNIQUE_ADDRESS container _values;

    struct convert_each_tag {};

    // clang-format off
    template <explicit_convertible_to<Ts>... Args>
    constexpr explicit tuple(convert_each_tag, Args&&... args)
        noexcept(nothrow_constructible_from<container, Ts...>)
        : _values(static_cast<Ts>(NEO_FWD(args))...) {}

    struct conversion_tag {};

    template <typename Other, std::size_t... Ns>
    constexpr explicit tuple(conversion_tag, Other&& other, std::index_sequence<Ns...>)
        noexcept(noexcept(container(static_cast<Ts>(NEO_FWD(other).template get<Ns>())...)))
        requires requires { container(static_cast<Ts>(NEO_FWD(other).template get<Ns>())...); }
        : tuple(convert_each_tag{}, NEO_FWD(other).template get<Ns>()...)
        {}

    template <std::size_t N>
    using _nth_box = _tup_detail::nth_box<N, typename pick_type<N>::template from<Ts...>::type>;

public:
    constexpr tuple() = default;

    constexpr tuple(tuple&) = default;
    constexpr tuple(const tuple&) = default;
    constexpr tuple(tuple&&) = default;
    constexpr tuple& operator=(tuple&) = default;
    constexpr tuple& operator=(const tuple&) = default;
    constexpr tuple& operator=(tuple&&) = default;

    explicit(not (convertible_to2<const Ts&, Ts> and ...))
    constexpr tuple(const Ts&... ts)
        noexcept(nothrow_constructible_from<container, Ts...>)
        requires (sizeof...(Ts) != 0)
             and constructible_from2<container, const Ts&...>
        : tuple(convert_each_tag{}, ts...)
        {}

    template <explicit_convertible_to<Ts>... Args>
        requires (sizeof...(Ts) > 0)
    // Explicit if any of Args is not convertible to a corresponding element:
    explicit(not (convertible_to2<Args, Ts> and ...))
    constexpr tuple(Args&&... args)
        noexcept(nothrow_constructible_from<container, Ts...> and
                 (nothrow_constructible_from<Ts, Args> and ...))
        : tuple(convert_each_tag{}, NEO_FWD(args)...)
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, const Us&> and ...)
    explicit (not (convertible_to2<const Us&, Ts> and ...))
    constexpr tuple(const tuple<Us...>& other)
        noexcept(nothrow_constructible_from<container, Ts...>
                 and (nothrow_constructible_from<Ts, const Us&> and ...))
        : tuple(conversion_tag{}, other, index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, Us&> and ...)
    explicit (not (convertible_to2<const Us&, Ts> and ...))
    constexpr tuple(tuple<Us...>& other)
        noexcept(nothrow_constructible_from<container, Ts...>
                 and (nothrow_constructible_from<Ts, Us&> and ...))
        : tuple(conversion_tag{}, other, index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, Us&&> and ...)
    explicit (not (convertible_to2<Us&&, Ts> and ...))
    constexpr tuple(tuple<Us...>&& other)
        noexcept(nothrow_constructible_from<container, Ts...>
                 and (nothrow_constructible_from<Ts, Us&&> and ...))
        : tuple(conversion_tag{}, NEO_FWD(other), index_sequence{})
        {}

    template <typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts))
             and (constructible_from<Ts, const Us&&> and ...)
    explicit (not (convertible_to2<const Us&&, Ts> and ...))
    constexpr tuple(const tuple<Us...>&& other)
        noexcept(nothrow_constructible_from<container, Ts...>
                 and (nothrow_constructible_from<Ts, const Us&&> and ...))
        : tuple(conversion_tag{}, NEO_FWD(other), index_sequence{})
        {}

    // clang-format on
    template <std::size_t N>
    constexpr decltype(auto) get() & noexcept {
        auto& box = static_cast<_nth_box<N>&>(_values);
        return static_cast<decltype(box.item)&>(box.item);
    }

    template <std::size_t N>
    constexpr decltype(auto) get() const& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_values);
        return static_cast<decltype(box.item) const&>(box.item);
    }

    template <std::size_t N>
    constexpr decltype(auto) get() && noexcept {
        auto& box = static_cast<_nth_box<N>&>(_values);
        return static_cast<decltype(box.item)&&>(box.item);
    }

    template <std::size_t N>
    constexpr decltype(auto) get() const&& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_values);
        return static_cast<decltype(box.item) const&&>(box.item);
    }

    template <std::equality_comparable_with<Ts>... Us>
    constexpr bool operator==(tuple<Us...> const& rhs) const noexcept {
        return _tup_detail::equal(*this, rhs, index_sequence{});
    }

    template <std::totally_ordered_with<Ts>... Us>
    constexpr typename std::common_comparison_category<
        typename std::compare_three_way_result<Ts, Us>::type...>::type
    operator<=>(const tuple<Us...>& other) const noexcept {
        return _tup_detail::compare_three_way(*this, other, index_sequence{});
    }
};

template <typename Tuple>
using index_sequence_t = typename std::remove_reference<Tuple>::type::index_sequence;

namespace _tup_detail {

template <typename... Tuples>
struct concat {

    using result_type = typename tag_concat<typename std::remove_cvref<Tuples>::type...>::type;

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
    noexcept((noexcept(std::remove_cvref_t<Tups>(NEO_FWD(tups))) and ...))
    requires requires {
        (static_cast<std::remove_cvref_t<Tups>>(NEO_FWD(tups)), ...);
    }
{
    // clang-format on
    return _tup_detail::concat<Tups...>::template inner<index_sequence_t<Tups>...>::template go<
        Tups...>(NEO_FWD(tups)...);
}

}  // namespace neo

template <typename... Ts>
struct std::tuple_size<neo::tuple<Ts...>> {
    constexpr static inline std::size_t value = sizeof...(Ts);
};

template <std::size_t N, typename... Ts>
struct std::tuple_element<N, neo::tuple<Ts...>> : neo::pick_type<N>::template from<Ts...> {};

template <typename... Ts, typename Alloc>
struct std::uses_allocator<neo::tuple<Ts...>, Alloc> : std::true_type {};
