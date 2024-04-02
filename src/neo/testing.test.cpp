#include "./testing.hpp"
#include <neo/meta.hpp>

#include <concepts>

int main() {}

neo::testing::cx_test_case SimpleTest = [](auto check) consteval {
    check(true);
    check(not false);
};

neo::testing::cx_test_case_foreach<int, double, void, int> ForEachType
    = []<typename T>(auto check, neo::meta::tag<T>) consteval {
          check(std::same_as<T, int> or std::same_as<T, void> or std::same_as<T, double>);
      };
