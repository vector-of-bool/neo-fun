#pragma once

#include "./any_iterator.hpp"
#include "./concepts.hpp"
#include "./iterator_facade.hpp"
#include "./memory.hpp"
#include "./range_concepts.hpp"

namespace neo {

/**
 * @brief Abstract base class of erased input ranges for reference type 'Ref'
 */
template <typename Ref>
class erased_input_range {
public:
    virtual ~erased_input_range() = default;

private:
    virtual any_input_iterator<Ref> do_begin() noexcept = 0;
    virtual any_sentinel            do_end() noexcept   = 0;

public:
    auto begin() noexcept { return do_begin(); }
    auto end() noexcept { return do_end(); }

    virtual std::unique_ptr<erased_input_range> clone() const noexcept = 0;
};

/**
 * @brief Implementation of erased input ranges for type 'Ref', erasing type 'Range'
 *
 * Prefer any_input_range. This class supports CTAD. The reference type of 'Range' must be
 * convertible to 'Ref'.
 */
template <typename Ref, typename Range>
requires convertible_to<ranges::range_reference_t<Range>, Ref>  //
class erase_input_range : public erased_input_range<Ref> {
    Range _range;

    any_input_iterator<Ref> do_begin() noexcept override { return ranges::begin(_range); }
    any_sentinel            do_end() noexcept override {
        return any_sentinel([end = ranges::end(_range)](const erased_iterator_base& iter) {
            using iter_t = ranges::iterator_t<Range>;
            neo_assert(expects,
                       iter.type_tag() == type_tag_v<iter_t>,
                       "Comparison of range sentinel to iterator of different range type");
            return iter.get<iter_t>() == end;
        });
    }

public:
    erase_input_range() = default;

    explicit erase_input_range(Range&& r)
        : _range(NEO_FWD(r)) {}

    std::unique_ptr<erased_input_range<Ref>> clone() const noexcept override {
        return copy_unique(*this);
    }
};

template <ranges::input_range Range>
erase_input_range(Range&&) -> erase_input_range<ranges::range_reference_t<Range>, Range>;

/**
 * @brief Type-erased storage of any input_range whose reference type is convertible to 'Ref'
 */
template <typename Ref>
class any_input_range {
    using erased_type = erased_input_range<Ref>;

public:
    using reference = Ref;

private:
    std::unique_ptr<erased_type> _impl;

public:
    // clang-format off
    template <typename R>
    requires ranges::input_range<R>
        && convertible_to<ranges::range_reference_t<R>, reference>
        && unalike<R, any_input_range>
    any_input_range(R&& r)
        : _impl(std::make_unique<erase_input_range<reference, R>>(NEO_FWD(r))) {}
    // clang-format on

    any_input_iterator<Ref> begin() noexcept { return _impl->begin(); }
    any_sentinel            end() noexcept { return _impl->end(); }
};

template <ranges::input_range R>
any_input_range(R&&) -> any_input_range<ranges::range_reference_t<R>>;

template <typename Ref>
any_input_range(const erased_input_range<Ref>&) -> any_input_range<Ref>;

}  // namespace neo
