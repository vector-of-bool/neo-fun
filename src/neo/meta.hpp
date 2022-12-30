#pragma once

#include <utility>

#define u64 unsigned long long
#define tn typename
#define tl template

/// Shorthand for a type-list-template-template parameter
#define neo_ttparam                                                                                \
    template <class...>                                                                            \
    class
/// Shorthand for a template-template parameter that accepts one type argument
#define neo_tt1param                                                                               \
    template <class>                                                                               \
    class
/// Shorthand for a value-list-template-template parameter
#define neo_vltparam                                                                               \
    template <auto...>                                                                             \
    class
/// Shorthand for a constant-sequence-template-template parameter
#define neo_cseqtparam                                                                             \
    template <tn C, C...>                                                                          \
    class

namespace neo::meta {

struct id {
    template <tn T>
    using f = T;
};

namespace tacit {

template <tn... Fns>
struct pipe;

template <>
struct pipe<> {
    template <tn T>
    using f = T;
};

template <tn F, tn... More>
struct pipe<F, More...> {
    template <tn T>
    using f = tn pipe<More...>::tl f<tn F::tl f<T>>;
};

}  // namespace tacit

/// A complete empty type that is qualified by an arbitrary type
template <tn T>
struct tag {
    using type = T;

    constexpr bool operator==(tag) const noexcept { return true; }
    template <tn U>
    constexpr bool operator==(tag<U>) const noexcept {
        return false;
    }
};

template <tn T>
constexpr auto tag_v = tag<T>{};

/**
 * @brief An incomplete type that is parameterized by an arbitrary list of types
 */
template <tn... Ts>
struct list;

template <tn... Ts>
constexpr auto list_tag_v = tag_v<list<Ts...>>;

template <tn... Ts>
using list_tag = tag<list<Ts...>>;

/// Look up the nested '::type' on the given type
template <typename T>
using type_t = typename T::type;

/// Look up the nested '::value' on the given type
template <typename T>
constexpr decltype(auto) value_v = T::value;

template <auto V>
struct val {
    constexpr static auto value = V;
};

template <auto Const>
struct C;

namespace detail {

template <neo_ttparam Mapping>
struct map_ {
    template <tn L>
    struct on;

    template <neo_ttparam L, tn... Ts>
    struct on<L<Ts...>> {
        using type = L<Mapping<Ts>...>;
    };

    template <tn L>
    using f = tn on<L>::type;
};

}  // namespace detail

/**
 * @brief Apply the given type-template to each element of the given list
 *
 * @tparam Mapping A type-template that accepts one type and produces another type
 * @tparam L A template specialized on one or more types (e.g. meta::list<>, tuple<>, variant<>)
 */
template <neo_ttparam Mapping, tn L>
using map = tn detail::map_<Mapping>::tl f<L>;

namespace detail {

template <u64 N, typename Seq = std::make_index_sequence<N>>
struct filled_list_;

template <typename T, u64>
using filled_list_el = T;

template <u64 N, auto... Ns>
struct filled_list_<N, std::index_sequence<Ns...>> {
    template <tn T, neo_ttparam L>
    using with = L<filled_list_el<T, Ns - Ns>...>;
};

}  // namespace detail

/**
 * @brief Specialize a list-like template with N copies of the type T
 *
 * @tparam T The type to fill
 * @tparam N The number of T arguments to pass
 * @tparam L The template that will be specialized (default: meta::list)
 */
template <tn T, u64 N, neo_ttparam L = list>
using filled_list = tn detail::filled_list_<N>::tl with<T, L>;

namespace tacit {

template <tn L>
constexpr auto len_v = nullptr;

template <neo_ttparam L, tn... Ts>
constexpr std::size_t len_v<L<Ts...>> = sizeof...(Ts);

template <neo_ttparam L, tn... Ts>
constexpr std::size_t len_v<const L<Ts...>> = sizeof...(Ts);

template <neo_vltparam L, auto... Vs>
constexpr std::size_t len_v<L<Vs...>> = sizeof...(Vs);

template <neo_vltparam L, auto... Vs>
constexpr std::size_t len_v<const L<Vs...>> = sizeof...(Vs);

struct len {
    template <tn L>
    using f = meta::val<len_v<L>>;
};

}  // namespace tacit

/**
 * Get the length of in a list-like template specialization.
 *
 * Exposes a nested member ::valuee specifying the list of the list-list type
 */
template <tn L>
using len = tacit::len::f<L>;

template <tn L>
constexpr std::size_t len_v = tacit::len_v<L>;

namespace tacit {

struct clear {
    template <tn L>
    struct impl;

    template <neo_ttparam L, tn... Ts>
    struct impl<L<Ts...>> {
        using type = L<>;
    };

    template <tn L>
    using f = tn impl<L>::type;
};

}  // namespace tacit

template <tn L>
using clear = tacit::clear::f<L>;

template <tn L>
struct head_tail;

template <neo_ttparam L, tn H, tn... Ts>
struct head_tail<L<H, Ts...>> {
    using head = H;
    using tail = L<Ts...>;
};

template <neo_ttparam L, tn H, tn S, tn... Ts>
struct head_tail<L<H, S, Ts...>> {
    using head   = H;
    using second = S;
    using tail   = L<S, Ts...>;
};

/**
 * @brief Obtain the first type in the given typelist
 */
template <tn L>
using head = tn head_tail<L>::head;

/**
 * @brief Obtain the remainder of a typelist following the head element
 */
template <tn L>
using tail = tn head_tail<L>::tail;

/// Alias of 'head': Returns the first element in a typelist
template <tn L>
using first = head<L>;

/**
 * @brief Obtain the second type in the given typelist
 */
template <tn L>
using second = tn head_tail<L>::second;

namespace tacit {

struct head {
    template <tn L>
    using f = tn meta::head<L>;
};

struct tail {
    template <tn L>
    using f = tn meta::tail<L>;
};

struct second {
    template <tn L>
    using f = tn meta::second<L>;
};

}  // namespace tacit

/**
 * @brief Predicate on whether the given list is empty
 */
template <tn L>
using is_empty = val<(len_v<L> == 0)>;

/// Predicate variable on whether a list is empty
template <tn L>
constexpr auto is_empty_v = is_empty<L>::value;

namespace detail {

constexpr u64 sa_log2fn(u64 n) {
    u64 r = 0;
    for (; n > 1; n >>= 1) {
        ++r;
    }
    return r;
}

template <u64 N>
struct sa_log2 {
    constexpr static u64 value = sa_log2fn(N);
};

template <u64 N, tn Acc, tn More, u64 Log2 = sa_log2<N>::value>
struct split_at_;

// Final:
template <neo_ttparam L, tn... Acc, tn... Ts>
struct split_at_<0, L<Acc...>, L<Ts...>, 0> {
    using type = L<L<Acc...>, L<Ts...>>;
};

// One more:
template <neo_ttparam L, tn... Acc, tn Head, tn... Tail>
struct split_at_<1, L<Acc...>, L<Head, Tail...>, 0> {
    using type = L<L<Acc..., Head>, L<Tail...>>;
};

// At least two more:
template <u64 N, neo_ttparam L, tn... Acc, tn A, tn B, tn... Tail>
struct split_at_<N, L<Acc...>, L<A, B, Tail...>, 1>
    : split_at_<N - 2, L<Acc..., A, B>, L<Tail...>> {};

// At least four more:
template <u64 N, neo_ttparam L, tn... Acc, tn A, tn B, tn C, tn D, tn... Tail>
struct split_at_<N, L<Acc...>, L<A, B, C, D, Tail...>, 2>
    : split_at_<N - 4, L<Acc..., A, B, C, D>, L<Tail...>> {};

// At least eight more:
template <u64         N,
          neo_ttparam L,
          tn... Acc,
          tn A,
          tn B,
          tn C,
          tn D,
          tn E,
          tn F,
          tn G,
          tn H,
          tn... Tail>
struct split_at_<N, L<Acc...>, L<A, B, C, D, E, F, G, H, Tail...>, 3>
    : split_at_<N - 8, L<Acc..., A, B, C, D, E, F, G, H>, L<Tail...>> {};

template <u64         N,
          u64         Log2,
          neo_ttparam L,
          tn... Acc,
          tn T0,
          tn T1,
          tn T2,
          tn T3,
          tn T4,
          tn T5,
          tn T6,
          tn T7,
          tn T8,
          tn T9,
          tn Ta,
          tn Tb,
          tn Tc,
          tn Td,
          tn Te,
          tn Tf,
          tn... Tail>
    requires(Log2 > 3)  //
struct split_at_<N,
                 L<Acc...>,
                 L<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ta, Tb, Tc, Td, Te, Tf, Tail...>,
                 Log2>
    : split_at_<N - 16,
                L<Acc..., T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ta, Tb, Tc, Td, Te, Tf>,
                L<Tail...>> {};

}  // namespace detail

template <tn L, u64 N>
using split_at = typename detail::split_at_<N, clear<L>, L>::type;

template <tn L, u64 N>
using first_n = head<split_at<L, N>>;

namespace detail {

template <neo_ttparam L, tn Voids>
struct remove_prefix_fn;

template <neo_ttparam L, tn... Voids>
struct remove_prefix_fn<L, list<Voids...>> {
    template <tn... Ts>
    static L<Ts...> r(Voids*..., tag<Ts>*...);
};

template <u64 N>
struct remove_prefix_ {
    using voids = meta::filled_list<void, N>;

    template <tn L>
    struct from;

    template <neo_ttparam L, tn... Ts>
    struct from<L<Ts...>> {
        static_assert(sizeof...(Ts) >= N, "remove_prefix N is too large");
        using type = decltype(remove_prefix_fn<L, voids>::r(static_cast<tag<Ts>*>(nullptr)...));
    };
};

template <tn Vs>
struct at_fn;

template <neo_ttparam L, tn... Voids>
struct at_fn<L<Voids...>> {
    template <tn T>
    static tag<T> r(Voids*..., list<T>*, ...);
};

template <u64 N>
struct at_ {
    using voids = meta::filled_list<void, N>;

    template <tn L>
    struct from;

    template <neo_ttparam L, tn... Ts>
    struct from<L<Ts...>> {
        static_assert(sizeof...(Ts) > N, "N index is out-of-bounds for list");
        using type = tn decltype(at_fn<voids>::r(static_cast<list<Ts>*>(nullptr)...))::type;
    };
};

/// XXX: In the future, we can use the concept-prefix trick to more quickly
/// implement remove_suffix. GCC currently has issues with it

// tl <tn T, tn Void>
// concept okay = true;

// tl <tn L>
// struct remove_suffix_fn;

// tl <tn... Voids>
// struct remove_suffix_fn<list<Voids...>> {
//     static auto r(okay<Voids> auto*..., ...);
// };

// tl <u64 N>
// struct remove_suffix {
//     tl <tn L>
//     struct from;

//     tl <neo_ttparam L, tn... Ts>
//     struct from<L<Ts...>> {
//         using type = decltype(remove_suffix_fn<meta::filled_list<void, sizeof...(Ts) - N>>::r(
//             static_cast<tag<Ts>*>(nullptr)...));
//     };
// };

}  // namespace detail

/**
 * @brief Obtain the Nth element of the given typelist
 */
template <tn L, u64 N>
using at = tn detail::at_<N>::template from<L>::type;

namespace tacit {

template <u64 N>
struct remove_prefix {
    template <tn L>
    using f = tn meta::detail::remove_prefix_<N>::tl from<L>::type;
};

}  // namespace tacit

/**
 * @brief Obtain a new typelist with N elements removed from the front
 */
template <tn L, u64 N>
using remove_prefix = tn detail::remove_prefix_<N>::tl from<L>::type;

namespace tacit {

template <u64 N>
struct remove_suffix {
    template <tn L>
    using f = meta::first_n<L, (len_v<L> - N)>;
};

}  // namespace tacit

/**
 * @brief Obtain a new typelist with N elements removed from the back
 */
template <tn L, u64 N>
using remove_suffix = first_n<L, (len_v<L> - N)>;

namespace tacit {

template <u64 Pos, u64 Len = ~(u64)(0)>
struct slice {
    template <tn L>
        requires(Pos + Len <= len_v<L>)
    using f = pipe<remove_prefix<Pos>, remove_suffix<(len_v<L> - Pos - Len)>>::tl f<L>;
};

template <u64 Pos>
struct slice<Pos, ~(u64)(0)> : remove_prefix<Pos> {};

}  // namespace tacit

/**
 * @brief Obtain a subsequence of a given list's template arguments
 *
 * @tparam L The listst to modify
 * @tparam Pos The position at which to begin the slice (Zero-indexed)
 * @tparam Len The number of elements to take beginning at Pos. Default is the remainder of the
 * list.
 */
template <typename L, u64 Pos, u64 Len = ~(u64)(0)>
using slice = tn tacit::slice<Pos, Len>::tl f<L>;

namespace tacit {

template <u64 N>
struct chunk {

    template <tn L, tn Seq = std::make_index_sequence<(len_v<L> / N) + (len_v<L> % N != 0)>>
    struct impl;

    template <neo_ttparam L, tn... Ts, auto... I>
    struct impl<L<Ts...>, std::index_sequence<I...>> {
        constexpr static auto len = sizeof...(Ts);
        using type = L<meta::slice<L<Ts...>, N * I, (N > (len - N * I) ? len - N * I : N)>...>;
    };

    template <tn L>
    using f = tn impl<L>::type;
};

}  // namespace tacit

/**
 * @brief Obtain a new list-of-lists, taken as consecutive slices of length N from L
 *
 * @tparam L The list to inspect
 * @tparam N The length of chunks to divide into
 *
 * @note If N does not even divide L, then the final chunk will be shorter than N
 */
template <tn L, u64 N>
using chunk = tn tacit::chunk<N>::template f<L>;

namespace detail {

template <tn... Ls>
struct append;

template <>
struct append<> {
    using type = list<>;
};

template <neo_ttparam L, tn... Ts>
struct append<L<Ts...>> {
    using type = L<Ts...>;
};

template <neo_ttparam L, tn... Ts, tn... Us, tn... Tail>
struct append<L<Ts...>, L<Us...>, Tail...> : append<L<Ts..., Us...>, Tail...> {};

template <neo_ttparam L,
          tn... Ts,
          tn... Us,
          tn... Vs,
          tn... Ws,
          tn... Xs,
          tn... Ys,
          tn... Zs,
          tn... As,
          tn... Tail>
struct append<L<Ts...>,
              L<Us...>,
              L<Vs...>,
              L<Ws...>,
              L<Xs...>,
              L<Ys...>,
              L<Zs...>,
              L<As...>,
              Tail...>
    : append<L<Ts..., Us..., Vs..., Ws..., Xs..., Ys..., Zs..., As...>, Tail...> {};

}  // namespace detail

/**
 * @brief Concatenate the given lists into a single longer list
 *
 * @tparam Ls Zero or more lists to concatenate.
 *
 * @note Each list must be a specialization of the same class template. The result will be a
 * specialization of that class template. If given on lists, the result is an empty meta::list<>
 */
template <tn... Ls>
using append = tn detail::append<Ls...>::type;

namespace tacit {

template <neo_ttparam NewList>
struct rebind {
    template <tn L>
    struct impl;

    template <neo_ttparam OldList, tn... Ts>
    struct impl<OldList<Ts...>> {
        using type = NewList<Ts...>;
    };

    template <tn L>
    using f = tn impl<L>::type;
};

}  // namespace tacit

/**
 * @brief Copy the template arguments from L an apply them to the class template NewL
 *
 * @tparam L An input list or other class template
 * @tparam NewL The class template to specialize with L's arguments.
 */
template <typename L, neo_ttparam NewL>
using rebind = typename tacit::rebind<NewL>::tl f<L>;

}  // namespace neo::meta

#undef u64
#undef tn
#undef tl
