#pragma once

#include "./concepts.hpp"
#include "./object_box.hpp"
#include "./unit.hpp"

#include <memory>

namespace neo {

/**
 * @brief Presents constexpr-ready storage for an object of type T.
 *
 * The storage is has conditionally trivial special member functions, but is
 * not trivially default-constructible (required for constexpr). This object
 * is conditionally empty if T is empty.
 *
 * @tparam T Any object type, reference type, void type, or bounded array type.
 *
 * The storage is not exposed directly, and starts unalive. The value may be
 * made "alive" by calling .construct(), and ended by .destroy(). The .get()
 * method returns a reference to the live object.
 *
 * To copy the stored object, use copy_from and assign_from (not construct())
 */
template <typename T>
class storage_for {
    using boxed_t = object_box<T>;

    union union_t {
        /// The initiail "nothing" value
        NEO_NO_UNIQUE_ADDRESS unit _unit;
        /// The union member of the wrapped type
        NEO_NO_UNIQUE_ADDRESS boxed_t _box;

        constexpr union_t() noexcept
            : _unit() {}

        union_t(const union_t&)
            requires trivially_constructible<boxed_t, const boxed_t&>
        = default;

        union_t(union_t&&)
            requires trivially_constructible<boxed_t, boxed_t&&>
        = default;

        union_t& operator=(const union_t&)
            requires trivially_assignable<boxed_t&, const boxed_t&>
        = default;

        union_t& operator=(union_t&&)
            requires trivially_assignable<boxed_t&, boxed_t&&>
        = default;

        ~union_t() = default;
        constexpr ~union_t()
            requires(not trivially_destructible<boxed_t>)
        {}
    };

    /// @private The union used as storage.
    NEO_NO_UNIQUE_ADDRESS union_t _p_union;

public:
    /// Copy-construct the value into storage from the other live storage.
    constexpr void copy_from(storage_for const& other) {
        std::construct_at(&_p_union._box, other._p_union._box);
    }
    constexpr void copy_from(storage_for&& other) {
        std::construct_at(&_p_union._box, NEO_MOVE(other)._p_union._box);
    }
    /// Assign to the value in live storage from the other live storage.
    constexpr void assign_from(storage_for const& other) { _p_union._box = other._p_union._box; }
    constexpr void assign_from(storage_for&& other) {
        _p_union._box = NEO_MOVE(other)._p_union._box;
    }

    /// Obtain the current stored value.
    constexpr add_lvalue_reference_t<T> get() & noexcept { return _p_union._box.get(); }
    constexpr add_const_reference_t<T>  get() const& noexcept { return _p_union._box.get(); }
    constexpr add_rvalue_reference_t<T> get() && noexcept { return NEO_MOVE(_p_union)._box.get(); }
    constexpr add_rvalue_reference_t<add_const_t<T>> get() const&& noexcept {
        return NEO_MOVE(_p_union)._box.get();
    }

    /**
     * @brief Construct the value in-place in storage. The storage must not already contain a value.
     *
     * If this function returns normally, the storage now contains a live object.
     */
    template <typename... Args>
        requires constructible_from<boxed_t, Args...>
    constexpr void
    construct(Args&&... args) noexcept(nothrow_constructible_from<boxed_t, Args...>) {
        std::construct_at(NEO_ADDRESSOF(_p_union._box), NEO_FWD(args)...);
    }

    /**
     * @brief Destroy the value currently in storage. The storage must contain a live object.
     *
     * Once this function returns, the storage no longer contains a live object.
     */
    constexpr void destroy() noexcept { std::destroy_at(NEO_ADDRESSOF(_p_union._box)); }

    /**
     * @brief Swap to two stored values.
     *
     * Both this and other must contain live objects to swap
     */
    constexpr void swap(storage_for& other) {
        using std::swap;
        swap(_p_union._box, other._p_union._box);
    }
};

}  // namespace neo