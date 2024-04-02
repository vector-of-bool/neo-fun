#pragma once

#include "./fwd.hpp"

#include <memory>

namespace neo {

template <typename T, typename... Args>
    requires requires(void* p, Args&&... args) { ::new (p) T(NEO_FWD(args)...); }
[[deprecated("Use std::construct_at")]] constexpr auto construct_at(T* addr,
                                                                    Args&&... args)  //
    noexcept(noexcept(::new((void*)0) T(NEO_FWD(args)...))) {
    return std::construct_at(addr, NEO_FWD(args)...);
}

template <typename T>
[[deprecated("Use std::destroy_at")]] constexpr void destroy_at(T* addr) {
    std::destroy_at(addr);
}

#define NEO_CONSTRUCT_AT ::neo::construct_at

// No compiler lets this "just work"
#define NEO_DESTROY_AT (::neo::destroy_at)

}  // namespace neo
