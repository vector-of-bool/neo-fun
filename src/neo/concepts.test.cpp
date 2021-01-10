// This just tests that we can compile with the concepts.hpp header

#include "./concepts.hpp"

#include "./test_concept.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace neo;

struct empty {};
struct irregular {
    irregular(int) {}
};

struct nontrivial {
    ~nontrivial() {}
};

struct immobile {
    immobile(const immobile&) = delete;
};

struct callable_thing {
    void operator()(int, int);
    void operator()(std::string);
    void operator()(std::string, std::string, int);
};

struct int_tester {
    bool operator()(int);
};

struct int_comparer {
    bool operator()(int, int);
};

struct empty_derived : empty {};

NEO_TEST_CONCEPT(same_as<int, int>);
NEO_TEST_CONCEPT(same_as<bool, bool>);
NEO_TEST_CONCEPT(same_as<empty, empty>);
NEO_TEST_CONCEPT(!same_as<empty, bool>);
NEO_TEST_CONCEPT(same_as<void, void>);

NEO_TEST_CONCEPT(convertible_to<int, float>);
NEO_TEST_CONCEPT(convertible_to<void, void>);
NEO_TEST_CONCEPT(convertible_to<const char*, std::string>);
NEO_TEST_CONCEPT(!convertible_to<bool, std::string>);

NEO_TEST_CONCEPT(derived_from<empty_derived, empty>);
NEO_TEST_CONCEPT(!derived_from<empty_derived, bool>);

NEO_TEST_CONCEPT(integral<int>);
NEO_TEST_CONCEPT(!integral<float>);
NEO_TEST_CONCEPT(integral<char>);

NEO_TEST_CONCEPT(signed_integral<int>);
NEO_TEST_CONCEPT(signed_integral<signed int>);
NEO_TEST_CONCEPT(signed_integral<signed>);
NEO_TEST_CONCEPT(signed_integral<signed char>);
NEO_TEST_CONCEPT(!signed_integral<unsigned>);
NEO_TEST_CONCEPT(!signed_integral<unsigned char>);

NEO_TEST_CONCEPT(destructible<int>);
NEO_TEST_CONCEPT(destructible<empty>);
NEO_TEST_CONCEPT(destructible<std::string>);

NEO_TEST_CONCEPT(constructible_from<float, int>);
NEO_TEST_CONCEPT(constructible_from<std::string, char, std::size_t>);
NEO_TEST_CONCEPT(!constructible_from<std::string, empty>);
NEO_TEST_CONCEPT(constructible_from<std::string>);

NEO_TEST_CONCEPT(default_initializable<std::string>);
NEO_TEST_CONCEPT(default_initializable<int>);
NEO_TEST_CONCEPT(!default_initializable<irregular>);

NEO_TEST_CONCEPT(move_constructible<int>);
NEO_TEST_CONCEPT(move_constructible<std::string>);
NEO_TEST_CONCEPT(move_constructible<std::unique_ptr<int>>);
NEO_TEST_CONCEPT(!move_constructible<immobile>);

NEO_TEST_CONCEPT(copy_constructible<int>);
NEO_TEST_CONCEPT(copy_constructible<std::string>);
NEO_TEST_CONCEPT(!copy_constructible<std::unique_ptr<int>>);
NEO_TEST_CONCEPT(!copy_constructible<immobile>);

NEO_TEST_CONCEPT(assignable_from<int&, int>);
NEO_TEST_CONCEPT(!assignable_from<int&, std::string>);
NEO_TEST_CONCEPT(assignable_from<std::string&, const char*>);

NEO_TEST_CONCEPT(movable<int>);
NEO_TEST_CONCEPT(movable<std::string>);
NEO_TEST_CONCEPT(!movable<immobile>);
NEO_TEST_CONCEPT(movable<std::unique_ptr<int>>);

NEO_TEST_CONCEPT(copyable<std::string>);
NEO_TEST_CONCEPT(copyable<int>);
NEO_TEST_CONCEPT(!copyable<immobile>);
NEO_TEST_CONCEPT(!copyable<std::unique_ptr<int>>);

NEO_TEST_CONCEPT(trivially_copyable<int>);
NEO_TEST_CONCEPT(!trivially_copyable<std::string>);
NEO_TEST_CONCEPT(trivially_copyable<empty>);

NEO_TEST_CONCEPT(trivial_type<int>);
NEO_TEST_CONCEPT(trivial_type<empty>);
NEO_TEST_CONCEPT(!trivial_type<nontrivial>);

NEO_TEST_CONCEPT(simple_boolean<int>);
NEO_TEST_CONCEPT(simple_boolean<char>);
NEO_TEST_CONCEPT(simple_boolean<bool>);
NEO_TEST_CONCEPT(!simple_boolean<std::string>);

NEO_TEST_CONCEPT(equality_comparable<std::string>);
NEO_TEST_CONCEPT(equality_comparable<int>);
NEO_TEST_CONCEPT(equality_comparable<bool>);
NEO_TEST_CONCEPT(!equality_comparable<empty>);

NEO_TEST_CONCEPT(totally_ordered<int>);
NEO_TEST_CONCEPT(totally_ordered<std::string>);
NEO_TEST_CONCEPT(!totally_ordered<empty>);

NEO_TEST_CONCEPT(totally_ordered_with<int, float>);
NEO_TEST_CONCEPT(totally_ordered_with<float, int>);
NEO_TEST_CONCEPT(totally_ordered_with<bool, int>);
NEO_TEST_CONCEPT(!totally_ordered_with<bool, std::string>);

NEO_TEST_CONCEPT(semiregular<int>);
NEO_TEST_CONCEPT(semiregular<std::string>);
NEO_TEST_CONCEPT(semiregular<std::vector<int>>);
NEO_TEST_CONCEPT(!semiregular<irregular>);
NEO_TEST_CONCEPT(!semiregular<std::unique_ptr<int>>);
NEO_TEST_CONCEPT(semiregular<std::shared_ptr<int>>);

NEO_TEST_CONCEPT(invocable<callable_thing, int, int>);
NEO_TEST_CONCEPT(invocable<callable_thing, std::string>);
NEO_TEST_CONCEPT(!invocable<callable_thing, int>);
NEO_TEST_CONCEPT(!invocable<callable_thing>);
NEO_TEST_CONCEPT(invocable<callable_thing, std::string, std::string, int>);
NEO_TEST_CONCEPT(!invocable<std::string, int>);

NEO_TEST_CONCEPT(predicate<int_tester, int>);
NEO_TEST_CONCEPT(!predicate<int_tester, std::string>);
NEO_TEST_CONCEPT(!predicate<std::string, int>);

NEO_TEST_CONCEPT(relation<int_comparer, int, int>);
NEO_TEST_CONCEPT(relation<int_comparer, int, float>);
NEO_TEST_CONCEPT(!relation<int_comparer, int, std::string>);

template <neo::trivially_copyable T>
std::true_type test_trivially_copyable_subsume(T);

template <neo::copyable T>
std::false_type test_trivially_copyable_subsume(T);

static_assert(decltype(test_trivially_copyable_subsume(5))::value);