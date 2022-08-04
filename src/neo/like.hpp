#pragma once

#include <type_traits>

namespace neo {

namespace like_detail {

template <bool B>
struct conditional {
    template <typename T, typename F>
    using eval = F;
};

template <>
struct conditional<true> {
    template <typename T, typename F>
    using eval = T;
};

template <typename T, typename U>
concept similar = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template <typename From, typename Onto>
struct copy_ref {
    using type = Onto;
};

template <typename From, typename Onto>
struct copy_ref<From&&, Onto> {
    using type = Onto&&;
};

template <typename From, typename Onto>
struct copy_ref<From&, Onto> {
    using type = Onto&;
};

template <typename From, typename Onto>
using copy_ref_t = typename copy_ref<From, Onto>::type;

template <bool IsRValRef>
struct override_ref {
    template <typename T>
    using eval = T&;
};

template <>
struct override_ref<true> {
    template <typename T>
    using eval = std::remove_reference_t<T>&&;
};

template <typename From, typename Onto>
using override_ref_t = typename override_ref<std::is_rvalue_reference_v<From>>::template eval<Onto>;

template <bool IsConst>
struct copy_const;

template <>
struct copy_const<true> {
    template <typename T>
    using eval = copy_ref_t<T, std::remove_reference_t<T> const>;
};

template <>
struct copy_const<false> {
    template <typename T>
    using eval = T;
};

template <typename From, typename To>
using copy_const_t =
    typename copy_const<std::is_const_v<std::remove_reference_t<From>>>::template eval<To>;

template <typename From, typename To>
using copy_cvref_t = copy_ref_t<From&&, copy_const_t<From, To>>;

}  // namespace like_detail

/**
 * @brief Merge the cvref-qualifiers of `T` onto `U`
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
using forward_like_t
    = like_detail::override_ref_t<T&&, like_detail::copy_const_t<T, std::remove_reference_t<U>>>;

template <typename T>
constexpr decltype(auto) forward_like(auto&& arg) noexcept {
    return static_cast<forward_like_t<T, decltype(arg)>>(arg);
}

}  // namespace neo
