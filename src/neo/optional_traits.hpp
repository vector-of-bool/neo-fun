#pragma once

namespace neo {

/**
 * @brief Specialize to customize the implementation of neo::optional
 *
 * @tparam T The type for which we are specializing
 *
 * A valid specialization of optional_traits defines the following:
 *
 * - `typename state_type` must be a type that is default-constructible. A default-constructed
 *   state_type object MUST represent an unalive "disengaged" state. The optional<T>
 *   will inherit its copy/move/assign/destruct attributes from the state_type object,
 *   i.e. whether it is trivial, copyable, constexpr, noexcept, etc.
 * - static has_value(const state_type&) noexcept -> bool
 *      - Must return `true` if the state is alive, otherwise `false`
 * - static get(state_type [const] &) -> T [const] &
 *      - Obtain a reference to the managed value. Should propagate cv
 *        qualifiers correctly for the underlying type. May assume that the
 *        state is alive.
 * - static destroy(state_type&) -> void
 *      - Should clear the live value in the state and reset to null. May assume
 *        that the state is alive.
 * - static construct(state_type&, ...)
 *      - Should construct a new live value from the given arguments. May assume
 *        that the state is dead.
 * - static copy(state_type& into, state_type [const] [ref] from)
 *      - Should copy/move construct from live state `from` into disengaged
 *        state `into`
 * - static assign(state_type& into, state_type [const] [ref] from)
 *      - Should copy/move assign from live state `from` into live state `into`
 *
 * - static constexpr bool trivial_move • Allow trivial moves of the state
 * - static constexpr bool trivial_move_assign • Allow trivial move-assignment of the state
 * - static constexpr bool trivial_copy • Allow trivial copy of the state
 * - static constexpr bool trivial_copy_assign • Allow trivial copy-assignment of the state
 */
template <typename T>
struct optional_traits;

}  // namespace neo
