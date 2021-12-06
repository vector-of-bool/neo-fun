#pragma once

#include "./concepts.hpp"
#include "./platform.hpp"

NEO_PRAGMA_WARNING_PUSH();
NEO_MSVC_PRAGMA(warning(disable : 4624));

namespace neo::arch {

/**
 * @brief Mixin to disable the destructor
 */
struct indestructible {
    ~indestructible() = delete;
};

/**
 * @brief Mixin to disable default constructors
 */
struct unconstructible {
    unconstructible() = delete;
};

/**
 * @brief Mixin to make a type non-movable (immobile)
 */
struct immovable {
    immovable(immovable&&) = delete;
};

/**
 * @brief Mixin to make a type move-only (non-copyable)
 */
struct uncopyable {
    uncopyable(uncopyable&&);
    uncopyable& operator=(uncopyable&&);
};

/**
 * @brief Mixin that disables all (semi)regular type traits
 */
class irregular {
    indestructible  _1;
    unconstructible _2;
    immovable       _3;
    uncopyable      _4;
};

/**
 * @brief Base class that disables all (semi)regular operations, and defines the comma and
 * address-of operators as deleted.
 */
struct pathological : irregular {
    template <typename T>
    pathological(T&&) = delete;

    template <typename T>
    friend void operator,(const pathological&, const T&) = delete;

    template <typename T>
    friend void operator,(const T&, const pathological&) = delete;

    void operator&() = delete;
};

struct destructible : pathological {
    ~destructible() noexcept(true);
};

template <typename... Args>
struct constructible_from : destructible {
    explicit(sizeof...(Args) != 0) constructible_from(Args...);
};

struct default_initializable : constructible_from<> {};

struct move_constructible : destructible {
    move_constructible(move_constructible&&);
};

struct movable : destructible {
    movable(movable&&);
    movable& operator=(movable&&) &;

    friend void swap(movable&, movable&);
};

struct copy_constructible : destructible {
    copy_constructible(copy_constructible&);
    copy_constructible(copy_constructible&&);
    copy_constructible(const copy_constructible&);
};

struct copyable : destructible {
    copyable(copyable&);
    copyable(copyable&&);
    copyable(const copyable&);
    copyable& operator=(copyable&) &;
    copyable& operator=(copyable&&) &;
    copyable& operator=(const copyable&) &;
};

struct semiregular : copyable {
    semiregular();
};

struct boolean_testable {
    operator bool();

    boolean_testable operator!();
};

template <typename Other>
struct weakly_equality_comparable_with : pathological {
    bool operator==(const Other&) const;
};

struct equality_comparable : pathological {
    bool operator==(const equality_comparable&) const;
};

template <typename Left, typename Right>
struct decl_order_ops {
    friend boolean_testable operator<(const Left&, const Right&) { return {}; }
    friend boolean_testable operator>(const Left&, const Right&) { return {}; }
    friend boolean_testable operator<=(const Left&, const Right&) { return {}; }
    friend boolean_testable operator>=(const Left&, const Right&) { return {}; }
};

template <typename T>
struct partially_ordered_with : pathological, decl_order_ops<partially_ordered_with<T>, T> {};

struct totally_ordered : equality_comparable, decl_order_ops<totally_ordered, totally_ordered> {};

struct regular : semiregular {
    friend bool operator==(const regular&, const regular&);
};

template <typename T>
struct derived_from : T, pathological {};

template <typename T>
struct convertible_to : pathological {
    operator T() &;
    operator T() const&;

    template <typename U>
    operator U() const& = delete;
};

template <typename T>
struct assignable_from : convertible_to<T> {
    assignable_from& operator=(T&&) &;
};

template <typename... Args>
struct invocable : pathological {
    pathological& operator()(Args&&...);
};

template <typename... Args>
struct predicate : pathological {
    bool operator()(Args&&...);
};

}  // namespace neo::arch

NEO_PRAGMA_WARNING_POP();
