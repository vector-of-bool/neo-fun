#pragma once

namespace neo {

namespace unref_detail {

template <typename T, typename U>
constexpr bool same_type_v = false;

template <typename T>
constexpr bool same_type_v<T, T> = true;

// clang-format off
template <typename R>
concept check_call_convert =
    requires(R const ref, void (&func)(typename R::type&) noexcept) {
        { func(ref) } noexcept;
    };

template <typename R>
concept like_a_reference_wrapper =
    requires(R const ref, R mref) {
        typename R::type;
        requires !same_type_v<R, typename R::type>;
        { mref = ref } noexcept;
        { ref.get() } noexcept;
        { static_cast<typename R::type&>(ref) } noexcept;
        requires same_type_v<decltype(ref.get()), typename R::type&>;
        requires check_call_convert<R>;
    };

template <typename T>
constexpr bool like_a_reference_wrapper_v = like_a_reference_wrapper<T>;

template <typename T>
constexpr bool like_a_reference_wrapper_v<T&> = like_a_reference_wrapper_v<T>;

template <typename T>
constexpr bool like_a_reference_wrapper_v<const T&> = like_a_reference_wrapper_v<T>;

template <typename T>
constexpr bool like_a_reference_wrapper_v<const T&&> = like_a_reference_wrapper_v<T>;

}  // namespace unref_detail

/**
 * @brief Un-wraps a reference_wrapper-like type. Returns a reference to the referred-to
 * object. If the given object is not a reference_wrapper-like, returns a reference
 * to the argument.
 */
template <typename T>
constexpr decltype(auto) unref(T&& t) noexcept {
    if constexpr (unref_detail::like_a_reference_wrapper_v<T>) {
        return t.get();
    } else {
        return static_cast<T&&>(t);
    }
}

/**
 * @brief Declare a set of getter-methods that return a member variable after passing it through
 * neo::unref(). This is meant to be used in conjunction with wrap_ref_member_t
 */
#define NEO_DECL_UNREF_GETTER(FuncName, MemName)                                                   \
    constexpr decltype(auto) FuncName()& noexcept { return ::neo::unref(this->MemName); }          \
    constexpr decltype(auto) FuncName() const& noexcept { return ::neo::unref(this->MemName); }    \
    constexpr decltype(auto) FuncName()&& noexcept {                                               \
        return ::neo::unref(static_cast<decltype(this->MemName)&&>(this->MemName));                \
    }                                                                                              \
    constexpr decltype(auto) FuncName() const&& noexcept {                                         \
        return ::neo::unref(static_cast<decltype(this->MemName)&&>(this->MemName));                \
    }                                                                                              \
    static_assert(true)

/**
 * @brief Obtain the reference type returned by neo::unref(T)
 */
template <typename T>
using unref_t = decltype(unref(((T(*)())(void*)(nullptr))()));

}  // namespace neo
