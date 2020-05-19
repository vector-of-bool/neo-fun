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

}  // namespace

TEST_CASE("Create an iterator facade") {}

namespace {

class iota_iterator : public neo::iterator_facade<iota_iterator> {
    std::ptrdiff_t _value = 0;

public:
    iota_iterator() = default;
    explicit iota_iterator(std::ptrdiff_t i)
        : _value(i) {}

    std::ptrdiff_t dereference() const noexcept { return _value; }

    void advance(std::ptrdiff_t off) noexcept { _value += off; }
    auto distance_to(iota_iterator o) const noexcept { return *o - **this; }

    bool equal_to(iota_iterator o) const { return *o == **this; }
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

    static_assert(std::is_same_v<decltype(it.operator->()), neo::arrow_proxy<std::string>>);
    // Even though we are acting on a temporary, the append() will return a new string
    auto twenty_four = it->append("4");
    CHECK(twenty_four == "24");
}

TEST_CASE("Sentinel support") {
    struct until_7_iter : neo::iterator_facade<until_7_iter> {
        int value = 0;
        struct sentinel_type {};

        auto dereference() const noexcept { return value; }
        auto increment() noexcept { ++value; }

        bool at_end() const noexcept { return value == 7; }
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
}