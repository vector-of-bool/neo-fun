#pragma once

#include "./any_range_fwd.hpp"

#include "./any_iterator.hpp"
#include "./assert.hpp"
#include "./fwd.hpp"
#include "./ref_member.hpp"
#include "./unref.hpp"

#include <memory>
#include <ranges>

namespace neo {

namespace range_detail {

// Type-erased storage for an any_range
template <typename Ref, typename Category>
class erased_range_base {
private:
    using iterator = any_iterator<Ref, Category>;
    using sentinel = std::conditional_t<std::derived_from<Category, std::forward_iterator_tag>,
                                        iterator,
                                        any_sentinel>;
    virtual iterator do_begin() = 0;
    virtual sentinel do_end()   = 0;

public:
    virtual ~erased_range_base() = default;

    iterator begin() { return do_begin(); }
    sentinel end() { return do_end(); }
};

template <typename Ref, typename Cat, typename Range>
class erased_range_impl : public erased_range_base<Ref, Cat> {
private:
    using iterator = any_iterator<Ref, Cat>;
    using sentinel = std::
        conditional_t<std::derived_from<Cat, std::forward_iterator_tag>, iterator, any_sentinel>;

    using wrapped_iter = std::ranges::iterator_t<neo::unref_t<Range>>;
    Range _range;

    iterator do_begin() override { return std::ranges::begin(neo::unref(_range)); }
    sentinel do_end() override {
        if constexpr (std::same_as<sentinel, any_sentinel>) {
            return any_sentinel(neo::tag_v<wrapped_iter>, std::ranges::end(neo::unref(_range)));
        } else {
            return std::ranges::end(neo::unref(_range));
        }
    }

public:
    template <typename R>
    explicit erased_range_impl(R&& r)
        : _range(NEO_FWD(r)) {}
};

}  // namespace range_detail

template <typename Ref, typename Category>
class any_range {
    using erased_type = range_detail::erased_range_base<Ref, Category>;

    std::unique_ptr<erased_type> _impl;

    template <typename R>
    std::unique_ptr<erased_type> _make_impl(R&& r) {
        if constexpr (alike<R, any_range>) {
            return NEO_FWD(r)._impl;
        } else {
            return std::make_unique<
                range_detail::erased_range_impl<Ref, Category, std::remove_cvref_t<R>>>(NEO_FWD(r));
        }
    }

public:
    using iterator = any_iterator<Ref, Category>;
    using sentinel = std::conditional_t<std::derived_from<Category, std::forward_iterator_tag>,
                                        iterator,
                                        any_sentinel>;

    any_range(any_range&&) noexcept = default;
    any_range& operator=(any_range&&) noexcept = default;

    // clang-format off
    template <std::ranges::range R>
        requires std::convertible_to<std::ranges::iterator_t<R>, iterator>
    any_range(R&& r)
        : _impl(_make_impl(NEO_FWD(r))) {}

    template <std::ranges::range R>
        requires std::convertible_to<std::ranges::iterator_t<R>, iterator>
    any_range(std::reference_wrapper<R> ref) noexcept
        : _impl(_make_impl(ref)) {}
    // clang-format on

    iterator begin() {
        neo_assert(expects, _impl != nullptr, "Invalid use of an empty/moved-from any_range");
        return _impl->begin();
    }
    sentinel end() {
        neo_assert(expects, _impl != nullptr, "Invalid use of an empty/moved-from any_range");
        return _impl->end();
    }
};

template <std::ranges::range Range>
any_range(Range&& r)
    -> any_range<std::ranges::range_reference_t<Range>,
                 typename std::iterator_traits<std::ranges::iterator_t<Range>>::iterator_category>;

template <std::ranges::range Range>
any_range(std::reference_wrapper<Range>)
    -> any_range<std::ranges::range_reference_t<Range>,
                 typename std::iterator_traits<std::ranges::iterator_t<Range>>::iterator_category>;

}  // namespace neo
