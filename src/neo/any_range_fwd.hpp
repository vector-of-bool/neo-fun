#pragma once

#include <iterator>

namespace neo {

/**
 * @brief A type-erased handle on an range of the corresponding category
 *
 * @tparam Ref The reference type of the range
 * @tparam Category The minimum required category of the range
 */
template <typename Ref, typename Category>
class any_range;

template <typename Ref>
using any_input_range = any_range<Ref, std::input_iterator_tag>;

template <typename Ref>
using any_forward_range = any_range<Ref, std::forward_iterator_tag>;

template <typename Ref>
using any_bidirectional_range = any_range<Ref, std::bidirectional_iterator_tag>;

template <typename Ref>
using any_random_access_range = any_range<Ref, std::random_access_iterator_tag>;

}  // namespace neo
