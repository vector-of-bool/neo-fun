#pragma once

#include <type_traits>

#include "./attrib.hpp"

namespace neo {

namespace detail {

// Hack: A concept that matches a type T that is a in_place_t tag
template <typename T>
concept in_place_t_tag = requires {
    // Check for an injected-name matching in_place_t
    typename T::in_place_t;
}
// And that the nested name is the same as the enclosing class
&&std::is_same_v<T, typename T::in_place_t>;

// A tiny in-place tag we can use
struct opt_in_place_t {
    using in_place_t = opt_in_place_t;
};
// (This is all so we don't pull <utility>)

}  // namespace detail

/**
 * @brief Provides storage for an object of type 'T'.
 *
 * The storage begins uninitialized. The lifetime of the `.value` members must
 * be managed manually.
 *
 * @tparam T The type that is stored in the union.
 */
template <typename T>
union nano_opt_storage {
    char _empty;
    T    value;

    /**
     * @brief Construct a value in-place.
     */
    template <detail::in_place_t_tag InPlace, typename... Args>
    constexpr explicit nano_opt_storage(InPlace, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>) requires std::is_constructible_v<T, Args...>
        : value((Args &&)(args)...) {}

    /// Default-construct as disenganged
    constexpr nano_opt_storage() = default;
    constexpr nano_opt_storage() requires(!std::is_trivially_default_constructible_v<T>) {}

    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() requires(!std::is_trivially_destructible_v<T>) {}
};

/**
 * @brief An extremely minimal optional-lite class.
 *
 * @tparam T
 */
template <typename T>
class nano_opt {
    using storage_type = nano_opt_storage<T>;

    storage_type _storage;
    bool         _active = false;

    void _destroy_nocheck() noexcept { _storage.value.~T(); }

public:
    constexpr nano_opt() = default;

    template <typename Arg>
    requires std::is_same_v<std::remove_cvref_t<Arg>, T>  //
        constexpr nano_opt(Arg&& arg)
        : _storage(detail::opt_in_place_t{}, (Arg &&)(arg))
        , _active(true) {}

    template <detail::in_place_t_tag InPlace, typename... Args>
    constexpr explicit nano_opt(InPlace tag, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>) requires std::is_constructible_v<T, Args...>
        : _storage(tag, (Args &&)(args)...), _active(true) {}

    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt() requires(!std::is_trivially_destructible_v<T>) {
        if (has_value()) {
            _destroy_nocheck();
        }
    }

    constexpr nano_opt(const nano_opt&) = default;
    constexpr nano_opt(const nano_opt& other) noexcept(
        std::is_nothrow_copy_constructible_v<T>) requires(!std::is_trivially_copyable_v<T>) {
        if (other.has_value()) {
            emplace(other.get());
        }
    }

    constexpr nano_opt(nano_opt&&) = default;
    constexpr nano_opt(nano_opt&& other) noexcept(std::is_nothrow_move_constructible_v<T>) requires(
        !std::is_trivially_move_constructible_v<T>) {
        if (other.has_value()) {
            emplace((T &&)(other));
        }
    }

    constexpr nano_opt& operator=(const nano_opt&) = default;
    // If no trivial assignment:
    constexpr nano_opt& operator=(const nano_opt& other) noexcept(
        std::is_nothrow_copy_assignable_v<T>) requires(!std::is_trivially_copy_assignable_v<T>) {
        if (has_value()) {
            if (other.has_value()) {
                get() = other.get();
            } else {
                _destroy_nocheck();
            }
        } else {
            if (other.has_value()) {
                emplace(get());
            } else {
                // Nothing to do.
            }
        }
        return *this;
    }

    constexpr nano_opt& operator=(nano_opt&&) = default;
    // Move
    constexpr nano_opt& operator=(nano_opt&& other) noexcept(
        std::is_nothrow_move_assignable_v<T>) requires(!std::is_trivially_move_assignable_v<T>) {
        if (has_value()) {
            if (other.has_value()) {
                get() = (T &&)(other.get());
            } else {
                _destroy_nocheck();
            }
        } else {
            if (other.has_value()) {
                emplace((T &&)(get()));
            } else {
                // Nothing to do
            }
        }
        return *this;
    }

    template <typename... Args>
    T& emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        ::new (const_cast<void*>(static_cast<const void*>(&_storage.value))) T((Args &&)(args)...);
        _active = true;
        return get();
    }

    constexpr bool     has_value() const noexcept { return _active; }
    constexpr T&       get() noexcept { return _storage.value; }
    constexpr const T& get() const noexcept { return _storage.value; }
};

}  // namespace neo
