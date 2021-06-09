#pragma once

#include <cassert>
#include <optional>
#include <type_traits>

namespace neo {

using std::nullopt;
using std::nullopt_t;

template <typename T>
class opt_ref;

template <typename T>
struct is_opt_ref : std::false_type {};

template <typename T>
struct is_opt_ref<opt_ref<T>> : std::true_type {};

template <typename T>
constexpr bool is_opt_ref_v = is_opt_ref<T>::value;

template <typename T>
class opt_ref {
    T* _ptr = nullptr;

public:
    constexpr opt_ref() = default;
    constexpr opt_ref(T& reference)
        : _ptr(std::addressof(reference)) {}
    constexpr opt_ref(T* ptr)
        : _ptr(ptr) {}
    constexpr opt_ref(std::nullopt_t)
        : _ptr(nullptr) {}

    constexpr explicit operator bool() const noexcept { return _ptr != nullptr; }

    constexpr T& operator*() const noexcept {
        assert(_ptr != nullptr && "Dereferencing null opt_ref");
        return *_ptr;
    }

    constexpr T* operator->() const noexcept {
        assert(_ptr != nullptr && "Dereferencing null opt_ref");
        return _ptr;
    }

    template <typename U>
    constexpr operator U() const noexcept
        requires(std::is_convertible_v<T&, U>&& std::is_convertible_v<std::nullopt_t, U>) {
        if (*this) {
            return **this;
        }
        return nullopt;
    }

    constexpr T* pointer() const noexcept { return _ptr; }

    // Compare with nullopt
    constexpr friend bool operator==(opt_ref lhs, nullopt_t) noexcept { return !bool(lhs); }
    constexpr friend bool operator==(nullopt_t, opt_ref rhs) noexcept { return !bool(rhs); }
    constexpr friend bool operator!=(opt_ref lhs, nullopt_t) noexcept { return bool(lhs); }
    constexpr friend bool operator!=(nullopt_t, opt_ref rhs) noexcept { return bool(rhs); }
    constexpr friend bool operator<(opt_ref, nullopt_t) noexcept { return false; }
    constexpr friend bool operator<(nullopt_t, opt_ref rhs) noexcept { return bool(rhs); }
    constexpr friend bool operator<=(opt_ref lhs, nullopt_t) noexcept { return !bool(lhs); }
    constexpr friend bool operator<=(nullopt_t, opt_ref) noexcept { return true; }
    constexpr friend bool operator>(opt_ref lhs, nullopt_t) noexcept { return bool(lhs); }
    constexpr friend bool operator>(nullopt_t, opt_ref) noexcept { return false; }
    constexpr friend bool operator>=(opt_ref, nullopt_t) noexcept { return true; }
    constexpr friend bool operator>=(nullopt_t, opt_ref rhs) noexcept { return !rhs; }

    constexpr friend void do_repr(auto out, opt_ref const* self) noexcept requires requires {
        out.repr_value(**self);
    }
    {
        if constexpr (out.just_type) {
            out.append("neo::opt_ref<{}>", out.template repr_type<T>());
        } else if constexpr (out.just_value) {
            if (!*self) {
                out.append("nullopt");
            } else {
                out.append("[{}]", out.repr_value(**self));
            }
        } else {
            out.append("[{} {}]", out.template repr_type<opt_ref>(), out.repr_value(*self));
        }
    }

#define DECL_OPT(Op)                                                                               \
    template <typename Other>                                                                      \
    constexpr friend bool operator Op(opt_ref lhs, opt_ref<Other> rhs) noexcept {                  \
        if (!lhs) {                                                                                \
            return nullopt Op rhs;                                                                 \
        } else if (!rhs) {                                                                         \
            return lhs Op nullopt;                                                                 \
        } else {                                                                                   \
            return *lhs Op * rhs;                                                                  \
        }                                                                                          \
    }                                                                                              \
    static_assert(true)

    DECL_OPT(==);
    DECL_OPT(!=);
    DECL_OPT(<);
    DECL_OPT(<=);
    DECL_OPT(>);
    DECL_OPT(>=);
#undef DECL_OPT

#define DECL_OPT(Op, NullLeftValue, NullRightValue)                                                \
    template <typename Other, typename = std::enable_if_t<!is_opt_ref<Other>::value>>              \
    constexpr friend bool operator Op(opt_ref lhs, const Other& rhs) noexcept {                    \
        if (!lhs) {                                                                                \
            return NullLeftValue;                                                                  \
        } else {                                                                                   \
            return *lhs Op rhs;                                                                    \
        }                                                                                          \
    }                                                                                              \
    template <typename Other, typename = std::enable_if_t<!is_opt_ref<Other>::value>>              \
    constexpr friend bool operator Op(const Other& lhs, opt_ref rhs) noexcept {                    \
        if (!rhs) {                                                                                \
            return NullRightValue;                                                                 \
        } else {                                                                                   \
            return lhs Op * rhs;                                                                   \
        }                                                                                          \
    }                                                                                              \
    static_assert(true)

    DECL_OPT(==, false, false);
    DECL_OPT(!=, true, true);
    DECL_OPT(<, true, false);
    DECL_OPT(<=, true, false);
    DECL_OPT(>, false, true);
    DECL_OPT(>=, false, true);
#undef DECL_OPT
};

template <typename T>
opt_ref(T&& what) -> opt_ref<std::remove_reference_t<T>>;

}  // namespace neo
