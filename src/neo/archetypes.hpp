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

struct deleted_copy_constructor {
    deleted_copy_constructor(const deleted_copy_constructor&) = delete;
    deleted_copy_constructor(deleted_copy_constructor&&)      = default;

    deleted_copy_constructor& operator=(const deleted_copy_constructor&) = default;
    deleted_copy_constructor& operator=(deleted_copy_constructor&&)      = default;
};

struct deleted_move_constructor {
    deleted_move_constructor(const deleted_move_constructor&) = default;
    deleted_move_constructor(deleted_move_constructor&&)      = delete;

    deleted_move_constructor& operator=(const deleted_move_constructor&) = default;
    deleted_move_constructor& operator=(deleted_move_constructor&&)      = default;
};

struct deleted_copy_assignment {
    deleted_copy_assignment(const deleted_copy_assignment&) = default;
    deleted_copy_assignment(deleted_copy_assignment&&)      = default;

    deleted_copy_assignment& operator=(const deleted_copy_assignment&) = delete;
    deleted_copy_assignment& operator=(deleted_copy_assignment&&)      = default;
};

struct deleted_move_assignment {
    deleted_move_assignment(const deleted_move_assignment&) = default;
    deleted_move_assignment(deleted_move_assignment&&)      = default;

    deleted_move_assignment& operator=(const deleted_move_assignment&) = default;
    deleted_move_assignment& operator=(deleted_move_assignment&&)      = delete;
};

struct deleted_copy_move {
    deleted_copy_constructor _1;
    deleted_move_constructor _2;
};

struct deleted_assignment {
    deleted_copy_assignment _1;
    deleted_move_assignment _2;
};

/**
 * @brief Mixin to make a type non-movable (immobile)
 */
struct immovable {
    immovable()            = default;
    immovable(immovable&&) = delete;
};

/**
 * @brief Mixin to make a type move-only (non-copyable)
 */
struct uncopyable {
    uncopyable() = default;
    uncopyable(uncopyable&&);
    uncopyable& operator=(uncopyable&&);
};

NEO_PRAGMA_WARNING_PUSH();
// Older GCC doesn't recognize the [[maybe_unused]] on members
NEO_GNU_PRAGMA(GCC diagnostic ignored "-Wattributes");

/**
 * @brief Mixin that disables all (semi)regular type traits
 */
class irregular {
    indestructible  _1 [[maybe_unused]];
    unconstructible _2 [[maybe_unused]];
    immovable       _3 [[maybe_unused]];
    uncopyable      _4 [[maybe_unused]];
};

NEO_PRAGMA_WARNING_POP();

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

struct trivially_default_constructible {
    // XXX: https://cplusplus.github.io/LWG/issue2116
    // ~trivially_default_constructible() {}
    uncopyable _2;
    immovable  _3;
};

struct nontrivial_default_constructor {
    nontrivial_default_constructor();
};

struct nontrivial_destructor {
    ~nontrivial_destructor();
};

struct nontrivial_copy_constructor {
    nontrivial_copy_constructor(const nontrivial_copy_constructor&);
};

struct nontrivial_move_constructor {
    nontrivial_move_constructor(nontrivial_move_constructor&&);
};

struct nontrivial_copy_assignment {
    nontrivial_copy_assignment& operator=(const nontrivial_copy_assignment&) &;
};

struct nontrivial_move_assignment {
    nontrivial_move_assignment& operator=(nontrivial_move_assignment&&) &;
};

struct trivially_copy_constructible {
    trivially_copy_constructible(const trivially_copy_constructible&) = default;
    trivially_copy_constructible(trivially_copy_constructible&&)      = delete;
    unconstructible    _1;
    deleted_assignment _2;
};

struct trivially_copy_assignable {
    unconstructible            _1;
    deleted_copy_move          _2;
    trivially_copy_assignable& operator=(trivially_copy_assignable const&) & = default;
    trivially_copy_assignable& operator=(trivially_copy_assignable&&) &      = delete;
};

struct trivially_copyable {
    deleted_move_constructor       _1;
    deleted_move_assignment        _2;
    nontrivial_default_constructor _3;
};

struct trivially_movable {
    deleted_copy_constructor       _1;
    deleted_copy_assignment        _2;
    nontrivial_default_constructor _3;
};

struct trivially_destructible {
    nontrivial_default_constructor _1;
    nontrivial_copy_constructor    _2;
    nontrivial_move_constructor    _3;
    nontrivial_copy_assignment     _4;
    nontrivial_move_assignment     _5;
};

}  // namespace neo::arch

NEO_PRAGMA_WARNING_POP();
