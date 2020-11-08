#pragma once

#include <string_view>
#include <system_error>

#include "./ufmt.hpp"

namespace neo {

/**
 * @brief Assists in throwing exceptions from APIs that use an error_code output parameter
 *
 * This object can be passed in place of a 'std::error_code&' argument to any function.
 * The call operator on this object accepts a set of formattable arguments, and will
 * throw an exception if a non-zero `error_code&` was previously assigned.
 *
 * Example:
 *
 *  void connect(address addr, std::error_code& ec) noexcept;
 *  void connect(address addr) {
 *      error_code_thrower err;
 *      connect(err);
 *      err("Error while connecting to {}", addr);
 *  }
 *
 * @see error_code_auto_thrower
 */
class error_code_thrower {
    std::error_code _my_ec{};

public:
    constexpr operator std::error_code &() noexcept { return _my_ec; }

    template <typename Exc = std::system_error, typename... Args>
    void operator()(std::string_view format, Args&&... args) const requires requires {
        ufmt(format, args...);
    }
    {
        if (_my_ec) {
            throw Exc(_my_ec, ufmt(format, args...));
        }
    }
};

/**
 * @brief Like an error_code_auto_thrower, but automatically checks in its
 * destructor. This means that THE DESTRUCTOR MAY THROW in this class.
 *
 * Because the destructor of this class can throw, this object should not have
 * a lifetime that overlaps any non-noexcept code.
 *
 * The best way to use this class is as in-situ constructed arguments for
 * noexcept APIs that use an error_code output:
 *
 * ```
 *  void connect(address, error_code&) noexcept;
 *  void connect(address) {
 *      connect(addr, error_code_auto_thrower("Failure while connecting"));
 *  }
 * ```
 *
 * Also, the UDL _ec_throw can be used to construct one from a string literal,
 * for an ever more terse syntax:
 *
 * ```
 *  void connect(address, error_code&) noexcept;
 *  void connect(address) {
 *      connect(addr, "Failure while connecting"_ec_throw);
 *  }
 * ```
 */
class error_code_auto_thrower {
    error_code_thrower _err;
    std::string_view   _message;

public:
    explicit error_code_auto_thrower(std::string_view sv) noexcept
        : _message(sv) {}

    ~error_code_auto_thrower() noexcept(false) { _err(_message); }

    operator std::error_code &() && noexcept { return _err; }
};

inline namespace error_literals {
inline error_code_auto_thrower operator""_ec_throw(const char* msg, std::size_t len) noexcept {
    return error_code_auto_thrower(std::string_view(msg, len));
}
}  // namespace error_literals

}  // namespace neo
