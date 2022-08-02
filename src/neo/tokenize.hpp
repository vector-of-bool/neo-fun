#pragma once

#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./iterator_facade.hpp"
#include "./reconstruct.hpp"
#include "./substring.hpp"
#include "./text_algo.hpp"
#include "./text_range.hpp"

#include "./enum.hpp"

#include <algorithm>

namespace neo {

/**
 * @brief A simple token type that stores a single view of a text range.
 *
 * @tparam View The type of the text view
 */
template <text_range View>
struct simple_token {
    /// The view of the token text.
    View view;
    /// The point at which to begin searching for the next token
    std::ranges::iterator_t<View> skip{};
};

template <typename FindSplit>
struct simple_token_splitter {
    [[no_unique_address]] FindSplit _find_split;

    template <text_range View>
    requires std::ranges::borrowed_range<View> and neo::reconstructible_range<View>
    constexpr std::optional<simple_token<View>> operator()(View prev,
                                                           View remaining) const noexcept {
        if (std::ranges::empty(remaining)
            and std::ranges::end(prev) == std::ranges::end(remaining)) {
            // The previous token ends at the end of the input stream, which indicates that the full
            // text view does not end with a splitting sequence. Do not yield another token.
            return std::nullopt;
            // If we are at the end of the remaining input stream and there was a splitter, we will
            // end up returning an empty token for the end of the text.
        }

        // Find the next splitter:
        View split = _find_split(std::as_const(remaining));

        // Get the text leading up to the new split
        auto tok = neo::reconstruct_range(remaining,
                                          std::ranges::begin(remaining),
                                          std::ranges::begin(split));

        return simple_token{tok, std::ranges::end(split)};
    }
};

template <typename C>
requires std::predicate<C, char32_t>
struct charclass_splitter {
    [[no_unique_address]] C _classifier{};

    template <text_range V>
    constexpr substring_t<V> operator()(V&& remaining) const noexcept {
        auto skip_start
            = std::ranges::find_if(remaining, [&](char32_t c) { return _classifier(c); });
        auto skip_end = std::ranges::find_if(skip_start,
                                             std::ranges::end(remaining),
                                             [&](char32_t c) { return not _classifier(c); });
        return neo::substring(remaining, skip_start, skip_end);
    }
};

struct is_whitespace_fn {
    constexpr auto operator()(char32_t c) const {
        return c == ' ' or c == '\n' or c == '\r' or c == '\t' or c == '\f';
    }
};

struct find_newline_fn {
    template <text_range T>
    constexpr substring_t<T> operator()(T view) const {
        // A CRLF constant range to help compare against
        std::ranges::range_value_t<T> crlf_arr[3] = {'\r', '\n', 0};
        const auto                    crlf        = borrow_text(crlf_arr);
        std::ranges::range_value_t<T> lf_arr[2]   = {'\n', 0};
        auto                          lf          = borrow_text(lf_arr);
        while (not std::ranges::empty(view) and not neo::starts_with(view, lf)
               and not neo::starts_with(view, crlf)) {
            view = substring(view, 1);
        }
        if (neo::starts_with(view, crlf)) {
            return substring(view, 0, 2);
        } else if (neo::starts_with(view, lf)) {
            return substring(view, 0, 1);
        }
        // "v" is empty:
        return neo::substring(view, 0);
    }
};

/**
 * @brief A simple tokenizer that splits text on whitespace.
 */
struct whitespace_splitter : simple_token_splitter<charclass_splitter<is_whitespace_fn>> {};
struct line_splitter : simple_token_splitter<find_newline_fn> {};

template <typename T, typename Text>
concept token_type = requires(T&& token) {
    { token.view } -> std::convertible_to<Text>;
};

template <typename T, typename Text>
concept token_result = neo::simple_boolean<T> && requires(T&& token) {
    { *token } -> token_type<Text>;
};

template <typename Func, typename Text>
concept tokenizer_fn =  //
    neo::invocable2<Func,
                    Text,
                    Text>  //
    && text_range<Text>    //
    && token_result<neo::invoke_result_t<Func, Text, Text>, Text>;

/**
 * @brief A tokenizer object. Splits a text_range into a range of tokens
 *
 * @tparam R The type of the text range that is being split
 * @tparam Tok The tokenization function
 */
template <text_range R, tokenizer_fn<borrow_text_t<R>> Tok>
class tokenizer {
    /// The text that is being tokenized
    R _text;
    /// The token-splitting function
    [[no_unique_address]] Tok _get_next_token;

public:
    using borrowed_text = borrow_text_t<R&>;

    /**
     * @brief Construct a new tokenizer from a text range and token splitter
     */
    constexpr explicit tokenizer(R&& text, Tok&& tkz) noexcept
        : _text(NEO_FWD(text))
        , _get_next_token(NEO_FWD(tkz)) {}

    constexpr explicit tokenizer(R&& text) noexcept
        : _text(NEO_FWD(text)) {}

    /// The type of the token that is generated by this tokenizer
    using token_result = invoke_result_t<Tok&, borrowed_text, borrowed_text>;

    /**
     * @brief The iterator that walks the range of tokens
     */
    struct iterator : iterator_facade<iterator> {
    private:
        borrowed_text _tail;
        token_result  _current;

        std::remove_reference_t<Tok>* _tokenize = nullptr;

    public:
        iterator() = default;

        struct sentinel_type {};

        explicit iterator(borrowed_text v, Tok& tok)
            : _tail(v)
            , _tokenize(neo::addressof(tok)) {
            increment();
        }

        constexpr const auto& dereference() const noexcept { return _current.value(); }

        constexpr void
        increment() noexcept(noexcept(neo::invoke(*_tokenize, _current->view, _tail))) {
            _current = neo::invoke(*_tokenize, _current->view, _tail);
            // Reconstruct the tail:
            if (_current) {
                _tail = reconstruct_range(_tail, _current->skip, std::ranges::end(_tail));
            }
        }

        constexpr bool operator==(sentinel_type) const { return not static_cast<bool>(_current); }
        constexpr bool operator==(iterator other) const noexcept {
            return std::ranges::data(_tail) == std::ranges::data(other._tail);
        }
    };

    constexpr iterator begin() noexcept {
        return iterator{neo::borrow_text(_text), _get_next_token};
    }

    constexpr auto end() noexcept { return typename iterator::sentinel_type{}; }
};

template <text_range R, tokenizer_fn<borrow_text_t<R>> Tok>
explicit tokenizer(R&&, Tok&&) -> tokenizer<R, Tok>;

namespace tokenize_detail {

template <text_range R>
using text_line_split_tokenizer = neo::tokenizer<R, neo::line_splitter>;

struct get_token_view_fn {
    constexpr auto operator()(const auto& token) const noexcept { return token.view; }
};

template <text_range R>
constexpr auto iter_lines_impl(R&& text) noexcept {
    return tokenizer<R, line_splitter>{NEO_FWD(text), line_splitter{}}
    | std::views::transform(get_token_view_fn{});
}

}  // namespace tokenize_detail

template <text_range Text>
struct text_lines_view : decltype(tokenize_detail::iter_lines_impl(NEO_DECLVAL(Text))) {
    explicit text_lines_view(Text&& t) noexcept
        : text_lines_view::transform_view(tokenize_detail::iter_lines_impl(NEO_FWD(t))) {}
};

inline constexpr struct iter_lines_fn {
    template <text_range Text>
    constexpr auto operator()(Text&& text) const noexcept {
        return text_lines_view<Text>{NEO_FWD(text)};
    }
} iter_lines;

}  // namespace neo

namespace std::ranges {

/// A tokenizer is a borrowed range if its text type is a borrowed range.
template <typename R, typename Tok>
constexpr bool enable_borrowed_range<neo::tokenizer<R, Tok>> = std::ranges::borrowed_range<R>;

template <typename R, typename Tok>
constexpr bool enable_view<neo::tokenizer<R, Tok>> = std::ranges::view<R>and
               std::is_trivially_copyable_v<Tok>;

template <neo::text_range R>
constexpr bool enable_borrowed_range<neo::text_lines_view<R>> = std::ranges::borrowed_range<R>;

}  // namespace std::ranges
