#pragma once

#include "./fwd.hpp"

#include <memory>
#include <new>

namespace neo {

template <typename T, typename... Args>
    requires requires(void* p, Args&&... args) { ::new (p) T(NEO_FWD(args)...); }
constexpr auto construct_at(T* addr,
                            Args&&... args)  //
    noexcept(noexcept(::new((void*)0) T(NEO_FWD(args)...))) {
    return std::construct_at(addr, NEO_FWD(args)...);
}

template <typename T>
constexpr void destroy_at(T* addr) {
    addr->~T();
}

#if defined __GNUC__ || defined _MSC_VER
// GCC, Clang, and MSVC all support constexpr placement-new, so we'll just expand a macro for it
#define NEO_CONSTRUCT_AT(Address, ...) (::std::construct_at(Address __VA_OPT__(, ) __VA_ARGS__))
#else
#define NEO_CONSTRUCT_AT ::neo::construct_at
#endif
// No compiler lets this "just work"
#define NEO_DESTROY_AT (::neo::destroy_at)

}  // namespace neo
