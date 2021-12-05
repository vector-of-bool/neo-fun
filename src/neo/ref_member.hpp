#pragma once

#include "./addressof.hpp"

namespace neo {

template <typename T>
class ref_member;

namespace rm_detail {

template <typename T>
T& convert(T& t) noexcept {
    return t;
}

template <typename T>
void convert(T&&) = delete;

template <typename From, typename To>
concept rebind_okay = requires(From&& item) {
    convert<To>(static_cast<From&&>(item));
};

template <typename T, typename U>
constexpr bool alike_type_v = false;

template <typename T>
constexpr bool alike_type_v<T, T> = true;

template <typename T>
constexpr bool alike_type_v<T, T&> = true;

template <typename T>
constexpr bool alike_type_v<T, const T&> = true;

template <typename T>
constexpr bool alike_type_v<T, const T&&> = true;

template <typename T>
struct wrap_ref_member {
    using type = T;
};

template <typename T>
struct wrap_ref_member<T&> {
    using type = ref_member<T>;
};

}  // namespace rm_detail

/**
 * @brief A simple reference-wrapping type, similar to reference_wrapper, but no <functional>
 * machinery is pulled in as a dependency
 *
 * @tparam T The referred-to type
 */
template <typename T>
class ref_member {
    T* _ptr;

public:
    using type = T;

    template <rm_detail::rebind_okay<type> U>
    requires(!rm_detail::alike_type_v<ref_member, U>)  //
        constexpr ref_member(U&& arg) noexcept(
            noexcept(rm_detail::convert<type>(static_cast<U&&>(arg))))
        : _ptr(neo::addressof(rm_detail::convert(static_cast<U&&>(arg)))) {}

    constexpr ref_member(const ref_member&) noexcept = default;
    constexpr ref_member& operator=(const ref_member&) noexcept = default;

    /// Obtain the referred-to object reference
    constexpr T& get() const noexcept { return *_ptr; }
    constexpr    operator T&() const noexcept { return *_ptr; }
};

/**
 * @brief If the given T is a reference, type is ref_member<remove_reference_t<T>>,
 * otherwise just T
 */
template <typename T>
using wrap_ref_member_t = typename rm_detail::wrap_ref_member<T>::type;

}  // namespace neo
