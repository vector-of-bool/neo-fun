#include "./variant_get.hpp"

#include <variant>

void neo::var_detail::throw_bad_variant_access() { throw std::bad_variant_access(); }
