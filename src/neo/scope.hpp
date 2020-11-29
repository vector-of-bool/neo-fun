#pragma once

#include "./fwd.hpp"
#include "./pp.hpp"

#include <type_traits>

namespace neo {

template <typename Func>
requires std::is_invocable_v<Func> class scope_exit {
    Func fn;

    scope_exit(const scope_exit&) = delete;

public:
    constexpr scope_exit(Func&& fn)
        : fn(NEO_FWD(fn)) {}

#if __cpp_constexpr_dynamic_alloc  // Introduced constexpr-destructors
    constexpr ~scope_exit() { fn(); }
#else
    ~scope_exit() { fn(); }
#endif
};

template <typename Func>
scope_exit(Func &&) -> scope_exit<Func>;

#define neo_defer                                                                                  \
    ::neo::scope_exit NEO_CONCAT_3(_neo_scope_exiter_, __LINE__, __COUNTER__)                      \
        = [&]() mutable -> void

}  // namespace neo
