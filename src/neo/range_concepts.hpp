#pragma once

#include "./fwd.hpp"
#include "./ref.hpp"
#include "./version.hpp"

#include "./iterator_concepts.hpp"

#include <limits>

#if __cpp_lib_ranges
#include <ranges>
#endif

namespace neo::ranges {

#if __cpp_lib_ranges
using std::ranges::begin;
using std::ranges::cbegin;
using std::ranges::cdata;
using std::ranges::cend;
using std::ranges::crbegin;
using std::ranges::crend;
using std::ranges::data;
using std::ranges::empty;
using std::ranges::end;
using std::ranges::rbegin;
using std::ranges::rend;
using std::ranges::size;
using std::ranges::ssize;

// Type aliases
using std::ranges::iterator_t;
using std::ranges::range_reference_t;
using std::ranges::range_rvalue_reference_t;
using std::ranges::range_size_t;
using std::ranges::range_value_t;
using std::ranges::sentinel_t;

// Concept aliases
using std::ranges::bidirectional_range;
using std::ranges::contiguous_range;
using std::ranges::forward_range;
using std::ranges::input_range;
using std::ranges::output_range;
using std::ranges::random_access_range;
using std::ranges::range;
using std::ranges::sized_range;

#else

using std::begin;
using std::cbegin;
// using std::cdata;
using std::cend;
using std::crbegin;
using std::crend;
using std::data;
using std::empty;
using std::end;
using std::rbegin;
using std::rend;
using std::size;
// using std::ssize;

#define DECL_ALIAS(Concept, Name, TypeExpr)                                                        \
    template <Concept T>                                                                           \
    using Name = TypeExpr

DECL_ALIAS(typename, iterator_t, decltype(neo::ranges::begin(NEO_DECLVAL(T&))));

// clang-format off
template <typename T>
concept range = requires(T& t) {
    begin(t);
    end(t);
};

DECL_ALIAS(range, sentinel_t, decltype(neo::ranges::end(NEO_DECLVAL(T&))));
DECL_ALIAS(range, range_reference_t, iter_reference_t<iterator_t<T>>);
DECL_ALIAS(range, range_difference_t, iter_difference_t<iterator_t<T>>);
DECL_ALIAS(range, range_value_t, iter_value_t<iterator_t<T>>);
DECL_ALIAS(range, range_rvalue_reference_t, iter_rvalue_reference_t<iterator_t<T>>);


// clang-format off

template <typename T>
concept sized_range =
    range<T> &&
    requires(T& t) {
        size(t);
    };

DECL_ALIAS(sized_range, range_size_t, decltype(neo::ranges::size(NEO_DECLVAL(T&))));
#undef DECL_ALIAS

template <typename T>
concept input_range =
    range<T> &&
    neo::input_iterator<iterator_t<T>>;

template <typename T, typename V>
concept output_range =
    range<T> &&
    neo::output_iterator<iterator_t<T>, V>;

template <typename T>
concept common_range =
    range<T> && neo::same_as<iterator_t<T>, sentinel_t<T>>;

template <typename T>
concept forward_range =
    input_range<T> &&
    neo::forward_iterator<iterator_t<T>>;

template <typename T>
concept bidirectional_range =
    forward_range<T> &&
    neo::bidirectional_iterator<iterator_t<T>>;

template <typename T>
concept random_access_range =
    bidirectional_range<T> &&
    neo::random_access_iterator<iterator_t<T>>;

template <typename T>
concept contiguous_range =
    random_access_range<T> &&
    neo::contiguous_iterator<iterator_t<T>> &&
    requires (T& t) {
        { data(t) } ->
            same_as<std::add_pointer_t<range_reference_t<T>>>;
    };

// clang-format on

template <contiguous_range T>
constexpr auto cdata(T&& t) noexcept {
    using CT      = make_cref_t<T>;
    using RetType = std::remove_reference_t<range_reference_t<CT>>;
    return data(static_cast<RetType>(t));
}

template <sized_range T>
constexpr auto ssize(T&& t) {
    using range_diff = range_difference_t<T>;
    if constexpr (std::numeric_limits<range_diff>::digits
                  < std::numeric_limits<std::ptrdiff_t>::digits) {
        return static_cast<std::ptrdiff_t>(size(NEO_FWD(t)));
    } else {
        return static_cast<range_diff>(size(NEO_FWD(t)));
    }
}

#endif

}  // namespace neo::ranges
