#pragma once

#include "./fwd.hpp"
#include "./shared_fwd.hpp"

#include <memory>
#include <optional>

namespace neo {

template <typename T>
class weak_ref {
public:
    /// The type that is referred-to (May be 'const')
    using referred_type = T;

private:
    // The type of the underlying state
    using state_type = typename referred_type::state_type;

    // Decide whether we are pointing-to-const or not
    using pointee_type
        = std::conditional_t<std::is_const_v<referred_type>, const state_type, state_type>;

    std::weak_ptr<pointee_type> _weak;

public:
    /// Construct a new weak reference to the referred-to type
    weak_ref(const referred_type& ref)
        : _weak(ref._state) {}

    /**
     * @brief Try to obtain an owning-reference to the referred-to shared object
     *
     * @return An optional<> containing an owning reference. Will be a ref_to_const if this a
     * weak-ref-to-const
     */
    [[nodiscard]] auto lock() const noexcept { return referred_type::_try_lock(_weak); }

    /**
     * @brief Obtain the number of strong-references to the underlying shared state
     */
    [[nodiscard]] long use_count() const noexcept { return _weak->use_count(); }
};

template <typename T>
class ref_to_const {
public:
    /// The type that is referred-to
    using referred_type = T;

    /// The shared-state type
    using state_type = typename referred_type::state_type;

private:
    /// The value that we only expose via const
    std::shared_ptr<state_type> _state;

public:
    /// Implicitly construct from any referred-type object
    ref_to_const(const referred_type& arg) noexcept
        : _state(arg._state) {}

    /// Access the underlying shared state
    const state_type* operator->() const noexcept { return _state.get(); }

    /// Bind a reference to the underlying shared state.
    operator const state_type&() const noexcept { return *_state; }

    /**
     * @brief Create a non-const clone of the referred-to object
     */
    [[nodiscard]] referred_type clone() const noexcept {
        return referred_type::_from_ptr(_state).clone();
    }

    /**
     * @brief Detach the shared state referred to by this object.
     */
    void unshare() noexcept { _state = clone()._state; }

    /**
     * @brief Create a new weak reference-to-const to the shared object
     */
    [[nodiscard]] class weak_ref<const referred_type> weak_ref() const noexcept {
        return referred_type::_from_ptr(_state);
    }

    /// Conversion to a weak-ref
    operator neo::weak_ref<const referred_type>() const noexcept {
        return this->weak_ref();
    }

    /**
     * @brief Obtain the number of strong-references to the underlying shared state
     */
    [[nodiscard]] long use_count() const noexcept { return _state.use_count(); }
};

template <typename T, typename SharedState>
class shared_state {
public:
    /// The shared-state type
    using state_type = SharedState;

private:
    /// Tag for constructing a new instance from a shared_ptr to the state
    struct _emplace_ptr_tag {};

    using derived_type = T;

private:
    friend neo::weak_ref<derived_type>;
    friend neo::weak_ref<const derived_type>;
    friend ref_to_const<derived_type>;

    /// The actual state
    std::shared_ptr<state_type> _state;

    /// Become our derived class
    auto& _as_derived() noexcept { return static_cast<derived_type&>(*this); }
    auto& _as_derived() const noexcept { return static_cast<const derived_type&>(*this); }

    /// Construct from a given shared state
    static derived_type _from_ptr(std::shared_ptr<state_type> p) noexcept {
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

    [[deprecated("Conversion would remove 'const' qualifier on reference")]]  //
    shared_state(ref_to_const<T>)
        = delete;

    /// Obtain a pointer-to the underlying shared state
    state_type* operator->() const noexcept { return _state.get(); }

    /// Convert to a reference the underlying shared state type
    operator state_type&() const noexcept { return *_state; }

    /**
     * @brief Create a new instance of the shared state by copying from some other shared state
     */
    [[nodiscard]] static derived_type
    clone_from(const state_type& st) noexcept(noexcept(state_type(st))) {
        return _from_ptr(std::make_shared<state_type>(st));
    }

    [[nodiscard]] static derived_type
    clone_from(state_type&& st) noexcept(noexcept(state_type(std::move(st)))) {
        return _from_ptr(std::make_shared<state_type>(std::move(st)));
    }

    /**
     * @brief Create a new clone of the shared object. The new instance will not shared any data
     * with this object.
     */
    [[nodiscard]] derived_type clone() const noexcept { return clone_from(*_state); }

    /**
     * @brief Detatch the shared state of this object from other instances.
     */
    void unshare() noexcept { _as_derived() = clone(); }

    /**
     * @brief Obtain the number of strong-references to the underlying shared state
     */
    [[nodiscard]] long use_count() const noexcept { return _state->use_count(); }
};

template <typename Shared, typename State>
weak_ref(const shared_state<Shared, State>&) -> weak_ref<Shared>;

template <typename Shared, typename State>
ref_to_const(const shared_state<Shared, State>&) -> ref_to_const<Shared>;

}  // namespace neo
