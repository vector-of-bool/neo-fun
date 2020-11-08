#include "./assert.hpp"

#include <iostream>

namespace {

/**
 * The default handler prints some (hopefully useful) diagnostic information.
 */
void _neo_default_assertion_handler(neo::assertion_info            info,
                                    neo::assertion_expression_list exprs) {
    std::cerr << "\n      HALT: CONTRACT VIOLATION"
                 "\n      ===== =================="
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
    std::cerr << "  Location: File \"" << info.loc.filename << "\", line " << info.loc.line << ",\n"
              << "            in [" << info.loc.pretty_func << "]\n";
    std::cerr << "   Message: " << info.message;
    if (exprs.begin() != exprs.end()) {
        auto expr_it = exprs.begin();
        std::cerr << "\n   Context: " << expr_it->spelling() << " := ";
        expr_it->write_into(std::cerr);
        ++expr_it;
        while (expr_it != exprs.end()) {
            std::cerr << "\n            " << expr_it->spelling() << " := ";
            expr_it->write_into(std::cerr);
            ++expr_it;
        }
    }
    std::cerr << '\n';
    neo::render_breadcrumbs(std::cerr);
    std::cerr << '\n';
    std::terminate();
}

neo::assertion_handler_fn_ptr _assertion_handler_fn = _neo_default_assertion_handler;

}  // namespace

void neo::detail::breadcrumb_base::_render(std::ostream&                  out,
                                           neo::assertion_expression_list exprs) const noexcept {
    out << "  --------- ------------------------------\n"
        << "  Location: File \"" << _loc.filename << "\", line " << _loc.line << ", \n"
        << "            in [" << _loc.pretty_func << "]\n";
    out << "      What: " << _msg << '\n';
    if (exprs.begin() != exprs.end()) {
        auto expr_it = exprs.begin();
        out << "   Context: " << expr_it->spelling() << " := ";
        expr_it->write_into(out);
        ++expr_it;
        while (expr_it != exprs.end()) {
            out << "\n            " << expr_it->spelling() << " := ";
            expr_it->write_into(out);
            ++expr_it;
        }
    }
    out << '\n';
}

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

void neo::detail::breadcrumb_base::render_all(std::ostream& out) noexcept {
    auto ptr = _top;
    while (ptr) {
        ptr->do_render(out);
        ptr = ptr->_previous;
    }
}

void neo::render_breadcrumbs(std::ostream& out) noexcept {
    neo::detail::breadcrumb_base::render_all(out);
}
