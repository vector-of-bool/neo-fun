#include "./optional.hpp"

#include "./assert.hpp"

#include <optional>

using namespace neo;

void opt_detail::throw_bad_optional() { throw std::bad_optional_access(); }
void opt_detail::terminate_bad_optional() {
    neo_assert(expects, false, "Attempted to dereference a disengaged optional object");
}
