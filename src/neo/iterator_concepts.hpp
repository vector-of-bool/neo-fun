#pragma once

#include "./concepts.hpp"
#include "./iterator.hpp"

namespace neo {

using std::iter_difference_t;
using std::iter_reference_t;
using std::iter_rvalue_reference_t;
using std::iter_value_t;

template <typename T>
concept weakly_incrementable =  //
    requires(T it) {
        { ++it } -> weak_same_as<T&>;
        it++;
        typename iter_difference_t<T>;
        requires signed_integral<iter_difference_t<T>>;
    }  //
    and movable<T>;

template <typename I>
concept input_or_output_iterator =  //
    requires(I it) {
        { *it } -> neo::detail::can_reference;
    }  //
    and weakly_incrementable<I>;

template <typename S, typename I>
concept sentinel_for =                                  //
    neo::detail::weakly_equality_comparable_with<S, I>  //
    and input_or_output_iterator<I>                     //
    and semiregular<S>;

template <typename S, typename I>
concept sized_sentinel_for =  //
    sentinel_for<S, I>        //
    and requires(const I& it, const S& sent) {
        { sent - it } -> weak_same_as<iter_difference_t<I>>;
        { it - sent } -> weak_same_as<iter_difference_t<I>>;
    } //
    and not
std::disable_sized_sentinel_for<remove_cv_t<S>, remove_cv_t<I>>;

namespace detail {

template <typename It>
concept indirectly_readable_impl =  //
    requires(const It it) {
        typename iter_reference_t<It>;
        typename iter_rvalue_reference_t<It>;
        typename iter_value_t<It>;
    }  //
    and common_reference_with<iter_reference_t<It>&&, iter_value_t<It>&>
    and common_reference_with<iter_reference_t<It>&&, iter_rvalue_reference_t<It>&&>
    and common_reference_with<iter_rvalue_reference_t<It>&&, const iter_value_t<It>&>;

}  // namespace detail

template <typename It>
concept indirectly_readable = detail::indirectly_readable_impl<remove_cvref_t<It>>;

template <typename It>
concept input_iterator =                                           //
    input_or_output_iterator<It>                                   //
    and requires { typename iter_concept_t<It>; }                  //
    and derived_from<iter_concept_t<It>, std::input_iterator_tag>  //
    and indirectly_readable<It>;

template <typename It>
concept incrementable =  //
    requires(It it) {
        { it++ } -> weak_same_as<It>;
    }                             //
    and weakly_incrementable<It>  //
    and regular<It>;

template <typename It>
concept forward_iterator =                                           //
    incrementable<It>                                                //
    and input_iterator<It>                                           //
    and derived_from<iter_concept_t<It>, std::forward_iterator_tag>  //
    and sentinel_for<It, It>;

template <typename It>
concept bidirectional_iterator =  //
    requires(It it) {
        { --it } -> weak_same_as<It&>;
        { it-- } -> weak_same_as<It>;
    }                         //
    and forward_iterator<It>  //
    and derived_from<iter_concept_t<It>, std::bidirectional_iterator_tag>;

template <typename It>
concept random_access_iterator =  //
    requires {
        typename iter_reference_t<It>;
        typename iter_difference_t<It>;
    }
    and requires(It it, const It cit, const iter_difference_t<It> off) {
            { it += off } -> weak_same_as<It&>;
            { cit + off } -> weak_same_as<It>;
            { off + cit } -> weak_same_as<It>;
            { it -= off } -> weak_same_as<It&>;
            { cit - off } -> weak_same_as<It>;
            { cit[off] } -> weak_same_as<iter_reference_t<It>>;
        } //
    and totally_ordered<It>
    and bidirectional_iterator<It>
    and sized_sentinel_for<It, It>
    and derived_from<iter_concept_t<It>, std::random_access_iterator_tag>
    ;

template <typename It>
concept contiguous_iterator =   //
    random_access_iterator<It>  //
    and lvalue_reference_type<iter_reference_t<It>>
    and derived_from<iter_concept_t<It>, std::contiguous_iterator_tag>
    and weak_same_as<iter_value_t<It>, remove_cvref_t<iter_reference_t<It>>>
    and requires(const It& it) {
            { std::to_address(it) } -> weak_same_as<add_pointer_t<iter_reference_t<It>>>;
        };

template <typename Out, typename T>
concept indirectly_writable =  //
    requires(Out&& out, T&& t) {
        *out                                                     = NEO_FWD(t);
        *NEO_FWD(out)                                            = NEO_FWD(t);
        const_cast<const iter_reference_t<Out>&&>(*out)          = NEO_FWD(t);
        const_cast<const iter_reference_t<Out>&&>(*NEO_FWD(out)) = NEO_FWD(t);
    };

template <typename It, typename T>
concept output_iterator =  //
    requires(It i, T&& v) { *i++ = NEO_FWD(v); }
    and input_or_output_iterator<It> and indirectly_writable<It, T>;

}  // namespace neo
