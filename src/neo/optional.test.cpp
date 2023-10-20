#include "./optional.hpp"

#include "./archetypes.hpp"
#include "./platform.hpp"
#include "./repr.hpp"
#include "./test_concept.hpp"
#include "neo/concepts.hpp"
#include "neo/type_traits.hpp"

#include <catch2/catch.hpp>
#include <compare>
#include <type_traits>

NEO_GNU_LIKE_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations");

namespace arch = neo::arch;

struct trivial {
    int i;
};

struct trivial_default_constructible {
    int i;
};

struct trivial_destructible {
    int i = 0;
};

struct nontrivial {
    int i = 0;
    ~nontrivial() {}
};

struct not_default_constructible {
    int a;
    constexpr explicit not_default_constructible(int a_)
        : a(a_) {}
};

// Optional traits:

static_assert(not neo::empty_type<neo::optional<trivial>>);
static_assert(not neo::trivial_type<neo::optional<trivial_default_constructible>>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible>&&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible>&&>);
static_assert(neo::trivially_copyable<neo::optional<not_default_constructible>>);

static_assert(neo::trivially_copyable<neo::optional<trivial_default_constructible>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_copy_constructor>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_copy_assignment>>);
static_assert(not neo::copyable<neo::optional<arch::nontrivial_move_constructor>>);
static_assert(not neo::copyable<neo::optional<arch::nontrivial_move_assignment>>);
static_assert(neo::movable<neo::optional<arch::nontrivial_move_constructor>>);
static_assert(neo::movable<neo::optional<arch::nontrivial_move_assignment>>);

static_assert(neo::trivially_copyable<neo::optional<trivial_default_constructible&>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_copy_constructor&>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_copy_assignment&>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_move_constructor&>>);
static_assert(neo::copyable<neo::optional<arch::nontrivial_move_assignment&>>);

static_assert(neo::trivially_movable<neo::optional<arch::trivially_movable>>);
static_assert(neo::trivially_movable<neo::optional<arch::trivially_movable>>);

static_assert(neo::trivially_movable<neo::optional<arch::trivially_movable&>>);
static_assert(neo::trivially_movable<neo::optional<arch::trivially_movable&>>);
static_assert(neo::movable<neo::optional<arch::nontrivial_move_constructor&>>);
static_assert(neo::movable<neo::optional<arch::nontrivial_move_assignment&>>);

static_assert(not neo::trivial_type<neo::optional<trivial_default_constructible>>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::optional<trivial_default_constructible>&,
                                        neo::optional<trivial_default_constructible>&&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible>&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible> const&>);
static_assert(neo::trivially_assignable<neo::optional<not_default_constructible>&,
                                        neo::optional<not_default_constructible>&&>);
static_assert(neo::trivially_copyable<neo::optional<not_default_constructible>>);

// static_assert(not neo::copy_constructible<neo::optional<arch::deleted_copy_constructor>>);

// The no_unique_address makes this just a single boolean:
// XXX: Disabled: MSVC doesn't seem to emit a single boolean?
// static_assert(sizeof(neo::optional<neo::unit>) == 1);
static_assert(sizeof(neo::optional<neo::unit&>) == sizeof(neo::unit*));

#if NEO_NANO_OPT_SUPPORTS_TRIVIAL_DESTRUCTIBILITY
NEO_TEST_CONCEPT(std::is_trivially_destructible_v<neo::nano_opt_storage<trivial>>);
NEO_TEST_CONCEPT(std::is_trivially_copy_constructible_v<neo::nano_opt_storage<trivial>>);
#endif

/// XXX: Bug in language/compilers sees a non-trivial destructor as invalidating
/// trivial_default_constructible. As such, these checks will fail:
// NEO_TEST_CONCEPT(std::is_trivially_default_constructible_v<trivial_default_constructible>);
// NEO_TEST_CONCEPT(std::is_trivially_copy_constructible_v<
//                  neo::nano_opt_storage<trivial_default_constructible>>);
/// Refer: https://cplusplus.github.io/LWG/issue2116 and
/// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51452

NEO_GNU_LIKE_PRAGMA(GCC diagnostic ignored "-Wdeprecated");

NEO_TEST_CONCEPT(!std::is_trivially_copy_constructible_v<neo::nano_opt_storage<nontrivial>>);
NEO_TEST_CONCEPT(!std::is_trivially_destructible_v<neo::nano_opt_storage<nontrivial>>);

// Check that nano_opt is constexpr-able
constexpr neo::nano_opt<int> get_const_opt() {
    auto ret = neo::nano_opt<int>(std::in_place, 41);
    ++ret.get();
    return ret;
}

constexpr neo::nano_opt<int> get_const_opt_2() {
    neo::nano_opt<int> ret;
    ret = {33};
    return ret;
}

TEST_CASE("Constexpr nano_opt") {
    constexpr auto opt = get_const_opt();
    static_assert(opt.get() == 42);

    constexpr auto opt_2 = get_const_opt_2();
    CHECK(opt_2.get() == 33);

    CHECK(neo::repr(opt).string() == "[neo::nano_opt<int32> [42]]");
    CHECK(neo::repr(opt_2).string() == "[neo::nano_opt<int32> [33]]");

    neo::nano_opt<int> null;
    CHECK(!null.has_value());
    CHECK(neo::repr(null).string() == "[neo::nano_opt<int32> nullopt]");
}

TEST_CASE("neo::optional") {
    // Constexpr ok?
    constexpr auto opt_null [[maybe_unused]] = neo::optional<int>{};
    using opt_int                            = neo::optional<int>;

    // Copy construct ok
    opt_int i1 [[maybe_unused]] = 12;
}

template <typename T>
void full_use(neo::optional<T> o) {
    o = neo::optional<T>(o);
    o = NEO_MOVE(o);
    o.has_value();
    o.emplace();
    CHECKED_IF(o.has_value()) {
        *o;
        CHECK_NOTHROW(o.value());
    }
    o.reset();
    o.reset();
    CHECK_FALSE(o.has_value());
    o.emplace();
    o.emplace();
    CHECK(o.has_value());
    o = {};
    CHECK_FALSE(o.has_value());
    o = neo::optional<T>();
    CHECK_FALSE(o);
    auto p = o;
    o.swap(p);
    auto dup  = o;
    auto dup2 = NEO_MOVE(dup);
    dup       = dup2;
    dup2      = NEO_MOVE(dup);
}

TEST_CASE("Optional of int") {
    constexpr auto x = neo::optional<int>();
    full_use(x);
    x <=> x;
}

TEST_CASE("Optional of void") {
    constexpr auto x = neo::optional<void>();
    full_use(x);
}

TEST_CASE("Optional of nontrivial") {
    auto x = neo::optional<std::string>("Howdy neighbor");
    full_use(x);
    CHECK((x <=> x) == std::strong_ordering::equal);
    CHECK(std::hash<neo::optional<std::string>>()(x) != 0);
}

TEST_CASE("Optional and_then") {
    auto x = neo::optional(12);
    CHECK(*x.and_then([](auto x) { return neo::optional(x + 2); }) == 14);
    CHECK(*x.transform([](auto x) { return x + 2; }) == 14);
}

TEST_CASE("Optional of reference") {
    std::string s          = "Hello, reference";
    auto        opt_string = neo::optional<std::string&>(s);
    // full_use(x);
    CHECK((opt_string <=> opt_string) == std::strong_ordering::equal);
    CHECK(opt_string == s);
    CHECK(opt_string == "Hello, reference");
    CHECK(opt_string != std::nullopt);
    CHECK(opt_string > std::nullopt);
    CHECK(std::hash<neo::optional<std::string&>>()(opt_string) != 0);
    // Compare two optionals of different types:
    auto opt_sv = neo::optional(std::string_view("Hello, reference"));
    CHECK(opt_sv == opt_string);

    std::string other = "Other string";
    // Rebind:
    opt_string = other;
    CHECK(opt_sv != opt_string);
    CHECK(*opt_string == "Other string");
}

struct never_defined;

TEST_CASE("Optional of incomplete reference") {
    neo::optional<never_defined&> oref;
    CHECK_FALSE(oref.has_value());
}

// XXX: MSVC has trouble with our designated-init trickery below?
#if !NEO_COMPILER(MSVC)

#define DECL_CHECKER(Concept, ...)                                                                 \
    template <bool B>                                                                              \
    struct check_##Concept {                                                                       \
        template <typename T>                                                                      \
            requires(__VA_ARGS__)                                                                  \
        using f = int;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct check_##Concept<false> {                                                                \
        template <typename T>                                                                      \
            requires(not(__VA_ARGS__))                                                             \
        using f = int;                                                                             \
    };                                                                                             \
    template <typename T, bool B>                                                                  \
    using check_##Concept##_t = check_##Concept<B>::template f<T>

DECL_CHECKER(copy_constructible, neo::copy_constructible<T>);
DECL_CHECKER(move_constructible, neo::move_constructible<T>);
DECL_CHECKER(copy_assignable, neo::copy_assignable<T>);
DECL_CHECKER(move_assignable, neo::move_assignable<T>);

DECL_CHECKER(trivially_copy_constructible, neo::trivially_constructible<T, const T&>);
DECL_CHECKER(trivially_move_constructible, neo::trivially_constructible<T, T&&>);
DECL_CHECKER(trivially_copy_assignable, neo::trivially_assignable<T&, const T&>);
DECL_CHECKER(trivially_move_assignable, neo::trivially_assignable<T&, T&&>);

struct Attrs {
    struct ctasn {
        bool construct = true, assign = construct;
    };
    struct triv {
        ctasn copy, move;
        bool  destroy = true;
    };
    static constexpr ctasn ctandasn = ctasn{.construct = true, .assign = true};

    ctasn copy = {}, move = copy;
    triv  trivial = triv{
         .copy = copy,
         .move = move,
    };
};
template <typename T, Attrs x>
struct exec_checks {
    using opt = neo::optional<T>;
    static constexpr int
    foo(check_copy_constructible_t<opt, x.copy.construct>                   = 0,
        check_copy_assignable_t<opt, x.copy.assign>                         = 0,
        check_move_constructible_t<opt, x.move.construct>                   = 0,
        check_move_assignable_t<opt, x.move.assign>                         = 0,
        check_trivially_copy_constructible_t<opt, x.trivial.copy.construct> = 0,
        check_trivially_move_constructible_t<opt, x.trivial.move.construct> = 0,
        check_trivially_copy_assignable_t<opt, x.trivial.copy.assign>       = 0,
        check_trivially_move_assignable_t<opt, x.trivial.move.assign>       = 0) {
        return 42;
    }
    static_assert(foo() == 42);
};

template <typename T, Attrs X>
constexpr auto exec_checks_v = exec_checks<T, X>{};

template <int>
struct unused {
    constexpr unused(auto...) {}
};

template <typename... Xs>
unused(Xs...) -> unused<0>;

constexpr auto u [[maybe_unused]] = unused{
    exec_checks_v<int, Attrs{}>,
    exec_checks_v<int&, Attrs{}>,
    exec_checks_v<const int&, Attrs{}>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .move_constructor = arch::special_member_kind::defaulted,
                      .move_assignment  = arch::special_member_kind::nontrivial,
                  }>,
                  Attrs{
                      .copy    = {false},
                      .move    = {true},
                      .trivial = {.copy = {false}, .move = {.construct = true, .assign = false}},
                  }>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .move_constructor = arch::special_member_kind::nontrivial,
                      .move_assignment  = arch::special_member_kind::nontrivial,
                  }>,
                  Attrs{
                      .copy    = {false},
                      .move    = {true},
                      .trivial = {.copy = {false}, .move = {false}},
                  }>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .move_constructor = arch::special_member_kind::defaulted,
                      .move_assignment  = arch::special_member_kind::defaulted,
                  }>,
                  Attrs{.copy = {false}, .move = {true}}>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .copy_constructor = arch::special_member_kind::defaulted,
                      .copy_assignment  = arch::special_member_kind::defaulted,
                  }>,
                  Attrs{}>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .destructor = arch::special_member_kind::defaulted,
                  }>,
                  Attrs{}>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      /// XXX: non-trivial destructor creates all-non-trivial SMFs (LEWG issue)
                      .destructor = arch::special_member_kind::nontrivial,
                  }>,
                  Attrs{.trivial = {.copy = {false}, .move = {false}, .destroy = false}}>,
    exec_checks_v<arch::special_members_t<arch::special_members_params{
                      .copy_constructor = arch::special_member_kind::defaulted,
                      .move_constructor = arch::special_member_kind::deleted,
                      .copy_assignment  = arch::special_member_kind::defaulted,
                  }>,
                  Attrs{}>,
};

#endif
