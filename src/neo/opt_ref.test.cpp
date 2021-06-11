#include <neo/opt_ref.hpp>
#include <neo/repr.hpp>

#include <type_traits>

#include <catch2/catch.hpp>

static_assert(neo::is_opt_ref_v<neo::opt_ref<int>>);
static_assert(!neo::is_opt_ref_v<int>);
static_assert(std::is_trivially_copyable_v<neo::opt_ref<int>>);

TEST_CASE("Basics") {
    int int_1 = 3;
    int int_2 = 4;

    neo::opt_ref<int> disengaged_1;
    neo::opt_ref<int> disengaged_2{std::nullopt};
    neo::opt_ref<int> engaged_1 = int_1;
    neo::opt_ref<int> engaged_2 = int_2;

    CHECK(engaged_1);
    CHECK_FALSE(disengaged_1);
    CHECK_FALSE(disengaged_2);

    CHECK(*engaged_1 == 3);
    CHECK(*engaged_2 == 4);

    // Some repr()
    CHECK(neo::repr(engaged_1).string() == "neo::opt_ref<int32>{->3}");
    CHECK(neo::repr(disengaged_1).string() == "neo::opt_ref<int32>{nullopt}");

    // Convert a reference to T to an optional<T>
    std::optional<int> opt_1 = engaged_1;
    CHECK(opt_1);
    CHECK(opt_1 == 3);

    std::optional<int> opt_2 = disengaged_2;
    CHECK_FALSE(opt_2);

    using std::nullopt;

    // Relational operators with nullopt_t
    CHECK_FALSE(engaged_1 == nullopt);
    CHECK_FALSE(nullopt == engaged_1);
    CHECK(disengaged_1 == nullopt);
    CHECK(nullopt == disengaged_1);

    CHECK(engaged_1 != nullopt);
    CHECK(nullopt != engaged_1);
    CHECK_FALSE(disengaged_1 != nullopt);
    CHECK_FALSE(nullopt != disengaged_1);

    CHECK_FALSE(engaged_1 < nullopt);
    CHECK(nullopt < engaged_1);
    CHECK_FALSE(disengaged_1 < nullopt);
    CHECK_FALSE(nullopt < disengaged_1);

    CHECK_FALSE(engaged_1 <= nullopt);
    CHECK(nullopt <= engaged_1);
    CHECK(disengaged_1 <= nullopt);
    CHECK(nullopt <= disengaged_1);

    CHECK(engaged_1 > nullopt);
    CHECK_FALSE(nullopt > engaged_1);
    CHECK_FALSE(disengaged_1 > nullopt);
    CHECK_FALSE(nullopt > disengaged_1);

    CHECK(engaged_1 >= nullopt);
    CHECK_FALSE(nullopt >= engaged_1);
    CHECK(disengaged_1 >= nullopt);
    CHECK(nullopt >= disengaged_1);

    // Relational operators between opt_refs
    CHECK_FALSE(engaged_1 == engaged_2);
    CHECK_FALSE(engaged_2 == engaged_1);
    CHECK(disengaged_1 == disengaged_2);
    CHECK(disengaged_2 == disengaged_1);

    CHECK(engaged_1 != engaged_2);
    CHECK(engaged_2 != engaged_1);
    CHECK_FALSE(disengaged_1 != disengaged_2);
    CHECK_FALSE(disengaged_2 != disengaged_1);

    CHECK(engaged_1 < engaged_2);
    CHECK_FALSE(engaged_2 < engaged_1);
    CHECK_FALSE(disengaged_1 < disengaged_2);
    CHECK_FALSE(disengaged_2 < disengaged_1);

    CHECK(engaged_1 <= engaged_2);
    CHECK_FALSE(engaged_2 <= engaged_1);
    CHECK(disengaged_1 <= disengaged_2);
    CHECK(disengaged_2 <= disengaged_1);

    CHECK_FALSE(engaged_1 > engaged_2);
    CHECK(engaged_2 > engaged_1);
    CHECK_FALSE(disengaged_1 > disengaged_2);
    CHECK_FALSE(disengaged_2 > disengaged_1);

    CHECK_FALSE(engaged_1 >= engaged_2);
    CHECK(engaged_2 >= engaged_1);
    CHECK(disengaged_1 >= disengaged_2);
    CHECK(disengaged_2 >= disengaged_1);

    // Relational operators with values
    CHECK_FALSE(engaged_1 == int_2);
    CHECK_FALSE(int_2 == engaged_1);
    CHECK_FALSE(disengaged_1 == int_2);
    CHECK_FALSE(int_2 == disengaged_1);

    CHECK(engaged_1 != int_2);
    CHECK(int_2 != engaged_1);
    CHECK(disengaged_1 != int_2);
    CHECK(int_2 != disengaged_1);

    CHECK(engaged_1 < int_2);
    CHECK_FALSE(int_2 < engaged_1);
    CHECK(disengaged_1 < int_2);
    CHECK_FALSE(int_2 < disengaged_1);

    CHECK(engaged_1 <= int_2);
    CHECK_FALSE(int_2 <= engaged_1);
    CHECK(disengaged_1 <= int_2);
    CHECK_FALSE(int_2 <= disengaged_1);

    CHECK_FALSE(engaged_1 > int_2);
    CHECK(int_2 > engaged_1);
    CHECK_FALSE(disengaged_1 > int_2);
    CHECK(int_2 > disengaged_1);

    CHECK_FALSE(engaged_1 >= int_2);
    CHECK(int_2 >= engaged_1);
    CHECK_FALSE(disengaged_1 >= int_2);
    CHECK(int_2 >= disengaged_1);
}
