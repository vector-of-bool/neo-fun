#pragma once

#include "./declval.hpp"
#include "./fwd.hpp"

namespace neo {

template <typename T>
constexpr void default_delete_fn(T* ptr) noexcept {
    delete ptr;
}

template <typename T>
class pimpl {
    T* _impl;
    void (*_del)(T*) noexcept(true);

public:
    // clang-format off

    constexpr explicit pimpl(auto&&... args)
        // Noexcept if construction is noexcept
        noexcept(noexcept(T(NEO_FWD(args)...)))
        // Require constructible:
        requires requires { T(NEO_FWD(args)...); }
        // Allocate a new T
        : _impl(new T(NEO_FWD(args)...))
        // Use the default deleting function
        , _del(&default_delete_fn<T>) {}

    constexpr explicit pimpl(void (del)(T*) noexcept, auto&&... args)
        // Noexcept if construction is noexcept
        noexcept(noexcept(T(NEO_FWD(args)...)))
        // Require constructible:
        requires requires { T(NEO_FWD(args)...); }
        // Allocate a new T
        : _impl(new T(NEO_FWD(args)...))
        // Use the provided deleting function
        , _del(del) {}

    constexpr pimpl(const pimpl& other)
        noexcept(noexcept(T(NEO_DECLVAL(const T&))))
        requires requires { T(NEO_DECLVAL(const T&)); }
        : _impl(new T(*other))
        , _del(other._del) {}

    constexpr pimpl(pimpl&& o) noexcept {
        _impl   = o._impl;
        _del    = o._del;
        o._impl = nullptr;
    }

    constexpr pimpl& operator=(const pimpl& other)
        noexcept(noexcept(NEO_DECLVAL(T) = NEO_DECLVAL(T)))
        requires requires { NEO_DECLVAL(T) = NEO_DECLVAL(T); }
        { **this = *other; return *this; }

    constexpr pimpl& operator=(pimpl&& o) noexcept {
        _impl   = o._impl;
        _del    = o._del;
        o._impl = nullptr;
    }

    // clang-format on

    constexpr ~pimpl() {
        if (_impl) {
            _del(_impl);
        }
    }

    constexpr T*       operator->() noexcept { return _impl; }
    constexpr const T* operator->() const noexcept { return _impl; }
    constexpr T&       operator*() noexcept { return *_impl; }
    constexpr const T& operator*() const noexcept { return *_impl; }
};

}  // namespace neo
