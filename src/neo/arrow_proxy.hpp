#pragma once

#include "./addressof.hpp"
#include "./ref_member.hpp"
#include "./unref.hpp"

namespace neo {

/**
 * `arrow_proxy` presents a minimal amount of pointer-like interface around another
 * object. `arrow_proxy` should be used with CTAD rather than explicit template
 * arguments.
 */
template <typename T>
class arrow_proxy {
    wrap_ref_member_t<T> _value;

public:
    explicit constexpr arrow_proxy(T&& t) noexcept
        : _value(static_cast<T&&>(t)) {}

    constexpr auto& operator*() noexcept { return unref(_value); }
    constexpr auto& operator*() const noexcept { return unref(_value); }

    constexpr auto operator->() noexcept { return neo::addressof(**this); }
    constexpr auto operator->() const noexcept { return neo::addressof(**this); }
};

template <typename T>
arrow_proxy(T&&) -> arrow_proxy<T>;

}  // namespace neo
