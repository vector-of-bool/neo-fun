#pragma once

#include <neo/fwd.hpp>
#include <neo/ref.hpp>

#include <memory>

namespace neo {

/**
 * `arrow_proxy` presents a minimal amount of pointer-like interface around another
 * object. `arrow_proxy` should be used with CTAD rather than explicit template
 * arguments.
 */
template <typename T>
class arrow_proxy {
    wrap_refs_t<T> _value;

public:
    explicit constexpr arrow_proxy(T&& t) noexcept
        : _value(NEO_FWD(t)) {}

    constexpr auto& operator*() noexcept { return unref(_value); }
    constexpr auto& operator*() const noexcept { return unref(_value); }

    constexpr auto operator->() noexcept { return std::addressof(**this); }
    constexpr auto operator->() const noexcept { return std::addressof(**this); }
};

template <typename T>
arrow_proxy(T &&) -> arrow_proxy<T>;

}  // namespace neo
