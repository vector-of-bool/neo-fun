#include "./get.hpp"

#include <catch2/catch.hpp>

#include <array>
#include <concepts>
#include <tuple>
#include <utility>

static_assert(neo::_get::has_nth_type<std::variant<int>, 0>);
static_assert(not neo::_get::has_nth_type<std::variant<int>, 1>);
static_assert(neo::_get::has_nth_type<std::tuple<int>, 0>);
static_assert(not neo::_get::has_nth_type<std::tuple<int>, 1>);
static_assert(not neo::_get::has_nth_type<std::vector<int>, 0>);
static_assert(not neo::_get::has_nth_type<std::vector<int>, 1>);

static_assert(neo::_get::has_free_get_if_nth<std::variant<int>, 0>);
static_assert(not neo::_get::has_free_get_if_nth<std::variant<int>, 1>);

static_assert(std::same_as<neo::try_get_nth_t<std::variant<int>, 0>, int*>);
static_assert(std::same_as<neo::try_get_nth_t<std::variant<int> const, 0>, int const*>);
static_assert(std::same_as<neo::try_get_nth_t<std::variant<int> const&, 0>, int const*>);
static_assert(std::same_as<neo::try_get_nth_t<std::variant<int>&&, 0>, int*>);
static_assert(std::same_as<neo::get_nth_t<std::variant<int>&&, 0>, int&&>);
static_assert(std::same_as<neo::get_nth_t<std::variant<int>&, 0>, int&>);
static_assert(std::same_as<neo::get_nth_t<std::variant<int> const&, 0>, const int&>);
static_assert(std::same_as<neo::get_nth_t<std::variant<int>, 0>, int&&>);

static_assert(std::same_as<neo::try_get_as_t<std::variant<int>, int>, int*>);
static_assert(std::same_as<neo::try_get_as_t<std::variant<int> const, int>, int const*>);
static_assert(std::same_as<neo::try_get_as_t<std::variant<int> const&, int>, int const*>);
static_assert(std::same_as<neo::try_get_as_t<std::variant<int>&&, int>, int*>);
static_assert(std::same_as<neo::get_as_t<std::variant<int>&&, int>, int&&>);
static_assert(std::same_as<neo::get_as_t<std::variant<int>&, int>, int&>);
static_assert(std::same_as<neo::get_as_t<std::variant<int> const&, int>, const int&>);
static_assert(std::same_as<neo::get_as_t<std::variant<int>, int>, int&&>);

static_assert(std::same_as<neo::get_nth_t<std::tuple<int>&&, 0>, int&&>);
static_assert(std::same_as<neo::get_nth_t<std::tuple<int>&, 0>, int&>);
static_assert(std::same_as<neo::get_nth_t<std::tuple<int> const&, 0>, const int&>);
static_assert(std::same_as<neo::get_nth_t<std::tuple<int>, 0>, int&&>);
static_assert(std::same_as<neo::get_as_t<std::tuple<int>&&, int>, int&&>);
static_assert(std::same_as<neo::get_as_t<std::tuple<int>&, int>, int&>);
static_assert(std::same_as<neo::get_as_t<std::tuple<int> const&, int>, const int&>);
static_assert(std::same_as<neo::get_as_t<std::tuple<int>, int>, int&&>);

TEST_CASE("Get Nth from std::tuple<>") {
    STATIC_REQUIRE(neo::can_get_nth<std::tuple<int>, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::tuple<int>&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::tuple<int> const&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::tuple<int>&&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::tuple<int> const&&, 0>);
    STATIC_REQUIRE(not neo::can_get_nth<std::tuple<int>, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::tuple<int>&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::tuple<int> const&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::tuple<int>&&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::tuple<int> const&&, 1>);

    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<int>, 0>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<int&>, 0>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<int&> const&, 0>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<const int&> const&, 0>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<const int&>&, 0>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<const int&>&&, 0>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<int&&>&&, 0>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::tuple<int&&>&, 0>, int&>);
}

TEST_CASE("Can get T from a std::tuple<>") {
    STATIC_REQUIRE(neo::can_get_as<std::tuple<int>, int>);
    STATIC_REQUIRE(neo::can_get_as<std::tuple<int>&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::tuple<int> const&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::tuple<int>&&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::tuple<int> const&&, int>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int> const&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>&&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int> const&&, char>);

    // The type T must be exactly an alternative, not cvr allowed:
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int> const&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int>&&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::tuple<int> const&&, int&>);

    // Getting from a tuple applies the cvref qualifiers:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int>, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int>&, int>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int> const&, int>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int>&&, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int> const&&, int>, const int&&>);

    // Reference collapsing:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int&>, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int&>&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int&> const&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int&>&&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::tuple<int&> const&&, int&>, int&>);
}

TEST_CASE("Get Nth from std::variant<>") {
    STATIC_REQUIRE(neo::can_get_nth<std::variant<int>, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::variant<int>&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::variant<int> const&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::variant<int>&&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::variant<int> const&&, 0>);
    STATIC_REQUIRE(not neo::can_get_nth<std::variant<int>, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::variant<int>&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::variant<int> const&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::variant<int>&&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::variant<int> const&&, 1>);
}

TEST_CASE("Get T from std::variant<>") {
    STATIC_REQUIRE(neo::can_get_as<std::variant<int>, int>);
    STATIC_REQUIRE(neo::can_get_as<std::variant<int>&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::variant<int> const&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::variant<int>&&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::variant<int> const&&, int>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int> const&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>&&, char>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int> const&&, char>);

    // The type T must be exactly an alternative, no cvr allowed:
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int> const&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int>&&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::variant<int> const&&, int&>);

    // Getting from a variant applies the cvref qualifiers:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::variant<int>, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::variant<int>&, int>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::variant<int> const&, int>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::variant<int>&&, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::variant<int> const&&, int>, const int&&>);
}

TEST_CASE("Get Nth from std::pair") {
    STATIC_REQUIRE(neo::can_get_nth<std::pair<int, int>, 0>);
    STATIC_REQUIRE(neo::can_get_nth<std::pair<int, int>, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<std::pair<int, int>, 2>);

    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, int>, 1>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, int&>, 1>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, int&> const&, 1>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, const int&> const&, 1>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, const int&>&, 1>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, const int&>&&, 1>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, int&&>&&, 1>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_nth_t<std::pair<int, int&&>&, 1>, int&>);
}

TEST_CASE("Can get T from a std::pair<>") {
    STATIC_REQUIRE(neo::can_get_as<std::pair<int, char>, int>);
    STATIC_REQUIRE(neo::can_get_as<std::pair<int, char>&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::pair<int, char> const&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::pair<int, char>&&, int>);
    STATIC_REQUIRE(neo::can_get_as<std::pair<int, char> const&&, int>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>, double>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>&, double>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char> const&, double>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>&&, double>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char> const&&, double>);

    // The type T must be exactly an alternative, no cvr allowed:
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char> const&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char>&&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<std::pair<int, char> const&&, int&>);

    // Getting from a pair applies the cvref qualifiers:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int, char>, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int, char>&, int>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int, char> const&, int>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int, char>&&, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int, char> const&&, int>, const int&&>);

    // Reference collapsing:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int&, char>, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int&, char>&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int&, char> const&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int&, char>&&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<std::pair<int&, char> const&&, int&>, int&>);
}

TEST_CASE("Get from std::array") {
    using array = std::array<int, 4>;
    STATIC_REQUIRE(neo::can_get_nth<array, 0>);
    STATIC_REQUIRE(neo::can_get_nth<array, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<array, 21>);
}

struct invalid_getter {
    template <std::size_t>
    using nth_type = int;

    template <std::size_t N>
    constexpr int const& get() & noexcept;
    template <std::size_t N>
    constexpr int const& get() const& noexcept;

    template <std::size_t N>
    constexpr int const& get() && noexcept;
    template <std::size_t N>
    constexpr int const& get() const&& noexcept;
};

struct invalid_ref_getter {
    template <std::size_t>
    using nth_type = int&;

    // All bad: The get should always return `int&`
    template <std::size_t N>
    constexpr int const& get() & noexcept;
    template <std::size_t N>
    constexpr int const& get() const& noexcept;
    template <std::size_t N>
    constexpr int const& get() && noexcept;
    template <std::size_t N>
    constexpr int const& get() const&& noexcept;
};

struct invalid_ref_getter_v2 {
    template <std::size_t>
    using nth_type = int&;

    // Okay: The getters return ref-to-mutable
    template <std::size_t N>
    constexpr int& get() & noexcept;
    template <std::size_t N>
    constexpr int& get() const& noexcept;
    // Bad: We should not forward rvalue-refs
    template <std::size_t N>
    constexpr int&& get() && noexcept;
    template <std::size_t N>
    constexpr int&& get() const&& noexcept;
};

TEST_CASE("Reject not-tuple-like types") {
    // Bad: The non-const getter returns a ref-to-const but the declared type is not `const&`
    STATIC_REQUIRE_FALSE(neo::can_get_nth<invalid_getter&, 0>);
    // Okay: The const `get` overload does the correct thing
    STATIC_REQUIRE(neo::can_get_nth<const invalid_getter&, 0>);

    // Bad: The rvalue-getter returns an lvalue reference
    STATIC_REQUIRE_FALSE(neo::can_get_nth<invalid_getter&&, 0>);
    // Bad: The const&& `get` overload does nto return an value-ref-to-const
    STATIC_REQUIRE_FALSE(neo::can_get_nth<const invalid_getter&&, 0>);

    STATIC_REQUIRE_FALSE(neo::can_get_nth<invalid_ref_getter&, 0>);
    STATIC_REQUIRE_FALSE(neo::can_get_nth<const invalid_ref_getter&, 0>);
    STATIC_REQUIRE_FALSE(neo::can_get_nth<invalid_ref_getter&&, 0>);
    STATIC_REQUIRE_FALSE(neo::can_get_nth<const invalid_ref_getter&&, 0>);

    STATIC_REQUIRE(neo::can_get_nth<invalid_ref_getter_v2&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<const invalid_ref_getter_v2&, 0>);
    STATIC_REQUIRE_FALSE(neo::can_get_nth<invalid_ref_getter_v2&&, 0>);
    STATIC_REQUIRE_FALSE(neo::can_get_nth<const invalid_ref_getter_v2&&, 0>);
}
