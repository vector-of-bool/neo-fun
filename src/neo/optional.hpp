#pragma once

#include "./optional.detail.hpp"

#include "./addressof.hpp"
#include "./attrib.hpp"
#include "./concepts.hpp"
#include "./constexpr_union.hpp"
#include "./declval.hpp"
#include "./emplacement.hpp"
#include "./invoke.hpp"
#include "./object_t.hpp"
#include "./returns.hpp"
#include "./storage.hpp"
#include "./swap.hpp"
#include "./type_traits.hpp"

#include <compare>
#include <cstddef>
#include <type_traits>

namespace neo {

/**
 * @brief Provides a stronger, leaner, and faster-compiling version of std::optional
 *
 * This optional supports any type that provides an instance of optional_traits,
 * which by default includes any type that supports object_box (including `void` and reference
 * types).
 *
 * @tparam T The type held by the optional
 */
template <typename T>
class optional : public opt_detail::adl_operators {
    // optional_traits for the optional
    using traits = optional_traits<T>;

    static_assert(opt_detail::valid_optional_traits<traits, T>);

    // The stored state type, obtained from the traits
    using state_type = traits::state_type;

    // The state that is held be the optional. Passed to/from the traits for operations
    NEO_NO_UNIQUE_ADDRESS state_type _state = state_type();

    /// If we do not currently hold any value, throws bad_optional_access
    constexpr void _check_has_value() const {
        if (not has_value()) {
            opt_detail::throw_bad_optional();
        }
    }

    /// If we do not currently hold any value, terminate the program.
    constexpr void _assert_has_value() const noexcept {
        if (not has_value()) {
            opt_detail::terminate_bad_optional();
        }
    }

    /// Perform a copy-construct/copy-assignment, which may include moving, depending
    /// on the cvref of "other_storage" and the behavior of our optional_traits
    constexpr void _assign(auto&& other_storage) {
        if (not has_value()) {
            // We have no value. We will take on the value of the other
            if (traits::has_value(other_storage)) {
                // Ther other has a value. Do a copy (may move):
                traits::copy(_state, NEO_FWD(other_storage));
            } else {
                // Both are empty. Nothing to do
            }
        } else {
            // We have a value. We may assigned over our own
            if (traits::has_value(other_storage)) {
                // Assign our value over the other's value
                traits::assign(_state, NEO_FWD(other_storage));
            } else {
                // Other is empty, so we'll become empty too
                traits::destroy(_state);
            }
        }
    }

    template <typename Self,
              typename Fn,
              typename Ret = invoke_result_t<Fn, decltype(*NEO_DECLVAL(Self))>>
    static constexpr Ret do_and_then(Self&& self, Fn&& fn) {
        static_assert(is_optional_v<Ret>,
                      "and_then() requires that the operation returns an optional. Did you mean to "
                      "use opt.transform()?");
        if (self.has_value()) {
            return NEO_INVOKE(NEO_FWD(fn), *NEO_FWD(self));
        } else {
            return {};
        }
    }

    template <typename Self,
              typename Fn,
              typename Ret = invoke_result_t<Fn, decltype(*NEO_DECLVAL(Self))>>
    static constexpr optional<Ret> do_transform(Self&& self, Fn&& fn) {
        if (self.has_value()) {
            return optional<Ret>(NEO_INVOKE(NEO_FWD(fn), *NEO_FWD(self)));
        } else {
            return optional<Ret>{};
        }
    }

public:
    using value_type = T;
    /// The lvalue-reference type that we return
    using reference = add_lvalue_reference_t<T>;
    /// The lvalue-reference-to-const type that we return
    using const_reference = add_const_reference_t<T>;
    /// The rvalue-reference type that we return from dereferencing an rvalue optional
    using rvalue_reference = add_rvalue_reference_t<T>;
    /// The rvalue-reference type that we return from dereferencing a const rvalue optional
    using const_rvalue_reference = add_rvalue_reference_t<add_const_t<T>>;
    /// The pointer type that we return from operator->()
    using pointer = add_pointer_t<T>;
    /// The pointer type that we return from operator->() const
    using const_pointer = add_const_pointer_t<T>;

    /// Default-constructs as a disengaged optional
    constexpr optional() noexcept = default;
    /// Simply constructs as a null optional
    constexpr optional(nullopt_t) noexcept {}

    constexpr ~optional() = default;
    constexpr ~optional()
        requires(not trivially_destructible<T>)
    {
        reset();
    }

    /// Copy-construct an optional from an optional of the same type
    constexpr optional(const optional&) noexcept
        requires traits::trivial_copy
    = default;
    constexpr optional(const optional& o) noexcept(nothrow_constructible_from<T, const_reference>)
        requires copy_constructible<T> and (not traits::trivial_copy)
    {
        _assign(o._state);
    }

    /// Move-construct an optional from an optional of the same type
    constexpr optional(optional&&) noexcept
        requires traits::trivial_move
    = default;
    constexpr optional(optional&& o) noexcept(nothrow_constructible_from<T, rvalue_reference>)
        requires move_constructible<T> and (not traits::trivial_move)
    {
        _assign(NEO_MOVE(o)._state);
    }

    // clang-format off
    /**
     * @brief Copy-construct from an optional of a differing type
     */
    template <typename OtherOpt, typename U = remove_reference_t<OtherOpt>::value_type>
        requires is_optional_v<remove_cvref_t<OtherOpt>>
    explicit(not convertible_to<forward_like_tuple_t<OtherOpt, U>, T>)  //
    constexpr optional(OtherOpt&& other)                            //
        noexcept(nothrow_constructible_from<T, decltype(*NEO_FWD(other))>)
    requires constructible_from<T, decltype(*NEO_FWD(other))>
         and (alike<T, bool>
              or not (
                    constructible_from<T, OtherOpt&>
                and constructible_from<T, OtherOpt const&>
                and constructible_from<T, OtherOpt const &&>
                and constructible_from<T, OtherOpt &&>
                and convertible_to<OtherOpt&, T>
                and convertible_to<OtherOpt const&, T>
                and convertible_to<OtherOpt const &&, T>
                and convertible_to<OtherOpt &&, T>
                )
            )
    // clang-format on
    {
        if (not other.has_value()) {
            // Okay: Do nothing (we are empty)
        } else {
            traits::construct(_state, *NEO_FWD(other));
        }
    }

    template <typename... Args>
    constexpr explicit optional(std::in_place_t,
                                Args&&... args)  //
        noexcept(nothrow_constructible_from<T, Args...>)
        requires constructible_from<T, Args...>
    {
        traits::construct(_state, NEO_FWD(args)...);
    }

    constexpr explicit optional(std::in_place_t) noexcept
        requires void_type<T>
    {
        traits::construct(_state);
    }

    template <typename U>
    constexpr explicit(not convertible_to<U&&, T>)
        optional(U&& arg) noexcept(nothrow_constructible_from<U&&, T>)
        requires constructible_from<T, U&&>  //
        and unalike<U, std::in_place_t>      //
        and (not is_optional_v<remove_cvref_t<U>>)
    {
        traits::construct(_state, NEO_FWD(arg));
    }

    constexpr optional& operator=(const optional&) noexcept
        requires traits::trivial_copy_assign
    = default;
    constexpr optional&
    operator=(const optional& o) noexcept(noexcept(traits::copy(_state, o._state))  //
                                              and noexcept(traits::assign(_state, o._state)))
        requires copy_assignable<T> and (not traits::trivial_copy_assign)
    {
        _assign(NEO_MOVE(o)._state);
        return *this;
    }

    constexpr optional& operator=(optional&&) noexcept
        requires traits::trivial_move_assign
    = default;
    constexpr optional&
    operator=(optional&& o) noexcept(noexcept(traits::copy(_state, NEO_MOVE(o)._state))  //
                                         and noexcept(traits::assign(_state, NEO_MOVE(o)._state)))
        requires move_assignable<T> and (not traits::trivial_move_assign)
    {
        _assign(NEO_MOVE(o)._state);
        return *this;
    }

    constexpr reference operator*() & noexcept {
        _assert_has_value();
        return traits::get(_state);
    }
    constexpr const_reference operator*() const& noexcept {
        _assert_has_value();
        return traits::get(_state);
    }

    constexpr rvalue_reference operator*() && noexcept {
        _assert_has_value();
        return (rvalue_reference)traits::get(_state);
    }
    constexpr const_rvalue_reference operator*() const&& noexcept {
        _assert_has_value();
        return (const_rvalue_reference)traits::get(_state);
    }

    constexpr const_pointer operator->() const noexcept { return NEO_ADDRESSOF(**this); }
    constexpr pointer       operator->() noexcept { return NEO_ADDRESSOF(**this); }

    constexpr explicit operator bool() const noexcept { return has_value(); }
    constexpr bool     has_value() const noexcept { return traits::has_value(_state); }

    constexpr auto value() & -> reference {
        _check_has_value();
        return **this;
    }

    constexpr auto value() && -> rvalue_reference {
        _check_has_value();
        return *NEO_MOVE(*this);
    }

    constexpr auto value() const& -> const_reference {
        _check_has_value();
        return **this;
    }

    constexpr auto value() const&& -> const_rvalue_reference {
        _check_has_value();
        return *NEO_MOVE(*this);
    }

    template <typename U>
    constexpr auto value_or(U&& dflt) & -> common_reference_t<reference, U&&> {
        if (has_value()) {
            return **this;
        } else {
            return NEO_FWD(dflt);
        }
    }

    template <typename U>
    constexpr auto value_or(U&& dflt) const& -> common_reference_t<const_reference, U&&> {
        if (has_value()) {
            return **this;
        } else {
            return NEO_FWD(dflt);
        }
    }

    template <typename U>
    constexpr auto value_or(U&& dflt) && -> common_reference_t<rvalue_reference, U&&> {
        if (has_value()) {
            return *NEO_MOVE(*this);
        } else {
            return NEO_FWD(dflt);
        }
    }

    template <typename U>
    constexpr auto value_or(U&& dflt) const&& -> common_reference_t<const_rvalue_reference, U&&> {
        if (has_value()) {
            return *NEO_MOVE(*this);
        } else {
            return NEO_FWD(dflt);
        }
    }

    /**
     * @brief If this optional currently holds a value, returns a pointer to that
     * value. Otherwise, returns nullptr
     */
    constexpr pointer as_pointer() noexcept {
        return has_value() ? NEO_ADDRESSOF(**this) : nullptr;
    }
    constexpr const_pointer as_pointer() const noexcept {
        return has_value() ? NEO_ADDRESSOF(**this) : nullptr;
    }

    /**
     * @brief Swap the value contained between two optionals.
     *
     * Requires that the values be swappable
     *
     * @param other The optional that will be swapped.
     */
    constexpr void swap(optional& other)                                 //
        noexcept(noexcept(traits::copy(_state, NEO_MOVE(other._state)))  //
                     and noexcept(traits::swap(_state, _state)))
        requires requires {
            traits::copy(_state, NEO_MOVE(_state));
            traits::swap(_state, other._state);
        }
    {
        if (not has_value()) {
            if (other.has_value()) {
                traits::copy(_state, NEO_MOVE(other._state));
            } else {
                // No one has a value. Nothing to do
            }
        } else {
            if (other.has_value()) {
                traits::swap(_state, other._state);
            } else {
                other.swap(*this);
            }
        }
    }

    /**
     * @brief Clear the value held by this optional, if non-empty.
     */
    constexpr void reset() noexcept {
        if (has_value()) {
            traits::destroy(_state);
        }
    }

    /**
     * @brief In-place contsruct a new value within the optional.
     *
     * If the optional held a value, the existing value is destroyed.
     *
     * @param args The arguments to the constructor of the underlying T
     * @return A reference to the underlying constructed value.
     */
    template <typename... Ts>
    constexpr reference
    emplace(Ts&&... args) noexcept(noexcept(traits::construct(_state, NEO_FWD(args)...)))
        requires requires { traits::construct(_state, NEO_FWD(args)...); }
    {
        reset();
        traits::construct(_state, NEO_FWD(args)...);
        return traits::get(_state);
    }

    template <typename F>
        constexpr auto and_then(F&& fun) &  //
        NEO_RETURNS(this->do_and_then(*this, NEO_FWD(fun)));

    template <typename F>
    constexpr auto and_then(F&& fun) const&  //
        NEO_RETURNS(this->do_and_then(*this, NEO_FWD(fun)));

    template <typename F>
        constexpr auto and_then(F&& fun) &&  //
        NEO_RETURNS(this->do_and_then(NEO_MOVE(*this), NEO_FWD(fun)));

    template <typename F>
    constexpr auto and_then(F&& fun) const&&  //
        NEO_RETURNS(this->do_and_then(NEO_MOVE(*this), NEO_FWD(fun)));

    template <typename F>
        constexpr auto transform(F&& fn) &  //
        NEO_RETURNS(this->do_transform(*this, NEO_FWD(fn)));
    template <typename F>
    constexpr auto transform(F&& fn) const&  //
        NEO_RETURNS(this->do_transform(*this, NEO_FWD(fn)));
    template <typename F>
        constexpr auto transform(F&& fn) &&  //
        NEO_RETURNS(this->do_transform(NEO_MOVE(*this), NEO_FWD(fn)));
    template <typename F>
    constexpr auto transform(F&& fn) const&&  //
        NEO_RETURNS(this->do_transform(NEO_MOVE(*this), NEO_FWD(fn)));

    template <typename F>
    constexpr optional or_else(F&& fn) const&
        requires copy_constructible<T>
    {
        if (has_value()) {
            return *this;
        } else {
            return NEO_INVOKE(NEO_FWD(fn));
        }
    }

    template <typename F>
    constexpr optional or_else(F&& fn) &&
        requires movable<T>
    {
        if (has_value()) {
            return *NEO_FWD(*this);
        } else {
            return NEO_INVOKE(NEO_FWD(fn));
        }
    }
};

template <typename T>
explicit optional(const T&) -> optional<T>;

/**
 * @brief The default state type for optional values
 *
 * @tparam T The stored type
 */
template <typename T>
struct default_optional_state {
public:
    NEO_NO_UNIQUE_ADDRESS constexpr_union<object_t<T>> onion;
    bool                                               has_value = false;

    default_optional_state() = default;
};

template <typename T>
struct optional_traits {
    using state_type = default_optional_state<T>;

    static constexpr bool trivial_copy        = copy_constructible<state_type>;
    static constexpr bool trivial_move        = move_constructible<state_type>;
    static constexpr bool trivial_copy_assign = trivially_copyable<state_type>;
    static constexpr bool trivial_move_assign = trivially_movable<state_type>;

    constexpr static bool has_value(const state_type& st) noexcept { return st.has_value; }
    constexpr static add_lvalue_reference_t<T> get(state_type& st) noexcept {
        return static_cast<add_lvalue_reference_t<T>>(st.onion._0);
    }
    constexpr static add_const_reference_t<T> get(const state_type& st) noexcept {
        return static_cast<add_const_reference_t<T>>(st.onion._0);
    }
    constexpr static void destroy(state_type& st) noexcept {
        st.onion.template destroy<0>();
        st.has_value = false;
    }
    template <typename... Args>
    constexpr static void
    construct(state_type& st, Args&&... args) noexcept(nothrow_constructible_from<T, Args...>) {
        st.onion.template construct<0>(NEO_FWD(args)...);
        st.has_value = true;
    }

    template <typename O>
    constexpr static void copy(state_type& into, O&& from)  //
        noexcept(nothrow_constructible_from<T,
                                            decltype(NEO_FWD(from).onion._0)>)  //
    {
        construct(into, NEO_FWD(from).onion._0);
    }

    template <typename O>
    constexpr static void assign(state_type& into, O&& from)                         //
        noexcept(std::is_nothrow_assignable_v<T, decltype(NEO_FWD(from).onion._0)>)  //
    {
        into.onion._0 = NEO_FWD(from).onion._0;
    }

    constexpr static void swap(state_type& a, state_type& b) { neo::swap(a.onion._0, b.onion._0); }
};

/**
 * @brief Specialization for reference types
 */
template <reference_type Ref>
struct optional_traits<Ref> {
    using state_type = remove_reference_t<Ref>*;

    static constexpr bool trivial_copy        = true;
    static constexpr bool trivial_move        = true;
    static constexpr bool trivial_copy_assign = true;
    static constexpr bool trivial_move_assign = true;

    constexpr static bool has_value(state_type s) noexcept { return s != nullptr; }
    constexpr static Ref& get(state_type s) noexcept { return *s; }

    constexpr static void destroy(state_type& s) noexcept { s = nullptr; }
    constexpr static void construct(state_type& into, Ref from) noexcept {
        into = NEO_ADDRESSOF(from);
    }

    // copy/assign/swap are not needed, since we are trivially copy/move-able
};

/**
 * @brief Provides constexpr-ready storage for an object of type 'T'.
 *
 * The storage begins uninitialized. The lifetime of the `.value` members must
 * be managed manually.
 *
 * @tparam T The type that is stored in the union.
 */
template <typename T>
union [[deprecated("Prefer neo::storage_for<T>")]] nano_opt_storage {
    /// A valueless union member. The initial value
    NEO_NO_UNIQUE_ADDRESS unit _empty{};
    // The storage that is managed by this union.
    using boxed_t = object_box<T>;
    NEO_NO_UNIQUE_ADDRESS boxed_t box;

    /**
     * @brief Construct a value in-place.
     */
    template <typename... Args>
    constexpr explicit nano_opt_storage(std::in_place_t, Args&&... args)  //
        noexcept(nothrow_constructible_from<T, Args...>)                  //
        requires constructible_from<T, Args...>                           //
        : box((Args&&)(args)...) {}

    /// Default-construct as disenganged
    constexpr nano_opt_storage() = default;
    constexpr nano_opt_storage()
        requires(not trivially_default_constructible<T>)
    {}

#if !NEO_COMPILER(Clang)
#define NEO_NANO_OPT_SUPPORTS_TRIVIAL_DESTRUCTIBILITY 1
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage()
        requires(not trivially_destructible<T>)
    {}
#else
#define NEO_NANO_OPT_SUPPORTS_TRIVIAL_DESTRUCTIBILITY 0
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt_storage() {}
#endif

    void destroy() noexcept { neo::destroy_at(&box); }
};

/**
 * @brief An extremely minimal optional-lite class.
 *
 * @tparam T The type stored by this optional
 */
template <typename T>
class nano_opt {
    using storage_type = storage_for<T>;

    storage_type _storage;
    bool         _active = false;

    void _destroy_nocheck() noexcept {
        _storage.destroy();
        _active = false;
    }

public:
    // Default-construct as dis-engaged
    constexpr nano_opt() = default;

    // Construct as a new T
    template <typename Arg>
        requires weak_same_as<remove_cvref_t<Arg>, T>  //
    constexpr nano_opt(Arg&& arg) noexcept(nothrow_constructible_from<T, Arg>) {
        this->emplace(NEO_FWD(arg));
    }

    // Construct as a new T with the given arguments
    template <typename... Args>
    constexpr explicit nano_opt(std::in_place_t, Args&&... args)  //
        noexcept(nothrow_constructible_from<T, Args...>)          //
        requires constructible_from<T, Args...>                   //
    {
        this->emplace(NEO_FWD(args)...);
    }

    // Pick the type of destructor we want based on whether the underlying type is triviallty
    // destructible
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~nano_opt()
        requires(not trivially_destructible<T>)
    {
        if (has_value()) {
            _destroy_nocheck();
        }
    }

    // Pick the type of copy we want based on whether the underlying type is trivially copyable
    constexpr nano_opt(const nano_opt&) = default;
    // If not trivial copy:
    constexpr nano_opt(const nano_opt& other)              //
        noexcept(nothrow_constructible_from<T, const T&>)  //
        requires(not trivially_copyable<T>)
    {
        if (other.has_value()) {
            emplace(other.get());
        }
    }

    // Pick the type of move we want based on whether the underlying type is trivially copyable
    constexpr nano_opt(nano_opt&&) = default;
    // If no trivial move:
    constexpr nano_opt(nano_opt&& other) noexcept(nothrow_constructible_from<T, T>)
        requires(not trivially_constructible<T, T>)
    {
        if (other.has_value()) {
            emplace(NEO_MOVE(other).get());
        }
    }

    // Pick the type of copy-assignment we want based on whether the underlying type is trivially
    // copyable-assignable
    constexpr nano_opt& operator=(const nano_opt&) = default;
    // If no trivial assignment:
    constexpr nano_opt&
    operator=(const nano_opt& other) noexcept(std::is_nothrow_copy_assignable_v<T>)
        requires(not trivially_assignable<T&, const T&>)
    {
        if (has_value()) {
            if (other.has_value()) {
                get() = other.get();
            } else {
                _destroy_nocheck();
            }
        } else {
            if (other.has_value()) {
                emplace(other.get());
            } else {
                // Nothing to do.
            }
        }
        return *this;
    }

    constexpr nano_opt& operator=(nano_opt&&) = default;
    // Move
    constexpr nano_opt& operator=(nano_opt&& other)     //
        noexcept(std::is_nothrow_move_assignable_v<T>)  //
        requires(not trivially_assignable<T&, T &&>)    //
    {
        if (has_value()) {
            if (other.has_value()) {
                get() = (T&&)(other.get());
            } else {
                _destroy_nocheck();
            }
        } else {
            if (other.has_value()) {
                emplace((T&&)(other.get()));
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
    constexpr T& emplace(Args&&... args)                  //
        noexcept(nothrow_constructible_from<T, Args...>)  //
        requires constructible_from<T, Args...>
    {
        reset();
        _storage.construct(NEO_FWD(args)...);
        _active = true;
        return get();
    }

    /// Determine whether we contain a value
    constexpr bool has_value() const noexcept { return _active; }

    /// Obtain the underlying value.
    constexpr T&        get() & noexcept { return _storage.get(); }
    constexpr const T&  get() const& noexcept { return _storage.get(); }
    constexpr T&&       get() && noexcept { return NEO_MOVE(_storage).get(); }
    constexpr const T&& get() const&& noexcept { return NEO_MOVE(_storage).get(); }

    friend constexpr void do_repr(auto out, const nano_opt* self) noexcept
        requires requires { out.repr(NEO_DECLVAL(const T&)); }
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

template <typename T>
    requires neo::object_type<T> and requires { std::hash<T>{}(NEO_DECLVAL(const T&)); }
struct std::hash<neo::optional<T>> {
    constexpr std::size_t operator()(const neo::optional<T>& opt) noexcept {
        if (opt) {
            return std::hash<T>()(*opt);
        } else {
            return std::hash<std::nullptr_t>()(nullptr) ^ std::hash<int>{}(1729);
        }
    }
};

template <typename T>
struct std::hash<neo::optional<T&>> {
    constexpr std::size_t operator()(const neo::optional<T&>& opt) noexcept {
        if (opt) {
            return std::hash<T*>()(NEO_ADDRESSOF(*opt)) ^ std::hash<int>{}(89);
        } else {
            return std::hash<T*>()(nullptr) ^ std::hash<int>{}(42);
        }
    }
};
