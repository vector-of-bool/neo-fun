#pragma once

#include "./archetypes.hpp"

#include <ranges>

NEO_PRAGMA_WARNING_PUSH();
NEO_MSVC_PRAGMA(warning(disable : 4624));

namespace neo::arch {

template <typename Derived>
struct make_weakly_incrementable : semiregular {
    make_weakly_incrementable();
    Derived& operator++() &;
    void     operator++(int) &;
    using difference_type = int;
};

template <typename Reference>
struct make_indirectly_readable {
    Reference operator*() const;
    using value_type = std::remove_cvref_t<Reference>;
};

template <typename Derived>
struct make_incrementable : regular {
    Derived& operator++() &;
    Derived  operator++(int) &;
    using difference_type = int;
};

struct weakly_incrementable : make_weakly_incrementable<weakly_incrementable> {};

struct incrementable : make_incrementable<incrementable> {};

template <typename T = pathological&>
struct input_or_output_iterator : make_weakly_incrementable<input_or_output_iterator<T>>,
                                  make_indirectly_readable<T> {};

template <typename T = pathological&>
struct input_iterator : make_weakly_incrementable<input_iterator<T>>, make_indirectly_readable<T> {
    using iterator_concept = std::input_iterator_tag;
};

template <typename T = pathological&>
struct forward_iterator : make_incrementable<forward_iterator<T>>, make_indirectly_readable<T> {
    using iterator_concept = std::forward_iterator_tag;
};

template <typename T = pathological&>
struct bidirectional_iterator : make_incrementable<bidirectional_iterator<T>>,
                                make_indirectly_readable<T> {
    using iterator_category = std::bidirectional_iterator_tag;

    bidirectional_iterator& operator--() &;
    bidirectional_iterator  operator--(int) &;
};

template <typename T = pathological&>
struct random_access_iterator
    : make_incrementable<random_access_iterator<T>>,
      make_indirectly_readable<T>,
      decl_order_ops<random_access_iterator<T>, random_access_iterator<T>> {

    using iterator_category = std::random_access_iterator_tag;

    random_access_iterator& operator--() &;
    random_access_iterator  operator--(int) &;

    random_access_iterator&       operator+=(int) &;
    random_access_iterator        operator+(int) const;
    friend random_access_iterator operator+(int, const random_access_iterator&) { return {}; }

    random_access_iterator& operator-=(int) &;
    random_access_iterator  operator-(int) const;
    int                     operator-(const random_access_iterator&) const;

    T operator[](int) const;
};

template <typename T = pathological&>
struct contiguous_iterator : make_incrementable<contiguous_iterator<T>>,
                             make_indirectly_readable<T>,
                             decl_order_ops<contiguous_iterator<T>, contiguous_iterator<T>> {
    using iterator_category = std::contiguous_iterator_tag;

    contiguous_iterator& operator--() &;
    contiguous_iterator  operator--(int) &;

    contiguous_iterator&       operator+=(int) &;
    contiguous_iterator        operator+(int) const;
    friend contiguous_iterator operator+(int, contiguous_iterator) { return {}; }

    contiguous_iterator& operator-=(int) &;
    contiguous_iterator  operator-(int) const;
    int                  operator-(contiguous_iterator) const;

    T operator[](int) const;

    std::remove_reference_t<T>* operator->() const;
};

template <typename Out>
struct indirectly_writable : pathological {
    assignable_from<Out>& operator*();
};

template <typename Output>
struct output_iterator : make_incrementable<output_iterator<Output>> {
    assignable_from<Output>& operator*();
};

template <std::input_or_output_iterator Iter>
struct sentinel_for : semiregular {
    bool operator==(const Iter&) const&;
};

template <std::input_or_output_iterator Iter>
struct sized_sentinel_for : sentinel_for<Iter> {
    friend int operator-(const sized_sentinel_for&, const Iter&) { return 0; }
    friend int operator-(const Iter&, const sized_sentinel_for&) { return 0; }
};

template <std::input_or_output_iterator Iterator = input_or_output_iterator<>,
          std::sentinel_for<Iterator>   Sentinel = sentinel_for<Iterator>>
struct range : pathological {
    Iterator begin() &;
    Sentinel end() &;
};

template <std::input_or_output_iterator Iterator = input_or_output_iterator<>,
          std::sentinel_for<Iterator>   Sentinel = sentinel_for<Iterator>>
struct borrowed_range : range<Iterator, Sentinel> {};

template <std::input_or_output_iterator Iterator = input_or_output_iterator<>,
          std::sentinel_for<Iterator>   Sentinel = sentinel_for<Iterator>>
struct sized_range : range<Iterator, Sentinel> {
    int size();
};

template <typename T = pathological&>
struct input_range : range<input_iterator<T>> {};

template <typename T = pathological&>
struct forward_range : range<forward_iterator<T>> {};

template <typename T = pathological&>
struct bidirectional_range : range<bidirectional_iterator<T>> {};

template <typename T = pathological&>
struct random_access_range : range<random_access_iterator<T>> {};

template <typename T = pathological&>
struct contiguous_range : range<contiguous_iterator<T>> {};

template <typename T = pathological&>
struct sized_input_range : sized_range<input_iterator<T>> {};

template <typename T = pathological&>
struct output_range : range<output_iterator<T>> {};

}  // namespace neo::arch

template <typename It, typename Se>
inline constexpr bool std::ranges::enable_borrowed_range<neo::arch::borrowed_range<It, Se>> = true;

NEO_PRAGMA_WARNING_POP();
