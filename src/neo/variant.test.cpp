#include <neo/get.hpp>
#include <neo/testing.hpp>
#include <neo/variant.hpp>

#include <compare>
#include <memory>
#include <string>

#include <catch2/catch.hpp>

using namespace std::literals;
using neo::variant;

static_assert(neo::default_initializable<variant<int>>);
static_assert(neo::trivially_destructible<variant<int>>);
static_assert(neo::trivially_copyable<variant<int>>);
static_assert(neo::trivially_movable<variant<int>>);

static_assert(neo::regular<variant<int>>);
static_assert(neo::regular<variant<int, int>>);

static_assert(neo::default_initializable<variant<std::string>>);
static_assert(not neo::trivially_destructible<variant<std::string>>);
static_assert(neo::copyable<variant<std::string>>);
static_assert(not neo::trivially_copyable<variant<std::string>>);
static_assert(neo::movable<variant<std::string>>);
static_assert(not neo::trivially_movable<variant<std::string>>);

struct not_trivially_copyable {
    not_trivially_copyable(const not_trivially_copyable&) {}
    not_trivially_copyable(not_trivially_copyable&&) = default;
};

static_assert(neo::trivially_destructible<variant<not_trivially_copyable>>);
static_assert(not neo::trivially_copy_constructible<not_trivially_copyable>);
static_assert(neo::copy_constructible<variant<not_trivially_copyable>>);
static_assert(neo::move_constructible<variant<not_trivially_copyable>>);
static_assert(neo::trivially_move_constructible<variant<not_trivially_copyable>>);
static_assert(neo::trivially_move_constructible<neo::object_box<not_trivially_copyable>>);
static_assert(
    neo::trivially_move_constructible<variant<not_trivially_copyable, not_trivially_copyable>>);
static_assert(neo::move_constructible<variant<not_trivially_copyable, not_trivially_copyable>>);
static_assert(not neo::trivially_movable<variant<not_trivially_copyable>>);

static_assert(not neo::default_initializable<variant<int&>>);
static_assert(neo::trivially_destructible<variant<int&>>);
static_assert(neo::trivially_copyable<variant<int&>>);
static_assert(neo::trivially_movable<variant<int&>>);

static_assert(not neo::default_initializable<variant<const int&>>);
static_assert(neo::trivially_destructible<variant<const int&>>);
static_assert(neo::trivially_copyable<variant<const int&>>);
static_assert(neo::trivially_movable<variant<const int&>>);

static_assert(neo::default_initializable<variant<void>>);
static_assert(neo::trivially_destructible<variant<void>>);
static_assert(neo::trivially_copyable<variant<void>>);
static_assert(neo::trivially_movable<variant<void>>);

static_assert(neo::default_initializable<variant<const void>>);
static_assert(neo::trivially_destructible<variant<const void>>);
static_assert(neo::trivially_copyable<variant<const void>>);
static_assert(neo::trivially_movable<variant<const void>>);

static_assert(neo::equality_comparable<variant<int>>);
static_assert(neo::totally_ordered<variant<int>>);
static_assert(neo::totally_ordered<variant<int, double>>);

neo::testing::cx_test_case DefaultConstruction = [](auto check) consteval {
    variant<int> i;
    check(i.index() == 0);
    auto o = i.try_get<0>();
    check(!!o);
    check(*o == 0);
};

neo::testing::cx_test_case VoidDefault = [](auto check) consteval {
    variant<void> v;
    check(v.index() == 0);
    std::same_as<neo::optional<void>> auto o = v.try_get<0>();
    check(o.has_value());
    (void)o.value();
};

struct not_constexpr {
    not_constexpr() {}
};

neo::testing::cx_test_case ConstexprWithNonLiteralAlternatives = [](auto check) consteval {
    // XXX: Clang is missing CWG 2096: See llvm-project#40183
    variant<int, not_constexpr> v;
    check(v.index() == 0);
};

neo::testing::cx_test_case AmbiguousAlternativeConstruction = [](auto check) consteval {
    variant<int, int> v;
    check(v.index() == 0);
    v.emplace<1>(31);
    check(v.index() == 1);
    check(*v.try_get<1>() == 31);
    static_assert(neo::assignable_from<variant<int, std::string>&, int>);
    // Assignment would be ambiguous:
    static_assert(not neo::assignable_from<variant<int, int>&, int>);

    variant<int, int&> var_with_ref;
    // Choses the first altnerative, since the `int&` would require binding an rvalue
    // to an lvalue-reference:
    var_with_ref = 31;
    check(var_with_ref.index() == 0);
    int tmp = 42;
    // Ambiguous with an lvalue reference
    static_assert(not neo::assignable_from<variant<int, int&>, int&>);
    const int& cref = tmp;
    // Choses first alternative, since the `int const&` cannot convert to `int&`
    var_with_ref = cref;
    check(var_with_ref.index() == 0);
};

neo::testing::cx_test_case ConstructFromIndex = [](auto check) consteval {
    {
        variant<int> i{std::in_place_index<0>, 21};
        check(i.index() == 0);
        check(*i.try_get<0>() == 21);
    }
    {
        variant<void> with_void{std::in_place_index<0>};
        check(with_void.index() == 0);
        check(!!with_void.try_get<0>());
    }
    {
        variant<int, char> multiple{std::in_place_index<1>, 'h'};
        check(*multiple.try_get<1>() == 'h');
    }
    {
        int           i = 42;
        variant<int&> with_ref{std::in_place_index<0>, i};
        check(*with_ref.try_get<0>() == 42);
        *with_ref.try_get<0>() = 21;
        check(i == 21);

        int other = 71;
        with_ref  = other;
        check(i == 21);  // Did not modify i
        check(*with_ref.try_get<0>() == 71);
    }
};

template <int N>
struct item {
    constexpr ~item() {}
};

neo::testing::cx_test_case HugeVariants = [](auto) consteval {
    neo::variant<int> just_int [[maybe_unused]];
    neo::variant<item<1>,
                 item<2>,
                 item<3>,
                 item<4>,
                 item<5>,
                 item<6>,
                 item<7>,
                 item<8>,
                 item<9>,
                 item<10>>
        t1 [[maybe_unused]];

    neo::variant<item<1>, item<2>, item<3>, item<4>, item<5>, item<6>, item<7>, item<8>> t2
        [[maybe_unused]];

    neo::variant<item<0>,
                 item<1>,
                 item<2>,
                 item<3>,
                 item<4>,
                 item<5>,
                 item<6>,
                 item<7>,
                 item<8>,
                 item<9>,
                 item<10>,
                 item<11>,
                 item<12>,
                 item<13>,
                 item<14>,
                 item<15>,
                 item<16>,
                 item<17>,
                 item<18>,
                 item<19>,
                 item<20>,
                 item<21>,
                 item<22>,
                 item<23>,
                 item<24>,
                 item<25>,
                 item<26>,
                 item<27>,
                 item<28>,
                 item<29>,
                 item<30>,
                 item<31>,
                 item<32>,
                 item<33>,
                 item<34>,
                 item<35>,
                 item<36>,
                 item<37>,
                 item<38>,
                 item<39>,
                 item<40>,
                 item<41>,
                 item<42>,
                 item<43>,
                 item<44>,
                 item<45>,
                 item<46>,
                 item<47>,
                 item<48>,
                 item<49>,
                 item<50>,
                 item<51>,
                 item<52>,
                 item<53>,
                 item<54>,
                 item<55>,
                 item<56>,
                 item<57>,
                 item<58>,
                 item<59>,
                 item<60>,
                 item<61>,
                 item<62>,
                 item<63>,
                 item<64>,
                 item<65>,
                 item<66>,
                 item<67>,
                 item<68>,
                 item<69>,
                 item<70>,
                 item<71>,
                 item<72>,
                 item<73>,
                 item<74>,
                 item<75>,
                 item<76>,
                 item<77>,
                 item<78>,
                 item<79>,
                 item<80>,
                 item<81>>
              t3;
    item<24>* _24 [[maybe_unused]] = t3.try_get<24>();
    item<25>* _25 [[maybe_unused]] = t3.try_get<25>();
    item<33>* _33 [[maybe_unused]] = t3.try_get<33>();
    item<66>* _66 [[maybe_unused]] = t3.try_get<66>();
    item<78>* _78 [[maybe_unused]] = t3.try_get<78>();
    item<81>* _81 [[maybe_unused]] = t3.try_get<81>();

    item<6>* six [[maybe_unused]]   = t3.try_get<6>();
    item<8>* eight [[maybe_unused]] = t3.try_get<8>();
};

struct composite {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
};

TEST_CASE("Converting construct from a value") {
    neo::variant<std::string> s = "hello";
    CHECK(s.index() == 0);
    CHECK(s == "hello");
    CHECK(neo::get_as<std::string>(s) == "hello");
}

struct tracks_assignment {
    bool was_assigned_to = false;
    constexpr tracks_assignment() noexcept {}

    tracks_assignment(const tracks_assignment&) = default;

    constexpr tracks_assignment& operator=(tracks_assignment) noexcept {
        was_assigned_to = true;
        return *this;
    }
};

neo::testing::cx_test_case AssignThruBehavior = [](auto check) consteval {
    variant<int, tracks_assignment> v1 = 31;
    check(v1 == 31);
    variant<int, tracks_assignment> v2 = tracks_assignment{};
    // Do the first assignment, which changes the alt
    v1 = v2;
    check(v1.try_get<1>()->was_assigned_to == false);
    // Second assignment will assign-through
    v1 = v2;
    check(v1.try_get<1>()->was_assigned_to == true);

    int                    i    = 41;
    int                    i2   = 71;
    double                 d    = 3.14;
    variant<int&, double&> refs = d;
    check(refs.index() == 1);
    refs = i;
    check(refs.index() == 0);
    check(&*refs.try_get<0>() == &i);
    // Subsequent will use the assign-through logic, but it will only rebind the reference.
    // IT MUST NOT assign-through on reference itself.
    refs = i2;
    check(refs.index() == 0);
    check(&*refs.try_get<0>() == &i2);
    // Check that i and i2 are unchanged:
    check(i == 41);
    check(i2 == 71);

    variant<int, void> with_void;
    check(with_void.index() == 0);
    variant<int, void> as_void{std::in_place_index<1>};
    check(as_void.index() == 1);
    with_void = as_void;  // Switch from `int` to `void`
    check(with_void.index() == 1);
    with_void = 31;
    check(with_void.index() == 0);
    with_void = neo::unit{};  // Rebinds to `void`
    check(with_void.index() == 1);
};

neo::testing::cx_test_case VariantOrdering = [](auto check) consteval {
    variant<int, double, char> v1, v2, v3;
    check(v1 == v2);
    v1 = 900;
    check(v1 < 3.14);  // int is less than a double literal
    check(v1 > v2);
    check(v1 > v3);
    v2 = 3.14;       // Becomes `double`
    check(v2 > v1);  // The double alternative is "greater than" the int altnerative
    v3 = 'a';
    check(v3 > v2);
    check(v3 > v1);
};

TEST_CASE("Get Nth from neo::variant<>") {
    STATIC_REQUIRE(neo::can_get_nth<neo::variant<int>, 0>);
    STATIC_REQUIRE(neo::can_get_nth<neo::variant<int>&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<neo::variant<int> const&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<neo::variant<int>&&, 0>);
    STATIC_REQUIRE(neo::can_get_nth<neo::variant<int> const&&, 0>);
    STATIC_REQUIRE(not neo::can_get_nth<neo::variant<int>, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<neo::variant<int>&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<neo::variant<int> const&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<neo::variant<int>&&, 1>);
    STATIC_REQUIRE(not neo::can_get_nth<neo::variant<int> const&&, 1>);
}

TEST_CASE("Get T from neo::variant<>") {
    STATIC_REQUIRE(neo::can_get_as<neo::variant<int, double>, int>);
    STATIC_REQUIRE(neo::can_get_as<neo::variant<int, double>&, int>);
    STATIC_REQUIRE(neo::can_get_as<neo::variant<int, double> const&, int>);
    STATIC_REQUIRE(neo::can_get_as<neo::variant<int, double>&&, int>);
    STATIC_REQUIRE(neo::can_get_as<neo::variant<int, double> const&&, int>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>, char>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>&, char>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double> const&, char>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>&&, char>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double> const&&, char>);

    // The type T must be exactly an alternative, no cvr allowed:
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double> const&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double>&&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, double> const&&, int&>);

    // The type T must occur exactly once
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, int>, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, int>&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, int> const&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, int>&&, int&>);
    STATIC_REQUIRE(not neo::can_get_as<neo::variant<int, int> const&&, int&>);

    // Getting from a variant applies the cvref qualifiers:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int>, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int>&, int>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int> const&, int>, const int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int>&&, int>, int&&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int> const&&, int>, const int&&>);

    // Reference collapsing:
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int&>, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int&>&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int&> const&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int&>&&, int&>, int&>);
    STATIC_REQUIRE(std::same_as<neo::get_as_t<neo::variant<int&> const&&, int&>, int&>);
}

struct not_default_constructible {
    not_default_constructible(int);
};

TEST_CASE("Not-default-constructible") {
    STATIC_REQUIRE_FALSE(neo::default_initializable<neo::variant<not_default_constructible>>);
    // We are default-constructible only if the first alt is default constructible
    STATIC_REQUIRE(
        neo::default_initializable<neo::variant<std::string, not_default_constructible>>);
    STATIC_REQUIRE_FALSE(
        neo::default_initializable<neo::variant<not_default_constructible, std::string>>);

    neo::variant<not_default_constructible> ord = not_default_constructible{42};
}

template <typename T>
bool _static_checks() {
#define CHECK_SAME(Trait)                                                                          \
    static_assert(neo::Trait<T> == neo::Trait<neo::variant<T>>);                                   \
    static_assert(neo::Trait<T> == neo::Trait<neo::variant<T, T>>)
    CHECK_SAME(default_initializable);
    CHECK_SAME(copy_constructible);
    CHECK_SAME(move_constructible);
    CHECK_SAME(trivially_copy_constructible);
    CHECK_SAME(trivially_move_constructible);
    CHECK_SAME(trivially_copy_assignable);
    CHECK_SAME(trivially_move_assignable);
    CHECK_SAME(trivially_copyable);
    CHECK_SAME(trivially_movable);
    CHECK_SAME(trivially_destructible);
    return true;
}

TEST_CASE("Static checks") {
    _static_checks<int>();
    _static_checks<double>();
    _static_checks<std::string>();
    _static_checks<std::unique_ptr<int>>();
}

TEST_CASE("Triviallity") {
    neo::variant<void> v [[maybe_unused]];
    STATIC_REQUIRE(neo::trivially_destructible<neo::variant<void>>);
    STATIC_REQUIRE(neo::trivially_copyable<neo::variant<void>>);
    STATIC_REQUIRE(neo::trivially_copyable<neo::variant<int>>);
    STATIC_REQUIRE(neo::trivially_movable<neo::variant<int>>);
    STATIC_REQUIRE(neo::trivially_copy_constructible<neo::variant<int>>);
    STATIC_REQUIRE(neo::trivially_move_constructible<neo::variant<int>>);
    STATIC_REQUIRE(neo::trivially_copy_assignable<neo::variant<int>>);
    STATIC_REQUIRE(neo::trivially_move_assignable<neo::variant<int>>);

    STATIC_REQUIRE(not neo::trivially_copyable<neo::variant<std::string>>);
    STATIC_REQUIRE(not neo::trivially_copyable<neo::variant<std::string, void>>);
    STATIC_REQUIRE(not neo::trivially_movable<neo::variant<std::string>>);
    STATIC_REQUIRE(not neo::trivially_copy_constructible<neo::variant<std::string>>);
    STATIC_REQUIRE(not neo::trivially_move_constructible<neo::variant<std::string>>);
    STATIC_REQUIRE(not neo::trivially_copy_assignable<neo::variant<std::string>>);
    STATIC_REQUIRE(not neo::trivially_move_assignable<neo::variant<std::string>>);
}
