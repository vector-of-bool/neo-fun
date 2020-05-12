#include "./assert.hpp"

#include <iostream>

namespace {

void _neo_default_assertion_handler(std::string_view               cond,
                                    std::string_view               message,
                                    neo::assertion_expression_list exprs) {
    std::cerr << "\nAssertion: The required condition [" << cond << "] evaluated to false.\n";
    std::cerr << "  Message: " << message << '\n';
    if (exprs.begin() != exprs.end()) {
        std::cerr << "The following context was provided:\n";
        for (const auto& e : exprs) {
            std::cerr << "  " << e.spelling() << " := ";
            e.write_into(std::cerr);
            std::cerr << '\n';
        }
    }
    std::abort();
}

neo::assertion_handler_fn_ptr _assertion_handler_fn = _neo_default_assertion_handler;

}  // namespace

void neo::fire_assertion(std::string_view               cond,
                         std::string_view               message,
                         neo::assertion_expression_list exprs) NEO_NOEXCEPT_ASSERTS {
    _assertion_handler_fn(cond, message, exprs);
    std::terminate();
}

void neo::set_assertion_handler(neo::assertion_handler_fn_ptr fn) noexcept {
    neo_assert_audit(fn != nullptr,
                     "A null function pointer was given to neo::set_assertion_handler");
    _assertion_handler_fn = fn;
}
