#pragma once

#include <memory>
#include <optional>

namespace neo {

/**
 * @brief Base class to create objects that have a shared backing state.
 *
 * @tparam T The derived class (use CRTP)
 * @tparam SharedState The state that is shared between copies of the objects
 */
template <typename T, typename SharedState>
class shared_state;

/**
 * @brief A weak (non-owning) reference to the shared-state-class `T`
 */
template <typename T>
class weak_ref {
public:
    /// The type that is referred-to (May be 'const')
    using referred_type = T;

private:
    // The type of the underlying state
    using state_type = referred_type::state_type;

    // Decide whether we are pointing-to-const or not
    using pointee_type
        = std::conditional_t<std::is_const_v<referred_type>, const state_type, state_type>;

    std::weak_ptr<pointee_type> _weak;

public:
    /// Construct a new weak reference to the referred-to type
    weak_ref(referred_type& ref)
        : _weak(ref._state) {}

    /**
     * @brief Try to obtain an owning-reference to the referred-to shared object
     *
     * @return An optional<> containing an owning reference. Will be a ref_to_const if this a
     * weak-ref-to-const
     */
    [[nodiscard]] auto lock() const noexcept { return referred_type::_try_lock(_weak); }

    [[nodiscard]] long use_count() const noexcept { return _weak->use_count(); }
};

/**
 * @brief Acts as a reference-to-const for the given shared-state type T
 *
 * A `ref_to_const<T>` can bind to a `const T&`, but cannot bind to a non-const `T&`
 *
 * @tparam T The type that uses a shared state
 */
template <typename T>
class ref_to_const {
public:
    /// The type that is referred-to
    using referred_type = T;

private:
    using state_type = referred_type::state_type;

    /// The value that we only expose via const
    referred_type _value;

public:
    /// Implicitly construct from any referred-type object
    ref_to_const(const referred_type& arg) noexcept
        : _value(const_cast<referred_type&>(arg)) {}

    /// Access the underlying shared state
    const state_type* operator->() const noexcept { return _value._state.get(); }

    /// Bind a const& to the reference object
    operator const referred_type&() const noexcept { return _value; }

    /**
     * @brief Create a non-const clone of the referred-to object
     */
    [[nodiscard]] referred_type clone() const noexcept { return _value.clone(); }

    /**
     * @brief Create a new weak reference-to-const to the shared object
     */
    [[nodiscard]] class weak_ref<const referred_type> weak_ref() const noexcept {
        return _value.weak_ref();
    }

    [[nodiscard]] long
    use_count() const noexcept {
        return _value.use_count();
    }
};

template <typename T, typename SharedState>
class shared_state {
private:
    struct _emplace_ptr_tag {};

    using derived_type = T;
    using state_type   = SharedState;

    friend neo::weak_ref<derived_type>;
    friend neo::weak_ref<const derived_type>;
    friend ref_to_const<derived_type>;

    /// The actual state
    std::shared_ptr<state_type> _state;

    /// Become our derived class
    auto& _as_derived() noexcept { return static_cast<derived_type&>(*this); }
    auto& _as_derived() const noexcept { return static_cast<const derived_type&>(*this); }

    /// Construct from a given shared state
    static derived_type _from_ptr(std::shared_ptr<state_type>&& p) noexcept {
        static_assert(std::is_constructible_v<derived_type,
                                              _emplace_ptr_tag,
                                              std::shared_ptr<state_type>>,
                      "The shared_state<> derived class must explicitly inherit the constructors "
                      "of the shared_state<> base class");
        return derived_type{_emplace_ptr_tag{}, std::move(p)};
    }

    /// Lock a non-const weak reference
    static constexpr std::optional<derived_type>
    _try_lock(std::weak_ptr<state_type> const& st) noexcept {
        std::optional<derived_type> ret;
        if (auto ptr = st.lock()) {
            ret.emplace(_from_ptr(std::move(ptr)));
        }
        return ret;
    }

    /// Lock a weak reference-to-const
    static constexpr std::optional<ref_to_const<derived_type>>
    _try_lock(std::weak_ptr<const state_type> const& st) noexcept {
        std::optional<ref_to_const<derived_type>> ret;
        if (auto ptr = st.lock()) {
            ret.emplace(_from_ptr(std::const_pointer_cast<state_type>(std::move(ptr))));
        }
        return ret;
    }

public:
    /// Default-construct will allocate state. There is no "empty" condition.
    shared_state() noexcept(noexcept(state_type()))
        : _state(std::make_shared<state_type>()) {}

    /// Move in a new shared_ptr to the underlying state
    shared_state(_emplace_ptr_tag, std::shared_ptr<state_type>&& p) noexcept
        : _state(std::move(p)) {}

    /// Copy another shared state. Important: We can only copy from non-const instances!
    shared_state(shared_state& other) noexcept
        : _state(other._state) {}

    /// Move from another shared state. Important: We can only move-from non-const instances
    shared_state(shared_state&& other) noexcept
        : _state(other._state) {}

    /// Assign from another shared state. We cannot assign from a non-const instance
    derived_type& operator=(shared_state& other) & noexcept {
        _state = other._state;
        return _as_derived();
    }

    /// Move-assign from another shared staet. We cannot assign from a non-const instance
    derived_type& operator=(shared_state&& other) & noexcept {
        _state = other._state;
        return _as_derived();
    }

    /// Obtain a pointer-to the underlying shared state
    state_type*       operator->() noexcept { return _state.get(); }
    const state_type* operator->() const noexcept { return _state.get(); }

    /**
     * @brief Create a new clone of the shared object. The new instance will not shared any data
     * with this object.
     */
    [[nodiscard]] derived_type clone() const noexcept {
        return _from_ptr(std::make_shared<state_type>(*_state));
    }

    /**
     * @brief Obtain a ref_to_const for the shared object
     */
    [[nodiscard]] ref_to_const<derived_type> const_ref() const noexcept {
        return ref_to_const<derived_type>(_as_derived());
    }

    /**
     * @brief Detatch the shared state of this object from other instances.
     */
    void unshare() noexcept { _as_derived() = clone(); }

    /**
     * @brief Obtain a weak-reference to the shared object.
     */
    [[nodiscard]] neo::weak_ref<derived_type>       weak_ref() noexcept { return _as_derived(); }
    [[nodiscard]] neo::weak_ref<const derived_type> weak_ref() const noexcept {
        return _as_derived();
    }

    [[nodiscard]] long use_count() const noexcept { return _state->use_count(); }
};

}  // namespace neo
