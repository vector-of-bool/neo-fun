#pragma once

#include "./addressof.hpp"
#include "./object_box.hpp"

namespace neo {

/**
 * `arrow_proxy` presents a minimal amount of pointer-like interface around another
 * object. `arrow_proxy` should be used with CTAD rather than explicit template
 * arguments.
 */
template <typename T>
class arrow_proxy {
    NEO_NO_UNIQUE_ADDRESS object_box<T> _value;

public:
    explicit constexpr arrow_proxy(T&& t) noexcept
        : _value(static_cast<T&&>(t)) {}

    constexpr auto& operator*() noexcept { return _value.get(); }
    constexpr auto& operator*() const noexcept { return _value.get(); }

    constexpr auto operator->() noexcept { return NEO_ADDRESSOF(**this); }
    constexpr auto operator->() const noexcept { return NEO_ADDRESSOF(**this); }
};

template <typename T>
arrow_proxy(T&&) -> arrow_proxy<T>;

}  // namespace neo
