#pragma once

#include <iterator>

namespace neo {

/**
 * @brief Type-erased iterator whose iter_reference_t is 'Ref' or convertible to 'Ref', and whose
 * iterator category is 'Category'
 *
 * Can be constructed with CTAD. The reference type of the erased iterator must be convertible to
 * 'Ref'.
 */
template <typename Reference, typename Category>
class any_iterator;

template <typename Ref>
using any_input_iterator = any_iterator<Ref, std::input_iterator_tag>;

template <typename Ref>
using any_forward_iterator = any_iterator<Ref, std::forward_iterator_tag>;

template <typename Ref>
using any_bidirectional_iterator = any_iterator<Ref, std::bidirectional_iterator_tag>;

template <typename Ref>
using any_random_access_iterator = any_iterator<Ref, std::random_access_iterator_tag>;

/**
 * @brief A type-erased sentinel object.
 *
 * Should be constructed with an invocable that acts as a boolean predicate on an
 * `erased_iterator_base_parts const&`, returning `true` iff the iterator has reached the end
 * denoted by this sentinel.
 */
class any_sentinel;

}  // namespace neo