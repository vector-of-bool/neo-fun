#pragma once

namespace neo {

/**
 * @brief Acts as a reference-to-const for the given shared-state type T
 *
 * A `ref_to_const<T>` can bind to a `const T&`, but cannot bind to a non-const `T&`
 *
 * @tparam T The type that uses a shared state
 */
template <typename T>
class ref_to_const;

/**
 * @brief A weak (non-owning) reference to the shared-state-class `T`
 */
template <typename T>
class weak_ref;

/**
 * @brief Base class to create objects that have a shared backing state.
 *
 * @tparam T The derived class (use CRTP)
 * @tparam SharedState The state that is shared between copies of the objects
 */
template <typename T, typename SharedState>
class shared_state_base;

/// Deduce to a weak ref to 'T' from a shared state
template <typename T, typename S>
weak_ref(const shared_state_base<T, S>&) -> weak_ref<T>;

/// Deduce to a weak ref-to-const-T from a ref-to-const-T
template <typename T>
weak_ref(ref_to_const<T>) -> weak_ref<const T>;

/// Deduce to a ref-to-const-T from an instance of T
template <typename T, typename S>
ref_to_const(const shared_state_base<T, S>&) -> ref_to_const<T>;

}  // namespace neo