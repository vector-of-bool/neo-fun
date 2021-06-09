#pragma once

#include "./pp.hpp"

namespace neo {

/*
 ######   #######  ##     ## ########  #### ##       ######## ########
##    ## ##     ## ###   ### ##     ##  ##  ##       ##       ##     ##
##       ##     ## #### #### ##     ##  ##  ##       ##       ##     ##
##       ##     ## ## ### ## ########   ##  ##       ######   ########
##       ##     ## ##     ## ##         ##  ##       ##       ##   ##
##    ## ##     ## ##     ## ##         ##  ##       ##       ##    ##
 ######   #######  ##     ## ##        #### ######## ######## ##     ##
*/

#define NEO_COMPILER_IS_MSVC 0
#define NEO_COMPILER_IS_CLANG 0
#define NEO_COMPILER_IS_APPLE_CLANG 0
#define NEO_COMPILER_IS_LLVM_CLANG 0
#define NEO_COMPILER_IS_GNU 0

// clang-format off
#if defined(_MSC_VER)
    #undef NEO_COMPILER_IS_MSVC
    #define NEO_COMPILER_IS_MSVC 1
#elif defined(__clang__)
    #undef NEO_COMPILER_IS_CLANG
    #define NEO_COMPILER_IS_CLANG 1
    #if defined(__apple_build_version__)
        #undef NEO_COMPILER_IS_APPLE_CLANG
        #define NEO_COMPILER_IS_APPLE_CLANG 1
    #else
        #undef NEO_COMPILER_IS_LLVM_CLANG
        #define NEO_COMPILER_IS_LLVM_CLANG 1
    #endif
#elif defined(__GNUC__)
    #undef NEO_COMPILER_IS_GNU
    #define NEO_COMPILER_IS_GNU 1
#endif
// clang-format on

#define NEO_COMPILER_IS_GNU_LIKE (NEO_COMPILER_IS_GNU || NEO_COMPILER_IS_CLANG)

enum class compiler_id_t {
    unknown,
    msvc,
    clang,
    gnu,
};

// clang-format off
constexpr inline compiler_id_t compiler_id =
    #if NEO_COMPILER_IS_MSVC
        compiler_id_t::msvc
    #elif NEO_COMPILER_IS_CLANG
        compiler_id_t::clang
    #elif NEO_COMPILER_IS_GNU
        compiler_id_t::gnu
    #else
        compiler_id_t::unknown
    #endif
    ;
// clang-format on

constexpr inline bool compiler_is_gnu_like =  //
    compiler_id == compiler_id_t::gnu ||      //
    compiler_id == compiler_id_t::clang;

constexpr inline bool compiler_is_msvc        = compiler_id == compiler_id_t::msvc;
constexpr inline bool compiler_is_gnu         = compiler_id == compiler_id_t::gnu;
constexpr inline bool compiler_is_clang       = compiler_id == compiler_id_t::clang;
constexpr inline bool compiler_is_apple_clang = NEO_COMPILER_IS_APPLE_CLANG;
constexpr inline bool compiler_is_llvm_clang  = NEO_COMPILER_IS_LLVM_CLANG;

/// do_repr for compiler_id_t
constexpr void do_repr(auto out, compiler_id_t const* value) noexcept {
    if constexpr (out.just_type) {
        out.append("neo::compiler_id_t");
    } else if constexpr (out.just_value) {
        using t = compiler_id_t;
        switch (*value) {
        case t::clang:
            out.append("clang");
            break;
        case t::gnu:
            out.append("gnu");
            break;
        case t::msvc:
            out.append("msvc");
            break;
        case t::unknown:
            out.append("unknown");
            break;
        }
    } else {
        out.append("{}::{}", out.template repr_type<compiler_id_t>(), out.repr_value(*value));
    }
}

/*
 #######   ######
##     ## ##    ##
##     ## ##
##     ##  ######
##     ##       ##
##     ## ##    ##
 #######   ######
*/

#define NEO_OS_IS_WINDOWS 0
#define NEO_OS_IS_LINUX 0
#define NEO_OS_IS_MACOS 0
#define NEO_OS_IS_FREEBSD 0
#define NEO_OS_IS_NETBSD 0
#define NEO_OS_IS_OPENBSD 0
#define NEO_OS_IS_DRAGONFLYBSD 0

// clang-format off
#if defined(_WIN32)
    #undef NEO_OS_IS_WINDOWS
    #define NEO_OS_IS_WINDOWS 1
#elif __linux__
    #undef NEO_OS_IS_LINUX
    #define NEO_OS_IS_LINUX 1
#elif defined(__APPLE__)
    #undef NEO_OS_IS_MACOS
    #define NEO_OS_IS_MACOS 1
#elif defined(__FreeBSD__)
    #undef NEO_OS_IS_FREEBSD
    #define NEO_OS_IS_FREEBSD 1
#elif defined(__NetBSD__)
    #undef NEO_OS_IS_NETBSD
    #define NEO_OS_IS_NETBSD 1
#elif defined(__OpenBSD__)
    #undef NEO_OS_IS_OPENBSD
    #define NEO_OS_IS_OPENBSD 1
#elif defined(__DragonFly__)
    #undef NEO_OS_IS_DRAGONFLYBSD
    #define NEO_OS_IS_DRAGONFLYBSD 1
#endif

#define NEO_OS_IS_BSD \
    (NEO_OS_IS_FREEBSD \
    || NEO_OS_IS_NETBSD \
    || NEO_OS_IS_OPENBSD \
    || NEO_OS_IS_DRAGONFLYBSD)
// clang-format on

#define NEO_OS_IS_UNIX_LIKE (NEO_OS_IS_LINUX || NEO_OS_IS_MACOS || NEO_OS_IS_BSD)

enum class operating_system_t {
    unknown,
    windows,
    linux,
    macos,
    freebsd,
    netbsd,
    openbsd,
    dragonflybsd,
};

constexpr inline operating_system_t operating_system =
#if NEO_OS_IS_WINDOWS
    operating_system_t::windows
#elif NEO_OS_IS_LINUX
    operating_system_t::linux
#elif NEO_OS_IS_MACOS
    operating_system_t::macos
#elif NEO_OS_IS_FREEBSD
    operating_system_t::freebsd
#elif NEO_OS_IS_NETBSD
    operating_system_t::netbsd
#elif NEO_OS_IS_OPENBSD
    operating_system_t::openbsd
#elif NEO_OS_IS_DRAGONFLYBSD
    operating_system_t::dragonflybsd
#else
    operating_system_t::unknown
#endif
    ;

constexpr inline bool os_is_windows   = NEO_OS_IS_WINDOWS;
constexpr inline bool os_is_linux     = NEO_OS_IS_LINUX;
constexpr inline bool os_is_macos     = NEO_OS_IS_MACOS;
constexpr inline bool os_is_bsd       = NEO_OS_IS_BSD;
constexpr inline bool os_is_unix_like = NEO_OS_IS_UNIX_LIKE;

/// do_repr for operating_system_t
constexpr void do_repr(auto out, operating_system_t const* value) noexcept {
    if constexpr (out.just_type) {
        out.append("neo::operating_system_t");
    } else if constexpr (out.just_value) {
        using t = operating_system_t;
        switch (*value) {
        case t::windows:
            out.append("windows");
            break;
        case t::linux:
            out.append("linux");
            break;
        case t::macos:
            out.append("macos");
            break;
        case t::freebsd:
            out.append("freebsd");
            break;
        case t::netbsd:
            out.append("netbsd");
            break;
        case t::dragonflybsd:
            out.append("dragonflybsd");
            break;
        case t::openbsd:
            out.append("openbsd");
            break;
        case t::unknown:
            out.append("unknown");
            break;
        }
    } else {
        out.append("{}::{}", out.template repr_type<operating_system_t>(), out.repr_value(*value));
    }
}

/// Emit a '_Pragma' for only MSVC
#define NEO_MSVC_PRAGMA(...) static_assert(true)
/// Emit a '_Pragma' for only GCC
#define NEO_GNU_PRAGMA(...) static_assert(true)
/// Emit a '_Pragma' for only Clang
#define NEO_CLANG_PRAGMA(...) static_assert(true)
/// Emit a '_Pragma' for GNU-like compilers (GCC and Clang)
#define NEO_GNU_LIKE_PRAGMA(...) static_assert(true)

#define NEO_PRAGMA_1(String) _Pragma(String) static_assert(true)
#define NEO_PRAGMA(...) NEO_PRAGMA_1(NEO_STR(__VA_ARGS__))

// clang-format off
#if NEO_COMPILER_IS_MSVC
    #undef NEO_MSVC_PRAGMA
    #define NEO_MSVC_PRAGMA NEO_PRAGMA
    #define NEO_PRAGMA_WARNING_PUSH() NEO_PRAGMA(warning(push))
    #define NEO_PRAGMA_WARNING_POP() NEO_PRAGMA(warning(pop))
#elif NEO_COMPILER_IS_GNU
    #undef NEO_GNU_PRAGMA
    #define NEO_GNU_PRAGMA NEO_PRAGMA
    #define NEO_PRAGMA_WARNING_PUSH() NEO_PRAGMA(GCC diagnostic push)
    #define NEO_PRAGMA_WARNING_POP() NEO_PRAGMA(GCC diagnostic pop)
#elif NEO_COMPILER_IS_CLANG
    #undef NEO_CLANG_PRAGMA
    #define NEO_CLANG_PRAGMA NEO_PRAGMA
    #define NEO_PRAGMA_WARNING_PUSH() NEO_PRAGMA(clang diagnostic push)
    #define NEO_PRAGMA_WARNING_POP() NEO_PRAGMA(clang diagnostic pop)
#else
    #define NEO_PRAGMA_WARNING_PUSH() static_assert(true)
    #define NEO_PRAGMA_WARNING_POP() static_assert(true)
#endif

#if NEO_COMPILER_IS_GNU_LIKE
    #define NEO_PRAGMA_MESSAGE(String) NEO_PRAGMA(message, String)
#elif NEO_COMPILER_IS_MSVC
    #define NEO_PRAGMA_MESSAGE(String) NEO_PRAGMA(message(String))
#else
    #define NEO_PRAGMA_MESSAGE(S) static_assert(true)
#endif

#if NEO_COMPILER_IS_GNU_LIKE
    #define NEO_PRAGMA_WARNING(String) NEO_PRAGMA(GCC warning String)
#else
    #define NEO_PRAGMA_WARNING(String) NEO_PRAGMA_MESSAGE("warning: " String)
#endif

#if NEO_COMPILER_IS_GNU_LIKE
    #undef NEO_GNU_LIKE_PRAGMA
    #define NEO_GNU_LIKE_PRAGMA NEO_PRAGMA
#endif
// clang-format on

}  // namespace neo
