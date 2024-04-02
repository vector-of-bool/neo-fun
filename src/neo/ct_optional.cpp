#include "./ct_optional.hpp"

#include "./assert.hpp"

void neo::_ct_optional_detail::terminate(const char* func) {
    neo_assert(expects, false, "Attempted to access a null compile-time optional value.", func);
}
