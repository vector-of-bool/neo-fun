#pragma once

#include "./object_box.hpp"

#if defined(__bpt_header_check)
#include "./platform.hpp"
NEO_GNU_LIKE_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations");
NEO_MSVC_PRAGMA(warning(disable : 4996));
#endif

namespace neo {

template <typename T>
struct [[deprecated("Use object_box instead of assignable_box")]] assignable_box : object_box<T> {
    using assignable_box::object_box::object_box;
};

template <typename T>
explicit assignable_box(T&&) -> assignable_box<T>;

}  // namespace neo
