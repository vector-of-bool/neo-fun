#pragma once

#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./iterator_facade.hpp"
#include "./object_box.hpp"
#include "./ranges.hpp"
#include "./reconstruct.hpp"
#include "./substring.hpp"
#include "./text_algo.hpp"
#include "./text_range.hpp"

#include <algorithm>
#include <optional>

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
    NEO_NO_UNIQUE_ADDRESS FindSplit _find_split;

    template <text_range View>
    constexpr std::optional<simple_token<substring_t<View>>>
    operator()(View&& prev, std::type_identity_t<View> const& remaining) const noexcept {
        if (std::ranges::empty(remaining)
            and std::ranges::end(prev) == std::ranges::end(remaining)) {
            // The previous token ends at the end of the input stream, which indicates that the full
            // text view does not end with a splitting sequence. Do not yield another token.
            return std::nullopt;
            // If we are at the end of the remaining input stream and there was a splitter, we will
            // end up returning an empty token for the end of the text.
        }

        // Find the next splitter:
        const auto split = _find_split(remaining);

        // Get the text leading up to the new split
        auto tok = substring(remaining, std::ranges::begin(remaining), split.begin());

        return simple_token<substring_t<View>>{tok, split.end()};
    }
};

template <typename C>
    requires predicate<C, char32_t>
struct charclass_splitter {
    NEO_NO_UNIQUE_ADDRESS object_box<C> _classifier{};

    template <text_range T>
    constexpr substring_t<T> operator()(const T& remaining) const noexcept {
        auto skip_start = std::ranges::find_if(remaining, [&](char32_t c) {
            return neo::invoke(_classifier.get(), c);
        });
        auto skip_end
            = std::ranges::find_if(skip_start, std::ranges::end(remaining), [&](char32_t c) {
                  return not neo::invoke(_classifier.get(), c);
              });
        return substring(remaining, skip_start, skip_end);
    }
};

struct is_whitespace_fn {
    constexpr auto operator()(char32_t c) const {
        return c == ' ' or c == '\n' or c == '\r' or c == '\t' or c == '\f';
    }
};

/**
 * @brief A string searcher that finds the next newline sequence in `view` (LF or CRLF)
 */
struct find_newline_fn {
    template <text_range T>
    constexpr substring_t<T> operator()(T&& view) const noexcept(ranges::nothrow_range<T>) {
        // A CRLF constant range to help compare against
        std::ranges::range_value_t<T> crlf_arr[3] = {'\r', '\n', 0};
        const auto                    crlf        = substring(crlf_arr);
        std::ranges::range_value_t<T> lf_arr[2]   = {'\n', 0};
        auto                          lf          = substring(lf_arr);
        // Advance as iterators
        auto sub = substring(view);
        while (not sub.empty() and not neo::starts_with(sub, lf)
               and not neo::starts_with(sub, crlf)) {
            sub = substring(sub, 1);
        }
        if (neo::starts_with(sub, crlf)) {
            return substring(sub, 0, 2);
        } else if (neo::starts_with(sub, lf)) {
            return substring(sub, 0, 1);
        }
        // "v" is empty:
        return sub;
    }
};

/**
 * @brief A simple tokenizer that splits text on whitespace.
 */
struct whitespace_splitter : simple_token_splitter<charclass_splitter<is_whitespace_fn>> {};
struct line_splitter : simple_token_splitter<find_newline_fn> {};

template <typename T, typename Text>
concept token_type =  //
    requires(T&& token) {
        { std::ranges::begin(token.view) } -> convertible_to<std::ranges::iterator_t<Text>>;
    };

template <typename T, typename Text>
concept token_result =      //
    neo::simple_boolean<T>  //
    && requires(T&& token) {
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
template <text_range R, tokenizer_fn<substring_t<view_text_t<R&>>> Tok>
class tokenizer {
    using View = view_text_t<R&>;

    /// The text that is being tokenized
    NEO_NO_UNIQUE_ADDRESS object_box<R> _text;
    /// The token-splitting function
    NEO_NO_UNIQUE_ADDRESS object_box<Tok> _get_next_token;

    constexpr static bool _is_nothrow
        = ranges::nothrow_range<R> and nothrow_invocable<Tok, View, View>;

public:
    using borrowed_text = substring_t<View>;

    constexpr tokenizer() = default;

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
    template <typename IterTok>
    struct _iterator : iterator_facade<_iterator<IterTok>> {
        NEO_NO_UNIQUE_ADDRESS borrowed_text _tail;
        NEO_NO_UNIQUE_ADDRESS token_result  _current;

        remove_reference_t<IterTok>* _tokenize = nullptr;

        _iterator() = default;

        struct sentinel_type {};

        explicit _iterator(borrowed_text v, IterTok& tok)
            : _tail(v)
            , _tokenize(NEO_ADDRESSOF(tok)) {
            _current = NEO_INVOKE(*_tokenize, borrowed_text(_tail.begin(), _tail.begin()), _tail);
            if (_current) {
                _tail = borrowed_text(_current->skip, _tail.end());
            }
        }

        constexpr const auto& dereference() const noexcept(_is_nothrow) {
            return _current.value().view;
        }

        constexpr void increment() noexcept(_is_nothrow) {
            _current = NEO_INVOKE(*_tokenize, substring(_current->view), _tail);
            // Reconstruct the tail:
            if (_current) {
                _tail = borrowed_text(_current->skip, _tail.end());
            }
        }

        constexpr bool operator==(sentinel_type) const { return not static_cast<bool>(_current); }
        constexpr bool operator==(_iterator other) const noexcept {
            return std::ranges::begin(_tail) == std::ranges::begin(other._tail);
        }
    };

    constexpr _iterator<Tok> begin() noexcept(_is_nothrow) {
        return _iterator<Tok>{neo::substring(neo::view_text(_text.get()), 0),
                              _get_next_token.get()};
    }

    constexpr _iterator<const Tok> begin() const noexcept(_is_nothrow) {
        return _iterator<const Tok>{neo::substring(neo::view_text(_text.get()), 0),
                                    _get_next_token.get()};
    }

    constexpr auto end() noexcept { return typename _iterator<Tok>::sentinel_type{}; }
    constexpr auto end() const noexcept { return typename _iterator<const Tok>::sentinel_type{}; }
};

template <text_range R, tokenizer_fn<substring_t<view_text_t<R>>> Tok>
explicit tokenizer(R&&, Tok&&) -> tokenizer<R, Tok>;

template <text_view Text>
struct text_lines_view : std::ranges::view_interface<text_lines_view<Text>> {
    using inner_view = tokenizer<Text, line_splitter>;
    NEO_NO_UNIQUE_ADDRESS inner_view _view;

    text_lines_view() = default;

    constexpr explicit text_lines_view(Text t) noexcept
        : _view(NEO_FWD(t)) {}

    constexpr auto begin() noexcept(neo::ranges::nothrow_range<inner_view>) {
        return _view.begin();
    }
    constexpr auto begin() const noexcept(neo::ranges::nothrow_range<inner_view>)
        requires std::ranges::range<const inner_view>
    {
        return _view.begin();
    }
    constexpr auto end() noexcept { return _view.end(); }
    constexpr auto end() const noexcept
        requires std::ranges::range<const inner_view>
    {
        return _view.end();
    }
};

template <viewable_text_range R>
explicit text_lines_view(R&&) -> text_lines_view<view_text_t<R>>;

inline constexpr struct iter_lines_fn {
    template <viewable_text_range Text>
    constexpr auto operator()(Text&& text) const noexcept(ranges::nothrow_range<Text>) {
        return text_lines_view{NEO_FWD(text)};
    }
} iter_lines;

}  // namespace neo

namespace std::ranges {

/// A tokenizer is a borrowed range if its text type is a borrowed range.
template <typename R, typename Tok>
constexpr bool enable_borrowed_range<neo::tokenizer<R, Tok>> = std::ranges::borrowed_range<R>;

template <typename R, typename Tok>
constexpr bool enable_view<neo::tokenizer<R, Tok>>
    = std::ranges::viewable_range<R> and neo::trivially_constructible<Tok, const Tok&>;

template <neo::text_range R>
constexpr bool enable_borrowed_range<neo::text_lines_view<R>> = std::ranges::borrowed_range<R>;

}  // namespace std::ranges
