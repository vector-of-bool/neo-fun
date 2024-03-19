#pragma once

#include "./addressof.hpp"
#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./like.hpp"
#include "./type_traits.hpp"

#include <compare>
#include <type_traits>
#include <utility>

namespace neo {

/**
 * @brief Defines a object type that contains a (possibly non-object) type.
 *
 * @tparam T Any scalar type, object type, reference type, void type, bounded array type,
 * or any cv-qualified type thereof. Unbounded arrays are not supported.
 *
 * For object types, inherits the construct/assign requirements of the wrapped
 * type.
 *
 * For references, is trivially copyable, but not default-constructible.
 *
 * Defines operator== and operator<=> for applicable types. For references, the
 * operators are defined on the equivalent pointer values. For void, all instances
 * are equal.
 *
 * For `void`, is empty and regular.
 */
template <typename T>
class object_box;

/**
 * Implementation of object_box for non-array object types
 */
template <typename T>
    requires object_type<T> and (not array_type<T>)
class object_box<T> {
    NEO_NO_UNIQUE_ADDRESS T _value;

public:
    object_box() = default;

    // Defer to the default operators
    bool operator==(const object_box&) const  = default;
    auto operator<=>(const object_box&) const = default;

    /**
     * @brief Construct from any type that is explicitly-convertible-to the underlying type.
     *
     * This constructor is explicit if the underlying conversion is never implicit
     */
    template <unalike<object_box> U>
    // We are explicit if the implicit conversion is not viable
    explicit(not implicit_convertible_to<U&&, T>) constexpr object_box(U&& arg)
        NEO_CONSTRUCTS(object_box(std::in_place, NEO_FWD(arg))) {}

    /**
     * @brief In-place constructor. Calls the underlying constructor directly.
     */
    template <typename... Args>
        requires constructible_from<T, Args...>
    constexpr explicit object_box(std::in_place_t,
                                  Args&&... args)  //
        noexcept(noexcept(T(NEO_FWD(args)...)))
        : _value(NEO_FWD(args)...) {}

    /// Obtain a reference to the boxed value.
    [[nodiscard]] constexpr T&        get() & noexcept { return _value; }
    [[nodiscard]] constexpr const T&  get() const& noexcept { return _value; }
    [[nodiscard]] constexpr T&&       get() && noexcept { return NEO_MOVE(_value); }
    [[nodiscard]] constexpr const T&& get() const&& noexcept { return NEO_MOVE(_value); }

    /**
     * @brief Obtain the boxed value as-if by forwarding reference.
     */
    [[nodiscard]] constexpr T&& take() noexcept { return NEO_MOVE(_value); }

    constexpr void friend do_repr(auto out, const object_box* self) noexcept {
        out.type("[boxed {}]", out.template repr_type<T>("?"));
        if (self) {
            out.value("{}", out.repr_value(self->get(), "?"));
        }
    }

    friend void swap(object_box& left,
                     object_box& right) noexcept(noexcept(neo::swap(left._value, right._value)))
        requires requires { neo::swap(left._value, right._value); }
    {
        neo::swap(left._value, right._value);
    }
};

// Implementation of object_box for array types
template <typename Element, std::size_t Length>
class object_box<Element[Length]> {
    using array_type = Element[Length];

    // The contained object
    NEO_NO_UNIQUE_ADDRESS array_type _array;

    // Tag to construct from an array
    struct _array_construct {};

    // In-place construct an array member from a given array
    template <typename Arr, std::size_t... Idx>
    constexpr object_box(_array_construct, Arr&& arr, std::index_sequence<Idx...>) noexcept
        : _array{neo::forward_like<Arr>(arr[Idx])...} {}

public:
    object_box() = default;

    /**
     * @brief In-place constructor for constructing array elements.
     *
     * @param args The values for the array elements. Each element is constructed in-place via
     * conversion It is required that `args` be the same number of arguments as array elements.
     */
    template <convertible_to<Element>... Args>
        requires(sizeof...(Args) <= Length)
    constexpr explicit object_box(std::in_place_t, Args&&... args)
        // We are noexcept if each element conversion is noexcept
        noexcept((noexcept(static_cast<Element>(NEO_FWD(args))) and ...))
        : _array{NEO_FWD(args)...} {}

    /**
     * @brief Array-move-constructor. If the underlying type is an array, construct
     * from an array of values of a type that is convertible to element type of
     * the wrapped array.
     *
     * @tparam Elem An element type from which to construct. Must be convertible
     * to the underlying array's element type.
     * @tparam Len The deduced length of the array argument.
     */
    template <typename FromElement, std::size_t OtherLen>
        requires convertible_to<FromElement, Element>
    constexpr explicit object_box(FromElement (&&arr)[OtherLen]) noexcept(
        noexcept(static_cast<Element>(NEO_DECLVAL(FromElement&&))))
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<OtherLen>{}) {}

    /**
     * @brief Array-copy-constructor. If the underlying type is an array, construct
     * from an array of values of a type that is convertible to element type of
     * the wrapped array.
     *
     * @tparam Elem An element type from which to construct. Must be convertible
     * to the underlying array's element type.
     * @tparam Len The deduced length of the array argument.
     */
    template <typename FromElement, std::size_t OtherLength>
        requires convertible_to<FromElement, Element>
    constexpr explicit object_box(FromElement const (&arr)[OtherLength]) noexcept(
        noexcept(static_cast<Element>(NEO_DECLVAL(const FromElement&))))
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<OtherLength>{}) {}

    // in-place constructor for moving arrays
    template <typename FromElement, std::size_t Len>
        requires convertible_to<FromElement, Element>
    constexpr explicit object_box(std::in_place_t, FromElement (&&arr)[Len]) noexcept(
        noexcept(static_cast<Element>(NEO_DECLVAL(FromElement&&))))
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<Len>{}) {}

    // in-place constructor for copying arrays
    template <typename FromElement, std::size_t Len>
        requires convertible_to<FromElement, Element>
    constexpr explicit object_box(std::in_place_t, const FromElement (&arr)[Len]) noexcept(
        noexcept(static_cast<Element>(NEO_DECLVAL(const FromElement&))))
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<Len>{}) {}

    /// Obtain a reference to the boxed value.
    [[nodiscard]] constexpr array_type&        get() & noexcept { return _array; }
    [[nodiscard]] constexpr const array_type&  get() const& noexcept { return _array; }
    [[nodiscard]] constexpr array_type&&       get() && noexcept { return NEO_MOVE(_array); }
    [[nodiscard]] constexpr const array_type&& get() const&& noexcept { return NEO_MOVE(_array); }

    /**
     * @brief Obtain the boxed value as-if by forwarding reference.
     */
    [[nodiscard]] constexpr array_type&& take() noexcept { return NEO_MOVE(_array); }

    constexpr void friend do_repr(auto out, const object_box* self) noexcept {
        out.type("[boxed {}]", out.template repr_type<array_type>("?"));
        if (self) {
            out.value("{}", out.repr_value(self->get(), "?"));
        }
    }

    // operator== used for arrays of equality-comparable values
    constexpr bool operator==(const object_box& other) const noexcept
        requires equality_comparable<remove_all_extents_t<array_type>>
    {
        return _equal(this->_array, other._array);
    }

    // operator<=> used for arrays of totally ordered values
    constexpr auto operator<=>(const object_box& other) const noexcept
        requires totally_ordered<remove_all_extents_t<array_type>>
    {
        using Cat = std::common_comparison_category_t<
            std::compare_three_way_result_t<std::remove_all_extents_t<array_type>>>;
        return _compare<Cat>(this->_array, other._array);
    }

private:
    template <typename E>
    constexpr bool _equal(const E& left, const E& right) const noexcept {
        if constexpr (not neo::array_type<E>) {
            const bool eq = left == right;
            return eq;
        } else {
            // Compare the two arrays element-by-element
            auto       iter       = left + 0;
            auto       other_iter = right + 0;
            const auto stop       = iter + std::extent_v<E>;
            for (; iter != stop; ++iter, ++other_iter) {
                if (not _equal(*iter, *other_iter)) {
                    return false;
                }
            }
            return true;
        }
    }

    template <typename Cat, typename E>
    constexpr Cat _compare(const E& left, const E& right) const noexcept {
        if constexpr (not neo::array_type<E>) {
            Cat c = std::compare_strong_order_fallback(left, right);
            return c;
        } else {
            auto       l_iter = left + 0;
            auto       r_iter = right + 0;
            const auto stop   = l_iter + std::extent_v<E>;
            Cat        r      = std::strong_ordering::equal;
            for (; l_iter != stop and r == 0; ++l_iter, ++r_iter) {
                r = _compare<Cat>(*l_iter, *r_iter);
            }
            return r;
        }
    }
};

/**
 * @brief Implementation for reference types.
 *
 * Assigning to an object_box from another object box will rebind the reference.
 * @tparam Ref
 */
template <reference_type Ref>
class object_box<Ref> {
    using Pointer = add_pointer_t<Ref>;
    Pointer _ptr;

public:
    // boxed references are not default-constructible
    constexpr object_box() = delete;

    // Construct a boxed reference from a compatible referred-to type.
    template <typename U>
    // Require that we can pointer-convert to the stored type. Checking
    // for reference conversion would be unsafe since it could invoke an implicit conversion
    // constructor for materializing a temporary
        requires convertible_to<add_pointer_t<U>, Pointer>
    explicit constexpr object_box(U&& arg) noexcept
        : _ptr(neo::addressof(arg)) {}

    template <typename U>
        requires convertible_to<add_pointer_t<U>, Pointer>
    explicit constexpr object_box(std::in_place_t, U&& arg) noexcept
        : _ptr(NEO_ADDRESSOF(arg)) {}

    /// Obtain the boxed reference
    [[nodiscard]] constexpr Ref& get() const noexcept { return *_ptr; }

    /// Obtain the boxed reference.
    [[nodiscard]] constexpr Ref& take() noexcept { return *_ptr; }

    constexpr Pointer operator->() const noexcept { return _ptr; }

    constexpr void friend do_repr(auto out, const object_box* self) noexcept {
        out.type("[boxed {}]", out.template repr_type<Ref>("?&"));
        if (self) {
            out.value("{}", out.repr_value(self->get(), "?"));
        }
    }

    bool operator==(const object_box& other) const  = default;
    auto operator<=>(const object_box& other) const = default;
};

/**
 * @brief Implementation of object_box for void types.
 *
 * This object_box is default-constructible and empty
 */
template <void_type Void>
class object_box<Void> {
public:
    object_box() = default;
    constexpr explicit object_box(std::in_place_t) noexcept {}

    // Returns void for boxed void
    constexpr remove_cv_t<Void> get() const noexcept {}
    // Returns void for boxed void
    constexpr remove_cv_t<Void> take() noexcept {}

    constexpr void friend do_repr(auto out, const object_box* self) noexcept {
        out.type("[boxed {}]", out.template repr_type<Void>());
        if (self) {
            out.value("(void)");
        }
    }

    constexpr bool operator==(const object_box&) const noexcept  = default;
    constexpr auto operator<=>(const object_box&) const noexcept = default;
};

/**
 * @brief The sole argument deduction guide for object_box is designed to support
 * perfect forwarding of objects. If given an lvalue reference, the deduced box
 * is a boxed reference. If given an rvalue, deduces to a box of the value's type.
 *
 * This will never deduce `void`
 */
template <typename T>
object_box(T&& arg) -> object_box<T>;

}  // namespace neo
