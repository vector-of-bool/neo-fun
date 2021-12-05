#pragma once

#include "./opt_ref_fwd.hpp"

#include "./addressof.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <optional>

namespace neo {

using std::nullopt;
using std::nullopt_t;

/**
 * @brief An optional reference type.
 *
 * Either contains a reference to a T, or is null
 *
 * @tparam T The referred-to type. May be cv-qualified.
 */
template <typename T>
class opt_ref {
    T* _ptr = nullptr;

public:
    /// Default-construct to null
    constexpr opt_ref() noexcept = default;
    /// Bind to the given object
    constexpr opt_ref(T& reference) noexcept
        : _ptr(std::addressof(reference)) {}
    /// Bind to a pointer to the object (may be nullptr)
    constexpr opt_ref(T* ptr) noexcept
        : _ptr(ptr) {}
    /// Explicitly construct as a null reference
    constexpr opt_ref(std::nullopt_t) noexcept
        : _ptr(nullptr) {}

    /// Determine whether the reference is bound or not
    constexpr explicit operator bool() const noexcept { return _ptr != nullptr; }

    /**
     * @brief Obtain a reference to the referred-to object. Asserts to be non-NULL
     */
    constexpr T& operator*() const noexcept {
        assert(_ptr != nullptr && "Dereferencing null opt_ref");
        return *_ptr;
    }

    /**
     * @brief Access members of the pointed-to objects. Asserts to be non-NULL
     */
    constexpr T* operator->() const noexcept {
        assert(_ptr != nullptr && "Dereferencing null opt_ref");
        return _ptr;
    }

    // clang-format off
    /**
     * @brief Convert to an object that is constructible from either a T or a nullopt
     */
    template <typename U>
    requires std::constructible_from<U, T&>
          && std::constructible_from<U, nullopt_t>
    explicit(!std::convertible_to<T&, U>
          || !std::convertible_to<nullopt_t, U>)
    constexpr operator U() const
        noexcept(std::is_nothrow_constructible_v<U, T&>  //
              && std::is_nothrow_constructible_v<U, nullptr_t>)
    {
        // clang-format on
        if (*this) {
            return static_cast<U>(**this);
        }
        return static_cast<U>(nullopt);
    }

    /**
     * @brief Obtain a possibly-null pointer to the referred-to object
     */
    constexpr T* pointer() const noexcept { return _ptr; }

    // Compare with nullopt
    constexpr friend bool operator==(opt_ref lhs, nullopt_t) noexcept { return !bool(lhs); }
    constexpr friend std::strong_ordering operator<=>(opt_ref lhs, nullopt_t) noexcept {
        if (lhs) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    // Compare with another opt_ref
    constexpr friend bool operator==(opt_ref lhs, opt_ref rhs) noexcept  //
        requires std::equality_comparable<T> {
        if (bool(lhs) != bool(rhs)) {
            return false;
        }
        return bool(lhs) ? (*lhs == *rhs) : true;
    }
    constexpr friend auto operator<=>(opt_ref left, opt_ref right) noexcept  //
        requires std::three_way_comparable<T> {
        using cat = std::compare_three_way_result_t<T>;
        if (bool(left) != bool(right)) {
            if (left) {
                // engaged > null
                return cat::greater;
            } else {
                // null < enganged
                return cat::less;
            }
        }
        if (!left) {
            // both null
            return cat::equal;
        }
        // Compare the referred
        return std::compare_three_way{}(*left, *right);
    }

    constexpr friend bool operator==(const opt_ref& lhs,
                                     const T&       value) noexcept  //
        requires std::equality_comparable<T> {
        if (!lhs) {
            return false;
        }
        return *lhs == value;
    }

    constexpr friend auto operator<=>(const opt_ref& lhs,
                                      const T&       rhs) noexcept  //
        requires std::three_way_comparable<T> {
        using cat = std::compare_three_way_result_t<T>;
        if (!lhs) {
            return cat::less;
        }
        return std::compare_three_way{}(*lhs, rhs);
    }

    constexpr friend void do_repr(auto out, opt_ref const* self) noexcept {
        if constexpr (decltype(out)::template can_repr<T>) {
            out.type("neo::opt_ref<{}>", out.template repr_type<T>());
        } else {
            out.type("neo::opt_ref<[...]>");
        }
        if (self and *self) {
            if constexpr (decltype(out)::template can_repr<T>) {
                out.value("->{}", out.repr_value(**self));
            } else {
                out.value("->[opaque-value]");
            }
        } else {
            out.value("nullopt");
        }
    }
};

template <typename T>
opt_ref(T&& what) -> opt_ref<std::remove_reference_t<T>>;

}  // namespace neo
