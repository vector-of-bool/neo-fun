#pragma once

#include <cstddef>

#include "./assert.hpp"
#include "./enum.hpp"
#include "./iterator_facade.hpp"

namespace neo {

namespace utf8_detail {

constexpr int countl_one(std::byte b) {
    int n = 0;
    while ((b & std::byte(1 << 7)) != std::byte()) {
        ++n;
        b <<= 1;
    }
    return n;
}

}  // namespace utf8_detail

/**
 * @brief Types of errors that can appear while decoding UTF-8
 */
enum class utf8_errc {
    /// No error
    none = 0,
    /// An invalid byte following the start byte
    invalid_continuation_byte = 1,
    /// An invalid beginning of a codepoint
    invalid_start_byte = 2,
    /// A stream that ended before reading a character
    need_more = 3,
    /// An over-long encoded codepoint was encountered
    overlong_encoded = 4,
    /// A character beyond the valid range
    invalid_codepoint = 5,
};

constexpr bool operator!(utf8_errc e) { return e == utf8_errc::none; }

/**
 * @brief The result of attempting to decode a single codepoint from a UTF-8 stream
 */
struct utf8_codepoint {
    // The codepoint value if no error occurred. Unspecified otherwise
    char32_t codepoint = 0;
    // The number of bytes that were scanned from the input stream to produce this result
    std::size_t size = 0;

    /**
     * @brief Obtain the error from decoding, if present.
     */
    constexpr utf8_errc error() const noexcept {
        return static_cast<utf8_errc>(codepoint >> (32 - 11));
    }

    /**
     * @brief Create a result object that contains the specified error
     *
     * @param ec The error to represent
     * @param size The number of bytes that were consumed
     */
    constexpr static utf8_codepoint make_error(utf8_errc ec, std::size_t size = 0) noexcept {
        return {static_cast<char32_t>(ec) << (32 - 11), size};
    }
};

/**
 * @brief Decode the next codepoint from a UTF-8 range represented by the two given iterators
 *
 * @param it The beginning of the UTF-8 stream
 * @param stop The end of the UTF-8 stream
 */
template <std::input_iterator Iter, std::sentinel_for<Iter> Stop>
constexpr utf8_codepoint
next_utf8_codepoint(Iter it, const Stop stop) noexcept(nothrow_advancing_iterator<Iter>) {
    if (it == stop) {
        return utf8_codepoint::make_error(utf8_errc::need_more);
    }
    using u8              = std::uint8_t;
    const auto first_byte = static_cast<std::byte>(*it);

    // The number of leading one-bits is the length of the number of CUs in the stream
    int n_bytes = utf8_detail::countl_one(first_byte);
    if (n_bytes == 0) {
        // A zero represents one code unit, which is just one codepoint:
        return {static_cast<char32_t>(*it), 1};
    } else if (n_bytes > 4 || n_bytes == 1) {
        // More than four high bits is an invalid start byte:
        return utf8_codepoint::make_error(utf8_errc::invalid_start_byte);
    }

    // Pick a mask based on the number of bytes
    constexpr u8 lead_mask[] = {
        0,            // Zero bytes. Never happens
        0,            // One byte. Never happens here
        0b0001'1111,  // Two byte
        0b0000'1111,  // Three byte
        0b0000'0111,  // Four byte
    };
    constexpr char32_t min_codepoint[] = {
        0,
        0,
        0x80,
        0x800,
        0x10'000,
    };

    // Init the character with the relevant bits in the mask for the lead byte
    const u8 mask = lead_mask[n_bytes];
    char32_t ret  = static_cast<u8>(*it) & mask;
    ++it;

    // Decode remaining bytes
    auto remain = n_bytes - 1;
    for (; it != stop && remain; ++it, --remain) {
        const std::byte c = static_cast<std::byte>(*it);
        if (utf8_detail::countl_one(c) != 1) {
            // Each continuation should have a single leading 1 bit
            return utf8_codepoint::make_error(utf8_errc::invalid_continuation_byte,
                                              static_cast<std::size_t>(n_bytes - remain));
        }
        // Insert the next six bits
        ret <<= 6;
        ret |= static_cast<char32_t>(c & std::byte{0b00'11'11'11});
    }

    // If we broke out before reading all expected bytes, the sequence is incomplete
    if (remain != 0) {
        return utf8_codepoint::make_error(utf8_errc::need_more,
                                          static_cast<std::size_t>(n_bytes - remain));
    }

    if (ret < min_codepoint[n_bytes]) {
        // This codepoint could fit within the range of one fewer bytes, so this is an overlong
        // character
        return utf8_codepoint::make_error(utf8_errc::overlong_encoded,
                                          static_cast<std::size_t>(n_bytes));
    }

    if (ret > 0x10'ffff) {
        // Outside of the Unicode range
        return utf8_codepoint::make_error(utf8_errc::invalid_codepoint,
                                          static_cast<std::size_t>(n_bytes));
    }

    // Got one!
    return {ret, static_cast<std::size_t>(n_bytes)};
}

extern template utf8_codepoint next_utf8_codepoint(const std::byte*, const std::byte*) noexcept;

// Overload that normalizes all pointer-like iterators into std::byte pointers
template <std::contiguous_iterator I, std::sized_sentinel_for<I> S>
    requires(sizeof(std::iter_value_t<I>) == 1) and (not neo::weak_same_as<I, const std::byte*>)
constexpr utf8_codepoint
    next_utf8_codepoint(I iter, const S stop) noexcept(nothrow_advancing_iterator<I>) {
    auto b = reinterpret_cast<const std::byte*>(std::to_address(iter));
    return neo::next_utf8_codepoint(b, b + (stop - iter));
}

/**
 * @brief Decode a single UTF-8 encoded Unicode codepoint from the beginning of the given range
 *
 * @param rng The range from which byte data will be extracted
 * @return constexpr utf8_codepoint The decoding result, which may indicate an error
 */
constexpr utf8_codepoint next_utf8_codepoint(std::ranges::input_range auto&& rng) {
    return neo::next_utf8_codepoint(std::ranges::begin(rng), std::ranges::end(rng));
}

/**
 * @brief Adapt a range of UTF8 code units into a range of UTF-32 codepoints
 *
 * @tparam Range A range or reference to a range
 */
template <typename Range>
class utf8_range {
    NEO_NO_UNIQUE_ADDRESS scalar_box<Range> _range;

    using _iter_type = decltype(std::cbegin(_range.get()));
    using _stop_type = decltype(std::cend(_range.get()));

public:
    constexpr utf8_range() = default;

    constexpr explicit utf8_range(Range&& r)
        : _range{NEO_FWD(r)} {}

    class iterator : public iterator_facade<iterator> {
        _iter_type _begin{};
        _iter_type _iter{};
        _stop_type _stop{};

    public:
        constexpr iterator() = default;

        struct sentinel_type {};

        constexpr iterator(_iter_type begin, _iter_type it, _stop_type stop)
            : _begin(begin)
            , _iter(it)
            , _stop(stop) {}

        constexpr utf8_codepoint dereference() const noexcept {
            return next_utf8_codepoint(_iter, _stop);
        }

        constexpr _iter_type base() const noexcept { return _iter; }

        constexpr void increment() {
            neo_assert(expects, _iter != _stop, "Advanced the past-the-end utf8_range iterator");

            auto n_bytes = utf8_detail::countl_one(std::byte(*_iter));
            n_bytes      = n_bytes ? n_bytes : 1;
            while (n_bytes && _iter != _stop) {
                ++_iter;
                --n_bytes;
            }
        }

        constexpr void decrement() {
            neo_assert(expects, _iter != _begin, "Rewound the begin utf8_range iterator");
            --_iter;
            while (_iter != _begin && utf8_detail::countl_one(std::byte(*_iter)) == 1) {
                --_iter;
            }
        }

        constexpr bool at_end() const noexcept { return _iter == _stop; }

        constexpr bool operator==(iterator other) const noexcept { return _iter == other._iter; }
        constexpr bool operator==(sentinel_type) const noexcept { return at_end(); }

        constexpr std::string_view tail_string() const noexcept {
            return at_end() ? "" : std::string_view(&*_iter, _stop - _iter);
        }
    };

    constexpr iterator begin() const noexcept {
        return iterator{std::cbegin(_range.get()),
                        std::cbegin(_range.get()),
                        std::cend(_range.get())};
    }
    constexpr typename iterator::sentinel_type end() const noexcept { return {}; }
};

template <typename R>
explicit utf8_range(R&&) -> utf8_range<R>;

}  // namespace neo

template <typename R>
constexpr inline bool std::ranges::enable_view<neo::utf8_range<R>> = std::ranges::view<R>;
