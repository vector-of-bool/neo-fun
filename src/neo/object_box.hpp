#pragma once

#include "./addressof.hpp"
#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./like.hpp"
#include "./type_traits.hpp"

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
 * For `void`, is empty and semiregular.
 */
template <typename T>
class object_box;

/**
 * Implementation of object_box for (most) object types, excluding unbounded arrays
 */
template <object_type T>
    requires(not unbounded_array_type<T>)
class object_box<T> {
    // The contained object
    NEO_NO_UNIQUE_ADDRESS T _value;

    // Tag to construct from an array
    struct _array_construct {};

    // In-place construct an array member from a given array
    template <typename Arr, std::size_t... Idx>
    constexpr object_box(_array_construct, Arr&& arr, std::index_sequence<Idx...>) noexcept
        : _value{neo::forward_like<Arr>(arr[Idx])...} {}

public:
    object_box() = default;

    // clang-format off
    /**
     * @brief Construct from any type that is explicitly-convertible-to the underlying type.
     *
     * This constructor is explicit if the underlying conversion is never implicit
     */
    template <unalike<object_box> U>
        // Require the explicit conversion
        requires explicit_convertible_to<U&&, T>
        // We are explicit if the implicit conversion is not viable
        explicit(not implicit_convertible_to<U&&, T>)
    constexpr object_box(U&& arg)
        noexcept(noexcept(T(NEO_FWD(arg))))
        : _value(NEO_FWD(arg)) {}

    /**
     * @brief Array-move-constructor. If the underlying type is an array, construct
     * from an array of values of a type that is convertible to element type of
     * the wrapped array.
     *
     * @tparam Elem An element type from which to construct. Must be convertible
     * to the underlying array's element type.
     * @tparam Len The deduced length of the array argument.
     */
    template <typename Elem, std::size_t Len>
    requires array_type<T>
         and convertible_to<Elem, remove_all_extents_t<T>>
    constexpr explicit object_box(Elem (&&arr)[Len]) noexcept
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<Len>{}) {}

    /**
     * @brief Array-copy-constructor. If the underlying type is an array, construct
     * from an array of values of a type that is convertible to element type of
     * the wrapped array.
     *
     * @tparam Elem An element type from which to construct. Must be convertible
     * to the underlying array's element type.
     * @tparam Len The deduced length of the array argument.
     */
    template <typename Elem, std::size_t Len>
    requires array_type<T>
         and convertible_to<Elem, remove_all_extents_t<T>>
    constexpr explicit object_box(Elem const (&arr)[Len]) noexcept
        : object_box(_array_construct{}, NEO_FWD(arr), std::make_index_sequence<Len>{}) {}
    // clang-format on

    /// Obtain a reference to the boxed value.
    [[nodiscard]] constexpr T&        get() & noexcept { return _value; }
    [[nodiscard]] constexpr const T&  get() const& noexcept { return _value; }
    [[nodiscard]] constexpr T&&       get() && noexcept { return NEO_MOVE(_value); }
    [[nodiscard]] constexpr const T&& get() const&& noexcept { return NEO_MOVE(_value); }

    /**
     * @brief Obtain the boxed value as-if by forwarding reference.
     */
    [[nodiscard]] constexpr T&& take() noexcept { return NEO_MOVE(_value); }

    constexpr T*       operator->() noexcept { return neo::addressof(_value); }
    constexpr const T* operator->() const noexcept { return neo::addressof(_value); }

    constexpr void friend do_repr(auto out, const object_box* self) noexcept {
        out.type("[boxed {}]", out.template repr_type<T>("?"));
        if (self) {
            out.value("{}", out.repr_value(self->get(), "?"));
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
};

/**
 * @brief Implementation of object_box for void types.
 *
 * This object_box is default-constructible and empty
 */
template <void_type Void>
class object_box<Void> {
public:
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
