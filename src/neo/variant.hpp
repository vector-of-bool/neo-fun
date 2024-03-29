#pragma once

#include "./variant.detail.hpp"

#include <neo/compare.hpp>
#include <neo/constexpr_union.hpp>
#include <neo/optional.hpp>  // optional<void>
#include <neo/unit.hpp>      // unit/nonvoid_t

#include <compare>
#include <cstddef>
#include <utility>

namespace neo {

/**
 * @brief A type-safe tagged sum type. A leaner+faster alternative to std::variant.
 *
 * In addition to regular object types, supports reference types and `void` types.
 * The assignment behavior of reference types is to perform a rebinding of the held
 * reference. A `void` alternative acts like an empty unit type (akin to std::monostate)
 */
template <typename... Ts>
class variant : _variant_detail::variant_operators<sizeof...(Ts) == 0 or true> {
    friend _variant_detail::variant_operators<true>;

    // We carefully choose between using object_t and nonvoid_t throughout. nonvoid_t
    // will map `void` to `unit`, but leave reference types as reference types. `object_t`
    // will map `void` to `unit` and changes reference types to `reference_object`, which
    // treats reference more like non-nullable pointers which can be rebound and compared without
    // touching the referred-to object.

    // In constructor contexts, `nonvoid_t` is preferred since it is lighter weight, but in
    // assignment, storage, and comparison, we use `object_t` so that we get "pointer-like"
    // semantics out of our references.

    // Extracting the first type once and reusing it is actually measurably more compile-time
    // efficient than calling `pack_at<0, ...>` multiple times.
    using _first_type = nonvoid_t<meta::pack_at<0, Ts...>>;

public:
    // Yield the Nth alternative type for the variant
    template <std::size_t N>
    using nth_type = meta::pack_at<N, Ts...>;

    /**
     * @brief Default constructor for the variant type. Default-constructs to the first alternative.
     *
     * This constructor requires that the first alternative be default-constructible.
     */
    constexpr variant() noexcept(noexcept(_first_type()))
        requires neo::default_initializable<_first_type>
    {
        _storage.template construct<0>();
        // Our _index is already default-assigned to zero
    }

    /**
     * @brief Emplacement constructor for the Nth alternative
     *
     * @param tag The in-place-index tag that selects which alternative to construct and activate
     * @param args... The arguments to pass to the constructor of the underlying alternative
     */
    template <std::size_t N, typename... Args, typename T = nth_type<N>>
    constexpr explicit variant(std::in_place_index_t<N>,
                               Args&&... args)  //
        noexcept(noexcept(nonvoid_t<T>(NEO_FWD(args)...))) {
        _storage.template construct<N>(NEO_FWD(args)...);
        this->_index = index_type(N);
    }

    /**
     * @brief In-place-by-type constructor. The requested type must occur exactly once in `Ts...`
     *
     * @param tag The in-place-type tag that selects which alternative to construct and activate
     * @param args... The arguments to pass to the constructor of the underlying alternative
     */
    template <typename T, typename... Args, std::size_t N = meta::find_type_v<T, Ts...>>
        requires((weak_same_as<T, Ts> + ...) == 1)
    NEO_ALWAYS_INLINE constexpr explicit variant(std::in_place_type_t<T>, Args&&... args)  //
        noexcept(noexcept(nonvoid_t<T>(NEO_FWD(args)...)))
        : variant(std::in_place_index<N>, NEO_FWD(args)...) {}

    /**
     * @brief Alternative conversion constructor.
     *
     * Converts from an arbitrary value to the variant alternative that most closely
     * matches it according to overload resolution. Narrowing conversions are never considered
     * (this saves us from string-literal -> bool conversions, among other things). If the
     * conversion is ambiguous, this constructor is not considered.
     *
     * This constructor is explicit if the given argument is not implicitly convertible
     * to the selected alternative.
     *
     * This constructor is never considered when the argument is an emplacement tag
     */
    template <unalike<variant> Arg,
              std::size_t      N = _variant_detail::best_conversion_index_t<Arg, Ts...>::value,
              typename T         = nth_type<N>>
    explicit(not implicit_convertible_to<Arg, nonvoid_t<T>>)  //
        constexpr variant(Arg&& arg) noexcept(noexcept(nonvoid_t<T>(NEO_FWD(arg)))) {
        _storage.template construct<N>(NEO_FWD(arg));
        this->_index = index_type(N);
    }

    /**
     * @brief Conversion assignment operator. Selects the best conversion
     * according to the same rules as our converting constructor.
     *
     * If the variant currently holds the candidate conversion alternative, then
     * the alternative will be updated in-place. Otherwise, will `emplace` a
     * new instance of the target alternative, destroying the old one.
     */
    template <unalike<variant> Arg,
              std::size_t      N = _variant_detail::best_conversion_index_t<Arg, Ts...>::value,
              typename T         = nth_type<N>>
        requires assignable_from<object_t<T>, Arg>
    constexpr variant& operator=(Arg&& other)
        // Noexcept if:
        noexcept(
            // Assignment of the chosen alt is noexcept,
            noexcept(NEO_DECLVAL(object_t<T>&) = NEO_FWD(other))
                // and construction of the chosen alt is noexcept
                and noexcept(nonvoid_t<T>(NEO_FWD(other)))) {
        if (this->index() == N) {
            // We currently hold the alternative that would be the best candidate conversion,
            // so do an assign-through to the held object.
            this->_storage.template get<N>() = NEO_FWD(other);
        } else {
            // We do not hold the requested alternative. Instead, destroy what we
            // are holding and emplace a new instance of the conversion target:
            this->emplace<N>(NEO_FWD(other));
        }
        return *this;
    }

    /**
     * @brief Emplace a new instance of the Nth alternative within the variant.
     * The existing alternative is destroyed before the new alternative is emplaced.
     *
     * A reference to the constructed value is returned.
     *
     * Unlike std::variant, this variant has no valueless_by_exception state. In
     * a rare situation where the strong exception safety guarantee cannot be met,
     * the variant will call std::terminate().
     */
    template <std::size_t N,
              typename... Args,
              typename T   = nth_type<N>,
              typename Ret = add_lvalue_reference_t<T>>
    constexpr Ret emplace(Args&&... args)
        // We throw if the constructor may throw:
        noexcept(noexcept(nonvoid_t<T>(NEO_FWD(args)...)) or not move_constructible<nonvoid_t<T>>)
        requires constructible_from<nonvoid_t<T>, Args...>
    {
        using obj_t = nonvoid_t<T>;
        if constexpr (noexcept(obj_t(NEO_FWD(args)...)) or not move_constructible<obj_t>) {
            // We are calling a no-throw constructor OR there is no available move constructor.
            // This block will never throw an exception, but may terminate()
            // Destroy the current value.
            this->_destroy_current();
            // Update the index
            this->_index = index_type(N);
            // If this constructor throws, it means that there is no valid move-constructor, then we
            // will call std::terminate()
            return static_cast<Ret>(_storage.template construct<N>(NEO_FWD(args)...));
        } else {
            // The constructor is possibly-throwing, but we have a valid move-constructor
            // to use. Do the possibly-throwing construct:
            obj_t obj(NEO_FWD(args)...);
            // We constructed without throwing. Now we are ready to swap alternatives.
            // Destroy the current alternative:
            this->_destroy_current();
            // Update our index:
            this->_index = index_type(N);
            try {
                // If the move-constructor throws, we've already cleared out our current
                // alternative and have updated our index, so there's no going back, and
                // we'll terminate(). (No valueless_by_exception() allowed)
                return static_cast<Ret>(_storage.template construct<N>(NEO_MOVE(obj)));
            } catch (...) {
                std::terminate();
            }
        }
    }

    /**
     * @brief Emplace a new instance of the given alternative within the variant.
     * The existing alternative is destroyed before the new alternative is emplaced.
     * The requested alternative must occur exactly once in `Ts...`
     *
     * A reference to the constructed value is returned.
     *
     * Unlike std::variant, this variant has no valueless_by_exception state. In
     * a rare situation where the strong exception safety guarantee cannot be met,
     * the variant will call std::terminate().
     *
     * @tparam Alt The alternative to be constructed
     */
    template <typename Alt, std::size_t N = meta::find_type_in<Alt, variant>, typename... Args>
        requires((weak_same_as<Alt, Ts> + ...) == 1) and constructible_from<nonvoid_t<Alt>, Args...>
    constexpr add_lvalue_reference_t<Alt>
    emplace(Args&&... args) noexcept(nothrow_constructible_from<nonvoid_t<Alt>, Args...>) {
        return this->template emplace<N>(NEO_FWD(args)...);
    }

    /**
     * @brief Get the zero-based index of the currently active alternative
     */
    [[nodiscard]] constexpr std::size_t index() const noexcept {
        return static_cast<std::size_t>(this->_index);
    }

    /**
     * @brief Obtain an optional reference to the Nth alternative stored by this type
     *
     * @tparam N The index of the alternative to seek.
     * @return An optional-ref-like to the Nth alternative (a pointer or optional<void>).
     * Returns a false-ish value if the requested alternative is not active.
     *
     * @note To get the alternative by type, use `neo/get.hpp` with `try_get_as`
     */
    template <std::size_t N>
        requires((N < sizeof...(Ts)))
    [[nodiscard]] constexpr auto try_get() noexcept {
        return this->template _do_try_get<N, nth_type<N>>(*this);
    }

    template <std::size_t N>
        requires((N < sizeof...(Ts)))
    [[nodiscard]] constexpr auto try_get() const noexcept {
        return this->template _do_try_get<N, const nth_type<N>>(*this);
    }

private:
    // The type used to represent the active variant member tag.
    using index_type = _variant_detail::pick_index_type<Ts...>;
    // An index sequence, for convenience
    using index_sequence = std::make_index_sequence<sizeof...(Ts)>;

    // The storage union. Being the first member ensures that `try_get` can be optimized away as
    // compile-time simple pointer conversion
    NEO_NO_UNIQUE_ADDRESS constexpr_union<object_t<Ts>...> _storage;
    // The currently active member is designated by the value of this index
    NEO_NO_UNIQUE_ADDRESS index_type _index = index_type(0);

public:
    ~variant() = default;
    // Only define a non-default destructor if any alternative has a non-trivial destructor
    NEO_CONSTEXPR_DESTRUCTOR ~variant()
        requires _variant_detail::enable_nontrivial_dtor<Ts...>
    {
        this->_destroy_current();
    }

    variant(const variant&)            = default;
    variant(variant&&)                 = default;
    variant& operator=(const variant&) = default;
    variant& operator=(variant&&)      = default;

    constexpr variant(const variant& other)
        // Noexcept if every copy-ctor is noexcept
        noexcept((noexcept(nonvoid_t<Ts>(NEO_DECLVAL(const nonvoid_t<Ts>&))) and ...))
        requires _variant_detail::enable_nontrivial_copy_construct<Ts...>
    {
        this->_copy_anew_from(*this, other, index_sequence{});
    }

    constexpr variant(variant&& other)
        // Noexcept if every move-ctor is noexcept
        noexcept((noexcept(nonvoid_t<Ts>(NEO_DECLVAL(nonvoid_t<Ts>&&))) and ...))
        requires _variant_detail::enable_nontrivial_move_construct<Ts...>
    {
        this->_copy_anew_from(*this, NEO_MOVE(other), index_sequence{});
    }

    constexpr variant& operator=(const variant& other)
        // Noexcept if:
        noexcept(
            // Every copy-assignment is noexcept:
            (noexcept(NEO_DECLVAL(object_t<Ts>&) = NEO_DECLVAL(object_t<Ts> const&)) and ...)
            // And every copy-ctor is noexcept
            and (noexcept(nonvoid_t<Ts>(NEO_DECLVAL(const nonvoid_t<Ts>&))) and ...))
        requires _variant_detail::enable_nontrivial_copy_assign<Ts...>
    {
        this->_copy_assign_from(*this, other, index_sequence{});
        return *this;
    }

    constexpr variant& operator=(variant&& other)
        // Noexcept if:
        noexcept(
            // Every move-assignment is noexcept:
            (noexcept(NEO_DECLVAL(object_t<Ts>&) = NEO_DECLVAL(object_t<Ts>&&)) and ...)
            // And every move-ctor is noexcept
            and (noexcept(nonvoid_t<Ts>(NEO_DECLVAL(nonvoid_t<Ts>&&))) and ...))
        requires _variant_detail::enable_nontrivial_move_assign<Ts...>
    {
        this->_copy_assign_from(*this, NEO_MOVE(other), index_sequence{});
        return *this;
    }

    /**
     * @brief Destroy the currrently held variant alternative.
     */
    constexpr void _destroy_current() noexcept {
        if constexpr (_variant_detail::enable_nontrivial_dtor<Ts...>) {
            this->_do_destroy_current(*this, index_sequence{});
        }
    }
};

namespace _variant_detail {

template <bool>
struct variant_operators {
    /**
     * @brief Define an ADL-visible swap() function between variants.
     *
     * Requires that all alternatives are swappable. If the two variants hold
     * the same alternative, then alternatives are swapped in-place. Otherwise,
     * the standard exchange-swap occurs between the three variants.
     */
    template <typename... Ts>
    constexpr friend void swap(variant<Ts...>& left, variant<Ts...>& right) noexcept(
        // All sub-swaps are noexcept?
        (noexcept(neo::swap(NEO_DECLVAL(object_t<Ts>&), NEO_DECLVAL(object_t<Ts>&))) and ...)
        // all objects are nothrow-move-constructible?
        and (nothrow_constructible_from<nonvoid_t<Ts>, nonvoid_t<Ts>&&> and ...))
        // Require all objects to be swappable and move-constructible
        requires(swappable<object_t<Ts>> and ...) and (move_constructible<nonvoid_t<Ts>> and ...)
    {
        _impl_swap(left, right, std::index_sequence_for<Ts...>{});
    }

    template <typename... Ts>
        requires(equality_comparable<object_t<Ts>> and ...)
    friend constexpr bool operator==(const variant<Ts...>&             self,
                                     nondeduced<variant<Ts...> const&> other) noexcept {
        return _compare_eq(self, other, std::index_sequence_for<Ts...>{});
    }

    template <typename... Ts>
        requires(totally_ordered<object_t<Ts>> and ...)
    friend constexpr auto operator<=>(const variant<Ts...>&             self,
                                      const nondeduced<variant<Ts...>>& other) noexcept {
        return _compare_order(self, other, std::index_sequence_for<Ts...>{});
    }

    // Implements order comparisong
    template <std::size_t... Ns, typename... Ts>
    static constexpr auto _compare_order(const variant<Ts...>& self,
                                         const variant<Ts...>& other,
                                         std::index_sequence<Ns...>) noexcept {
        auto this_index  = self.index();
        auto other_index = other.index();
        // If the indices are not the same, immedately record them as non-equal.
        std::common_comparison_category_t<std::compare_three_way_result_t<object_t<Ts>>...> ret
            = this_index <=> other_index;
        // Compare until we get to the active alternative:
        static_cast<void>((
            (ret != 0  // Don't proceed if we've already labeled ourself as unequal
             or ((this_index == Ns)
                 and 0
                     != (ret = (std::compare_strong_order_fallback(self._storage.template get<Ns>(),
                                                                   other._storage
                                                                       .template get<Ns>())))))
            or ...));
        // Return the result:
        return ret;
    }

    // Implements equality comparison
    template <std::size_t... Ns, typename... Ts>
    static constexpr bool _compare_eq(const variant<Ts...>& self,
                                      const variant<Ts...>& other,
                                      std::index_sequence<Ns...>) noexcept {
        auto this_index  = self.index();
        auto other_index = other.index();
        if (this_index != other_index) {
            // Not the same alternative, so not equal
            return false;
        }
        // Fold over the alternatives, and compare the two active ones:
        const bool eq = static_cast<bool>(
            (((this_index == Ns)
              and self._storage.template get<Ns>() == other._storage.template get<Ns>())
             or ...));
        return eq;
    }

    template <typename... Ts, std::size_t... Ns>
    static constexpr void
    _impl_swap(variant<Ts...>& left, variant<Ts...>& right, std::index_sequence<Ns...>) {
        // Our active index
        auto left_index = left.index();
        // The other's active index
        auto right_index = right.index();
        if (left_index != right_index) {
            // We do not share the same alternative. Do an exchange-swap
            variant tmp = NEO_MOVE(left);
            left        = NEO_MOVE(right);
            right       = NEO_MOVE(tmp);
        } else {
            // We share an alternative. Call the best available swapper for that alt:
            static_cast<void>(((left_index == Ns
                                and (neo::swap(left._storage.template get<Ns>(),
                                               right._storage.template get<Ns>()),
                                     true))
                               or ...));
        }
    }

    /**
     * @brief Decay-copy construct a new value from another variant instance. It is assumed
     * that no alternative is currently active.
     *
     * @param self This variant
     * @param other The variant from which to copy/move.
     */
    template <typename Other, std::size_t... Ns>
    static constexpr void _copy_anew_from(auto& self, Other&& other, std::index_sequence<Ns...>) {
        const std::size_t other_index = other.index();
        auto&             my_store    = self._storage;
        auto&&            their_store = NEO_FWD(other)._storage;
        static_cast<void>(((other_index == Ns
                            and (static_cast<void>(my_store.template construct<Ns>(
                                     NEO_FWD(their_store).template get<Ns>())),
                                 true))
                           or ...));
        self._index = other._index;
    }

    /**
     * @brief Decay-copy assign a new value from another variant instance. The currently
     * held alternative is destroyed.
     *
     * @param other The variant from which to copy/move.
     */
    template <typename Other, std::size_t... Ns>
    static constexpr void _copy_assign_from(auto& self, Other&& other, std::index_sequence<Ns...>) {
        const std::size_t other_index = other.index();
        const std::size_t my_index    = self.index();
        if (my_index == other_index) {
            // We both hold the same alternative. Copy-assign over the existing value
            static_cast<void>(
                ((other_index == Ns
                  and (static_cast<void>(self._storage.template get<Ns>()
                                         = NEO_FWD(other)._storage.template get<Ns>()),
                       true))
                 or ...));
        } else {
            // We contain different types. Use emplace(), which will destroy the current before
            // copying the other
            static_cast<void>((((other_index == Ns)
                                and (static_cast<void>(self.template emplace<Ns>(
                                         NEO_FWD(other)._storage.template get<Ns>())),
                                     true))
                               or ...));
        }
    }

    template <std::size_t... Ns>
    static constexpr void _do_destroy_current(auto& self, std::index_sequence<Ns...>) noexcept {
        const auto current_index = self.index();
        static_cast<void>((((current_index == Ns)
                            and (static_cast<void>(self._storage.template destroy<Ns>()), true))
                           or ...));
    }

    template <std::size_t N,
              typename T,
              typename Self,
              typename Ref = add_lvalue_reference_t<T>,
              typename OptionalRef
              = conditional_t<void_type<T>, optional<void>, add_pointer_t<Ref>>>
    constexpr static OptionalRef _do_try_get(Self& self) noexcept {
        const std::size_t my_index = self.index();
        if (N == my_index) {
            if constexpr (void_type<T>) {
                return OptionalRef{std::in_place};
            } else {
                return NEO_ADDRESSOF(static_cast<Ref>(self._storage.template get<N>()));
            }
        } else {
            return OptionalRef{};
        }
    }

    /**
     * @brief Conversion-skipping equality operator.
     *
     * If there is a valid conversion target from the given operand, and the
     * selected target is equality-comparable with the operand, then this
     * operator defines the comparison without attempting to construct a new
     * temporary `variant` from the altnerative.
     *
     * This operator== allows equality-comparing variants with values that are equality-comparable,
     * even if the equality operator is not fully defined for other alternatives.
     */
    template <typename... Ts,
              typename Other,
              std::size_t N = _variant_detail::best_conversion_index_t<Other const&, Ts...>::value,
              typename T    = meta::pack_at<N, Ts...>>
        requires equality_comparable_with<Other const&, object_t<T>>
    constexpr friend bool operator==(const variant<Ts...>& var, const Other& other) noexcept {
        // Call out to a static member function. Older compilers do not consider friend functions
        // to be within the scope of the enclosing class, thus the `friend variant_operators`
        // declaration in variant<...> won't be respected here.
        return _compare_eq<N, T>(var, other);
    }

    template <std::size_t N, typename T, typename Other, typename... Ts>
    static constexpr bool _compare_eq(const variant<Ts...>& var, const Other& other) noexcept {
        if (var.index() != N) {
            // We do not hold the alternative under question.
            return false;
        }
        // We are good to compare. DO NOT compare using try_get(): We want to
        // compare unit as unit and reference_object as reference_object!
        const object_t<T>& held = var._storage.template get<N>();
        return held == other;
    }

    /**
     * @brief Conversion-skipping comparison operator.
     *
     * If there is a valid conversion target from the given operand, and the
     * selected target is three-way-comparable with the operand, then this
     * operator defines the comparison without attempting to construct a new
     * temporary `variant` from the altnerative.
     *
     * This operator<=> allows three-way-comparing variants with values that are comparable,
     * even if an ordering is not fully defined for other alternatives.
     */
    template <typename... Ts,
              typename Other,
              std::size_t N = _variant_detail::best_conversion_index_t<Other const&, Ts...>::value,
              typename T    = meta::pack_at<N, Ts...>>
        requires totally_ordered_with<object_t<T>, Other const&>
    constexpr friend synth_three_way_result_t<object_t<T>, Other const&>
    operator<=>(const variant<Ts...>& var, const Other& other) noexcept {
        // See above operator== for why this is a separate function
        return _compare_3way<N, T>(var, other);
    }

    template <std::size_t N, typename T, typename Other, typename... Ts>
    constexpr static synth_three_way_result_t<object_t<T>, Other>
    _compare_3way(const variant<Ts...>& var, const Other& other) noexcept {
        if (var.index() != N) {
            // Not the same alternative, so order based on the expected alternative index:
            return var.index() <=> N;
        }
        // We are good to compare. DO NOT compare using try_get(): We want to
        // compare unit as unit and reference_object as reference_object!
        const object_t<T>& held = var._storage.template get<N>();
        return synth_three_way(held, other);
    }
};

}  // namespace _variant_detail

}  // namespace neo
