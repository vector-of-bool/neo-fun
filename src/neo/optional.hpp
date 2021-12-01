#pragma once

#include "./attrib.hpp"
#include "./declval.hpp"

#include <type_traits>

namespace neo {

namespace opt_detail {

template <typename T>
requires std::is_class_v<T>
struct inherit_from : T {
};

// Hack: A concept that matches a type T that is a in_place_t tag
template <typename T>
concept in_place_t_tag = requires {
    // Check for an injected-name matching in_place_t
    typename inherit_from<T>::in_place_t;
}
// And that the nested name is the same as the enclosing class
&&std::is_same_v<T, typename inherit_from<T>::in_place_t>;

// A tiny in-place tag we can use
struct opt_in_place_t {
    using in_place_t = opt_in_place_t;
};
// (This is all so we don't pull <utility>)

}  // namespace opt_detail

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
    template <typename... Args>
    constexpr explicit nano_opt_storage(opt_detail::in_place_t_tag auto, Args&&... args)  //
        noexcept(std::is_nothrow_constructible_v<T, Args...>)                             //
        requires std::is_constructible_v<T, Args...>                                      //
        : value((Args &&)(args)...) {}

    /// Default-construct as disenganged
    constexpr nano_opt_storage() = default;
    constexpr nano_opt_storage() requires(!std::is_trivially_default_constructible_v<T>) {}

#if !NEO_COMPILER_IS_CLANG
#define NEO_NANO_OPT_SUPPORTS_TRIVIAL_DESTRUCTIBILITY 1
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() requires(!std::is_trivially_destructible_v<T>) {}
#else
#define NEO_NANO_OPT_SUPPORTS_TRIVIAL_DESTRUCTIBILITY 0
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() {}
#endif
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

    void _destroy_nocheck() noexcept {
        _storage.value.~T();
        _active = false;
    }

public:
    // Default-construct as dis-engaged
    constexpr nano_opt() = default;

    // Construct as a new T
    template <typename Arg>
    requires std::is_same_v<std::remove_cvref_t<Arg>, T>  //
    constexpr nano_opt(Arg&& arg)
        : _storage(opt_detail::opt_in_place_t{}, (Arg &&)(arg))
        , _active(true) {}

    // Construct as a new T with the given arguments
    template <opt_detail::in_place_t_tag InPlace, typename... Args>
    constexpr explicit nano_opt(InPlace tag, Args&&... args)   //
        noexcept(std::is_nothrow_constructible_v<T, Args...>)  //
        requires std::is_constructible_v<T, Args...>           //
        : _storage(tag, (Args &&)(args)...), _active(true) {}

    // Pick the type of destructor we want based on whether the underlying type is triviallty
    // destructible
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt() requires(!std::is_trivially_destructible_v<T>) {
        if (has_value()) {
            _destroy_nocheck();
        }
    }

    // Pick the type of copy we want based on whether the underlying type is trivially copyable
    constexpr nano_opt(const nano_opt&) = default;
    // If not trivial copy:
    constexpr nano_opt(const nano_opt& other)              //
        noexcept(std::is_nothrow_copy_constructible_v<T>)  //
        requires(!std::is_trivially_copyable_v<T>) {
        if (other.has_value()) {
            emplace(other.get());
        }
    }

    // Pick the type of move we want based on whether the underlying type is trivially copyable
    constexpr nano_opt(nano_opt&&) = default;
    // If no trivial move:
    constexpr nano_opt(nano_opt&& other) noexcept(std::is_nothrow_move_constructible_v<T>) requires(
        !std::is_trivially_move_constructible_v<T>) {
        if (other.has_value()) {
            emplace((T &&)(other));
        }
    }

    // Pick the type of copy-assignment we want based on whether the underlying type is trivially
    // copyable-assignable
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
    constexpr nano_opt& operator=(nano_opt&& other)        //
        noexcept(std::is_nothrow_move_assignable_v<T>)     //
        requires(!std::is_trivially_move_assignable_v<T>)  //
    {
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

    /// If the optional contains a value, destroy that value and become dis-engaged.
    /// If we do not contain a value, does nothing.
    constexpr void reset() noexcept {
        if (has_value()) {
            _destroy_nocheck();
        }
    }

    /// In-place construct a new instance of 'T' with the given arguments.
    /// If we already contained a value, destroy that value
    template <typename... Args>
    constexpr T& emplace(Args&&... args)                       //
        noexcept(std::is_nothrow_constructible_v<T, Args...>)  //
        requires std::is_constructible_v<T, Args...> {
        reset();
        ::new (const_cast<void*>(static_cast<const void*>(&_storage.value))) T((Args &&)(args)...);
        _active = true;
        return get();
    }

    /// Determine whether we contain a value
    constexpr bool has_value() const noexcept { return _active; }

    /// Obtain the underlying value.
    constexpr T&        get() & noexcept { return _storage.value; }
    constexpr const T&  get() const& noexcept { return _storage.value; }
    constexpr T&&       get() && noexcept { return (T &&) _storage.value; }
    constexpr const T&& get() const&& noexcept { return (T &&) _storage.value; }

    friend constexpr void do_repr(auto out, const nano_opt* self) noexcept requires requires {
        out.repr(NEO_DECLVAL(const T&));
    }
    {
        if constexpr (out.just_type) {
            out.append("neo::nano_opt<{}>", out.template repr_type<T>());
        } else if constexpr (out.just_value) {
            if (self->has_value()) {
                out.append("[{}]", out.repr_value(self->get()));
            } else {
                out.append("nullopt");
            }
        } else {
            out.append("[{} {}]", out.template repr_type<nano_opt>(), out.repr_value(*self));
        }
    }
};

}  // namespace neo
