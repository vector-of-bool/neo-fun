#pragma once

#include <utility>

namespace neo {

#define u64 unsigned long long

/**
 * @brief Pick the Nth type from a variadic type list
 *
 * @tparam N The index to pick from
 *
 * Use the nested `from<Ts...>` struct to pass a variadic list of types
 */
template <u64 N>
struct pick_type;

template <>
struct pick_type<0> {
    template <typename T, typename...>
    struct from {
        using type = T;
    };
};

template <>
struct pick_type<1> {
    template <typename, typename T, typename...>
    struct from {
        using type = T;
    };
};

template <>
struct pick_type<2> {
    template <typename, typename, typename T, typename...>
    struct from {
        using type = T;
    };
};

template <>
struct pick_type<3> {
    template <typename, typename, typename, typename T, typename...>
    struct from {
        using type = T;
    };
};

template <u64 N>
requires(N >= 4 and N < 16) struct pick_type<N> {
    template <typename, typename, typename, typename, typename... Tail>
    struct from : pick_type<N - 4>::template from<Tail...> {};
};

template <u64 N>
requires(N >= 16 and N < 64) struct pick_type<N> {
    template <typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename... Tail>
    struct from : pick_type<N - 16>::template from<Tail...> {};
};

template <u64 N>
requires(N >= 64) struct pick_type<N> {
    template <typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename,
              typename... Tail>
    struct from : pick_type<N - 64>::template from<Tail...> {};
};

/**
 * @brief Select the Nth type from the list of variadic typs
 *
 * @tparam N The index (zero-based)
 * @tparam Ts Some number of types to read from
 */
template <u64 N, typename... Ts>
using pick_type_t = typename pick_type<N>::template from<Ts...>::type;

/**
 * @brief Pick the Nth type argument from a template specialization
 *
 * @tparam N The index to pick from
 */
template <u64 N>
struct pick_type_of {
    /// Specify the template specialization to pick from
    template <typename Seq>
    struct from;

    template <template <typename...> class L, typename... Ts>
    struct from<L<Ts...>> : pick_type<N>::template from<Ts...> {};
};

/**
 * @brief Obtain the Nth template argument for the given template specialization
 *
 * @tparam N The index to pick (zero-based)
 * @tparam Seq A template specialization (Only type parameters are supported)
 */
template <u64 N, typename Seq>
using pick_type_of_t = typename pick_type_of<N>::template from<Seq>::type;

namespace find_type_detail {

template <u64 N, typename T, typename U>
struct find_type_one {};

template <u64 N, typename T>
struct find_type_one<N, T, T> {
    enum { value = N };
};

template <typename Seq, typename T, typename... Ts>
struct finder;

template <std::size_t... Ns, typename T, typename... Ts>
struct finder<std::index_sequence<Ns...>, T, Ts...> : find_type_one<Ns, T, Ts>... {};

}  // namespace find_type_detail

template <typename Needle, typename... Ts>
struct find_type : find_type_detail::finder<std::index_sequence_for<Ts...>, Needle, Ts...> {};

template <typename Needle, typename... Ts>
constexpr u64 find_type_v = find_type<Needle, Ts...>::value;

#undef u64

}  // namespace neo
