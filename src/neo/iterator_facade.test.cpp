#include <neo/iterator_facade.hpp>

#include <catch2/catch.hpp>

#include <string>
#include <vector>

namespace {

template <typename Iter>
class as_string_iterator : public neo::iterator_wrapper_facade<as_string_iterator<Iter>, Iter> {
    using as_string_iterator::iterator_wrapper_facade::iterator_wrapper_facade;

public:
    std::string dereference() const noexcept { return std::to_string(*this->wrapped_iterator); }
};

template <typename It>
as_string_iterator(It) -> as_string_iterator<It>;

class iota_iterator : public neo::iterator_facade<iota_iterator> {
    int _value = 0;

public:
    iota_iterator() = default;
    explicit iota_iterator(int i)
        : _value(i) {}

    int dereference() const noexcept { return _value; }

    void advance(int off) noexcept { _value += off; }
    int  distance_to(iota_iterator o) const noexcept { return *o - **this; }
    bool operator==(iota_iterator o) const noexcept { return *o == **this; }
};

}  // namespace

TEST_CASE("Create an iota_iterator") {
    iota_iterator it;
    iota_iterator stop{44};
    CHECK((stop - it) == 44);
    CHECK(it < stop);
    CHECK(it <= stop);
    CHECK_FALSE(it > stop);
    CHECK_FALSE(it >= stop);
    CHECK(std::distance(it, stop) == 44);

    CHECK(it[33] == 33);
    CHECK(it[-9] == -9);
    CHECK(stop[2] == 46);
    CHECK(stop[-44] == 0);

    CHECK((stop - it) == 44);
    CHECK((it - stop) == -44);

    CHECK(it != stop);
    CHECK((it + 44) == stop);
    CHECK(it == (stop - 44));
}

TEST_CASE("arrow_proxy") {
    neo::arrow_proxy<std::string> s{""};
    s->append("Hello, ");
    s->append("world!");
    CHECK(*s.operator->() == "Hello, world!");
}

TEST_CASE("Trivial iterator") {
    struct deref_iter : neo::iterator_facade<deref_iter> {
        int* value = nullptr;
        int& dereference() const noexcept { return *value; }

        deref_iter(int& i)
            : value(&i) {}
    };

    // Not even an increment operator.

    int        i = 12;
    deref_iter it{i};

    CHECK(*it == 12);
    i = 7;
    CHECK(*it == 7);
}

TEST_CASE("Single-pass iterator") {
    struct in_iter : neo::iterator_facade<in_iter> {
        int value = 0;
        enum { single_pass_iterator = true };

        const int& dereference() const noexcept { return value; }
        void       increment() noexcept { ++value; }
    };

    in_iter it;
    CHECK(*it == 0);
    static_assert(std::is_same_v<decltype(++it), in_iter&>);
    ++it;
    CHECK(*it == 1);
    static_assert(std::is_void_v<decltype(it++)>);
}

TEST_CASE("Transforming iterator") {
    std::vector<int>   values = {1, 2, 3, 4};
    as_string_iterator it{values.begin()};

    CHECK(*it == "1");
    ++it;
    CHECK(*it == "2");
    CHECK_FALSE(it == as_string_iterator(values.begin()));
    // Post-increment returns a copy of the iterator
    auto copy = it++;
    CHECK(*copy == "2");

    static_assert(std::is_same_v<decltype(it.operator->()), neo::arrow_proxy<std::string>>);
    // Even though we are acting on a temporary, the append() will return a new string
    auto thirty_four = it->append("4");
    CHECK(thirty_four == "34");

    copy = copy - 1;
    CHECK(*copy == "1");
    CHECK(*(copy + 3) == "4");
    CHECK(*(3 + copy) == "4");

    ++copy;
    auto copy2 = copy--;
    CHECK(*copy == "1");
    CHECK(*copy2 == "2");

    // Advance by a negative number created from an unsigned
    CHECK(*copy == "1");
    ++copy;
    copy -= 1u;
    CHECK(*copy == "1");
}

TEST_CASE("Sentinel support") {
    struct until_7_iter : neo::iterator_facade<until_7_iter> {
        int value = 0;
        struct sentinel_type {};

        auto dereference() const noexcept { return value; }
        auto increment() noexcept { ++value; }

        auto distance_to(sentinel_type) const noexcept { return 7 - value; }
        bool operator==(sentinel_type s) const noexcept { return distance_to(s) == 0; }
    };

    struct seven_range {
        auto begin() { return until_7_iter(); }
        auto end() { return until_7_iter::sentinel_type(); }
    };

    int sum = 0;
    for (auto i : seven_range()) {
        sum += i;
        CHECK(i < 7);
    }
    CHECK(sum == (1 + 2 + 3 + 4 + 5 + 6));

    auto it   = seven_range().begin();
    auto stop = seven_range().end();
    CHECK(it != stop);
    CHECK(stop != it);
    CHECK_FALSE(it == stop);
    CHECK_FALSE(stop == it);

#if !_MSC_VER
    /// XXX: Last checked, MSVC has an issue finding the correct operator-() via ADL.
    /// If you're using MSVC and reading this comment in the future, revive this
    /// snippet and try again.
    CHECK((until_7_iter::sentinel_type() - it) == 7);
#endif
}
