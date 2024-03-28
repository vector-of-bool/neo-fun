#pragma once

#include <neo/meta.hpp>

namespace neo::testing {

struct asserter {
    consteval void operator()(bool cond) const {
        if (not cond) {
            throw;
        }
    }
};

/**
 * @brief Declare a compile-time test case. Must be initialized with an invocable
 * that is `consteval` and accepts an `asserter` as an argument.
 */
struct [[maybe_unused]] cx_test_case {
    template <typename F>
    consteval cx_test_case(F fn) {
        static_cast<void>(fn(asserter{}));
    }
};

/**
 * @brief Declare a compile-time test case that executes the test function once
 * for each given type argument, passing a `meta::tag<T>` as the second argument for each function
 *
 * @tparam Ts The types to pass to the test case function.
 */
template <typename... Ts>
struct [[maybe_unused]] cx_test_case_foreach {
    template <typename F>
    consteval cx_test_case_foreach(F fn) {
        (static_cast<void>(fn(asserter{}, meta::tag_v<Ts>)), ...);
    }
};

}  // namespace neo::testing