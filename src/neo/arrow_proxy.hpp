#pragma once

#include "./addressof.hpp"
#include "./scalar_box.hpp"

namespace neo {

/**
 * `arrow_proxy` presents a minimal amount of pointer-like interface around another
 * object. `arrow_proxy` should be used with CTAD rather than explicit template
 * arguments.
 */
template <typename T>
class arrow_proxy {
    NEO_NO_UNIQUE_ADDRESS scalar_box<T> _value;

public:
    explicit constexpr arrow_proxy(T&& t) noexcept
        : _value(static_cast<T&&>(t)) {}

    constexpr auto& operator*() noexcept { return _value.get(); }
    constexpr auto& operator*() const noexcept { return _value.get(); }

    constexpr auto operator->() noexcept { return neo::addressof(**this); }
    constexpr auto operator->() const noexcept { return neo::addressof(**this); }
};

template <typename T>
arrow_proxy(T&&) -> arrow_proxy<T>;

}  // namespace neo
