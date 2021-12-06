#pragma once

#include <iterator>

namespace neo {

namespace iter_detail {

template <typename It>
auto get_concept() {
    if constexpr (std::contiguous_iterator<It>) {
        return std::contiguous_iterator_tag{};
    } else if constexpr (std::random_access_iterator<It>) {
        return std::random_access_iterator_tag{};
    } else if constexpr (std::bidirectional_iterator<It>) {
        return std::bidirectional_iterator_tag{};
    } else if constexpr (std::forward_iterator<It>) {
        return std::forward_iterator_tag{};
    } else if constexpr (std::input_iterator<It>) {
        return std::input_iterator_tag{};
    } else {
        return std::output_iterator_tag{};
    }
}

}  // namespace iter_detail

/**
 * @brief Obtain the category (concept) tag for the given iterator type based on the iterator
 * concept that it models
 */
template <std::input_or_output_iterator Iter>
using iter_concept_t = decltype(iter_detail::get_concept<Iter>());

/**
 * @brief Get the category tag for the given iterator from its iterator_traits
 */
template <typename Iter>
requires requires { typename std::iterator_traits<Iter>::iterator_category; }
using iter_category_t = typename std::iterator_traits<Iter>::iterator_category;

}  // namespace neo
