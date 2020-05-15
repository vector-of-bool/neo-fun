#include "./assert.hpp"

#include <iostream>

namespace {

void _neo_default_assertion_handler(neo::assertion_info            info,
                                    neo::assertion_expression_list exprs) {
    std::cerr << "\n      HALT: CONTRACT VIOLATION"
                 "\n      ----- ------------------"
                 "\n      What: ";
    switch (info.kind) {
    case neo::assertion_kind::ensures:
        std::cerr << "An API was unable to fulfill its postconditions.\n";
        break;
    case neo::assertion_kind::invariant:
        std::cerr << "A class/library/function invariant was broken.\n";
        break;
    case neo::assertion_kind::expects:
        std::cerr << "A caller violated the preconditions of an API.\n";
        break;
    }
    std::cerr << "  Expected: " << info.expression_spelling << "\n";
    std::cerr << "  Location: File \"" << info.filepath << "\", line " << info.file_line
              << ",\n            in " << info.func_name << '\n';
    std::cerr << "   Message: " << info.message << '\n';
    if (exprs.begin() != exprs.end()) {
        auto expr_it = exprs.begin();
        std::cerr << "   Context: " << expr_it->spelling() << " := ";
        expr_it->write_into(std::cerr);
        ++expr_it;
        while (expr_it != exprs.end()) {
            std::cerr << "\n            " << expr_it->spelling() << " := ";
            expr_it->write_into(std::cerr);
            ++expr_it;
        }
    }
    std::cerr << "\n\n";
    std::terminate();
}

neo::assertion_handler_fn_ptr _assertion_handler_fn = _neo_default_assertion_handler;

}  // namespace

void neo::fire_assertion(neo::assertion_info            info,
                         neo::assertion_expression_list exprs) NEO_NOEXCEPT_ASSERTS {
    _assertion_handler_fn(info, exprs);
    std::terminate();
}

void neo::set_assertion_handler(neo::assertion_handler_fn_ptr fn) noexcept {
    neo_assert_always(expects,
                      fn != nullptr,
                      "A null function pointer was given to neo::set_assertion_handler");
    _assertion_handler_fn = fn;
}
