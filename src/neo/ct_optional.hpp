#pragma once

#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./like.hpp"
#include "./object_box.hpp"
#include "./unit.hpp"

namespace neo {

namespace _ct_optional_detail {

[[noreturn]] void terminate(const char*);

}  // namespace _ct_optional_detail

/**
 * @brief A compile-time optional value.
 *
 * Unlike a regular std::optional, a ct_optional either does or does not hold a
 * value, and this is fixed at compile-time.
 *
 * @tparam T
 * @tparam HasValue
 */
template <typename T, bool HasValue>
class ct_optional {
    using Stored = conditional_t<HasValue, object_box<T>, unit>;

    NEO_NO_UNIQUE_ADDRESS Stored _box;

    template <typename S>
    constexpr forward_like_t<S, T> _get(S&& self) noexcept {
        if constexpr (HasValue) {
            return NEO_FWD(self)._box.get();
        } else {
            _ct_optional_detail::terminate(__func__);
        }
    }

    struct emplace {};

    template <typename... Args>
        requires HasValue                           //
    constexpr ct_optional(emplace, Args&&... args)  //
        noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : _box(T(NEO_FWD(args)...)) {}

    template <typename... Args>
    constexpr ct_optional(emplace, Args&&...) noexcept {}

public:
    using has_value_tag = std::bool_constant<HasValue>;

    constexpr ct_optional()
        requires default_initializable<T>
    = default;

    template <explicit_convertible_to<T> U>
    explicit(not convertible_to<U, T>)  //
        constexpr ct_optional(U&& arg) noexcept(nothrow_constructible_from<T, U>)
        : ct_optional(emplace{}, NEO_FWD(arg)) {}

    template <typename... Args>
    constexpr explicit ct_optional(std::in_place_t, Args&&... args)  //
        noexcept(nothrow_constructible_from<T, Args&&...>)
        : ct_optional(emplace{}, NEO_FWD(args)...) {}

    constexpr ct_optional(ct_optional const&)  //
        requires copy_constructible<T>
    = default;

    constexpr ct_optional(ct_optional&&)  //
        requires move_constructible<T>
    = default;

    constexpr ct_optional& operator=(const ct_optional&)  //
        requires copy_assignable<T>
    = default;

    constexpr ct_optional& operator=(ct_optional&&)  //
        requires move_assignable<T>
    = default;

    constexpr bool has_value() const noexcept { return HasValue; }

    constexpr T&        operator*() & noexcept { return _get(*this); }
    constexpr const T&  operator*() const& noexcept { return _get(*this); }
    constexpr T&&       operator*() && noexcept { return _get(NEO_MOVE(*this)); }
    constexpr const T&& operator*() const&& noexcept { return _get(NEO_MOVE(*this)); }

    constexpr auto operator->() noexcept { return neo::addressof(**this); }
    constexpr auto operator->() const noexcept { return neo::addressof(**this); }
};

}  // namespace neo
