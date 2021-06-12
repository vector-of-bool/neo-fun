#pragma once

#include "./assert.hpp"
#include "./function_traits.hpp"
#include "./fwd.hpp"
#include "./invoke.hpp"
#include "./opt_ref.hpp"
#include "./optional.hpp"
#include "./scope.hpp"
#include "./tag.hpp"

#include <tuple>

namespace neo {

/**
 * @brief Abstract class type that refers to an event subscription for event type T
 *
 * @tparam T The event type of the subscription
 */
template <typename T>
class scoped_subscription;

namespace event_detail {

/// The top-most subscriber for events of type T, or `nullptr` if no one is subscribed
template <typename T>
thread_local opt_ref<scoped_subscription<T>> tl_subscr;

template <typename T>
thread_local opt_ref<scoped_subscription<T>> tl_cur_handler;

class subscr_agent {
public:
    template <typename S, typename Ev>
    static void invoke(S&& s, Ev&& e) {
        s.invoke(e);
    }

    template <typename S, typename Ev>
    static void bubble_event(S&& s, Ev&& e) {
        s.bubble_event(e);
    }
};

}  // namespace event_detail

template <typename T>
opt_ref<scoped_subscription<T>> get_event_subscriber() noexcept {
    return event_detail::tl_subscr<T>;
}

/// impl
template <typename T>
class scoped_subscription {
    // Private access to the subscription agent to invoke our private methods
    friend class event_detail::subscr_agent;

private:
    /**
     * @brief Actually execute the event handler
     */
    void invoke(const T& v) {
        // Store the prior executing handler
        auto prev_handler = event_detail::tl_cur_handler<T>;
        // Announce that we are the current handler
        event_detail::tl_cur_handler<T> = *this;
        // When scope leaves, restore the prior executing handler
        neo_defer { event_detail::tl_cur_handler<T> = prev_handler; };
        // Go!
        neo_assertion_breadcrumbs("Handling event");
        do_invoke(v);
    }

    /**
     * @brief Pass the event event to the parent handler in the stack, if it exists
     */
    void bubble_event(const T& v) {
        if (_prev_ref) {
            _prev_ref->invoke(v);
        }
    }

protected:
    // Keep a reference to the prior handler in the thread-local stack
    opt_ref<scoped_subscription<T>> _prev_ref = std::exchange(event_detail::tl_subscr<T>, *this);

    scoped_subscription() = default;

    virtual ~scoped_subscription() {
        neo_assert(expects,
                   event_detail::tl_subscr<T>.pointer() == this,
                   "neo::subscribe() objects are being destructed out-of-order, which is illegal.",
                   this,
                   event_detail::tl_subscr<T>.pointer());
        // Restore the prior event handler
        event_detail::tl_subscr<T> = _prev_ref;
    }

    // We are immobile
    scoped_subscription(const scoped_subscription&) = delete;

    friend constexpr void do_repr(auto out, scoped_subscription const* self) {
        if constexpr (decltype(out)::template can_repr<T>) {
            out.type("neo::scoped_subscription", out.template repr_type<T>());
        } else {
            out.type("neo::scoped_subscription<...>");
        }
        if (self) {
            bool is_active = self == event_detail::tl_cur_handler<T>.pointer();
            bool is_tail   = self == event_detail::tl_subscr<T>.pointer();
            out.bracket_value("active={}, tail={}", is_active, is_tail);
        }
    }

private:
    virtual void do_invoke(const T& value) const = 0;
};

namespace event_detail {

/// Emit a single instance of the given event
template <typename Event>
void emit_one(const Event& ev) {
    auto& handler = event_detail::tl_subscr<std::remove_cvref_t<Event>>;
    if (handler) {
        subscr_agent::invoke(*handler, ev);
    }
}

/// Emit a single event, but lazily call a factory function that will produce the event object
template <typename EventReturner>
void emit_one(const EventReturner& func) requires(
    !std::is_void_v<neo::invoke_result_t<EventReturner>>) {
    // The actual event type:
    using RetType = neo::invoke_result_t<EventReturner>;
    // If we have a handler, invoke the factory and emit the event
    if (!!event_detail::tl_subscr<std::remove_cvref_t<RetType>>) {
        emit_one(neo::invoke(func));
    }
}

// Forward-decl concrete implementation of erased subscriber type
template <typename T, typename Func>
class scoped_subscription_impl;

// Calculate the type of an event handler
template <typename Func>
using subscription_func_result_t
    = scoped_subscription_impl<std::remove_cvref_t<sole_arg_type_t<Func>>, Func>;

template <typename Func>
concept subscription_func_check = fixed_invocable<Func>&& invocable_arity_v<Func> == 1;

/// Impl class for event subscriptions
template <typename T, typename Func>
class scoped_subscription_impl : public scoped_subscription<T> {
    // Our handler:
    Func _fn;

    using arg_type                 = sole_arg_type_t<Func>;
    constexpr static bool is_rref  = std::is_rvalue_reference_v<arg_type>;
    constexpr static bool is_ref   = std::is_reference_v<arg_type>;
    constexpr static bool is_const = std::is_const_v<std::remove_reference_t<arg_type>>;
    constexpr static bool is_okay  = not is_rref and (not is_ref || (is_ref && is_const));
    static_assert(
        is_okay,
        "Subscription function should take a single parameter of value type or of 'const&' type");

    // Pass the event down to our handler function:
    void do_invoke(const T& value) const override {
        if constexpr (is_okay) {
            neo::invoke(_fn, value);
        }
    }

public:
    scoped_subscription_impl() = default;
    // Simple construct:
    explicit scoped_subscription_impl(Func&& fn)
        : _fn(NEO_FWD(fn)) {}
};

}  // namespace event_detail

/**
 * @brief Class template that holds a subscription to an event.
 *
 * Should be instantiated as a local object via CTAD
 */
template <event_detail::subscription_func_check Func>
class subscription : event_detail::subscription_func_result_t<Func> {
public:
    subscription() = default;
    subscription(Func&& h)
        : subscription::scoped_subscription_impl(NEO_FWD(h)) {}

    constexpr friend void do_repr(auto out, subscription const* self) noexcept {
        do_repr(out, static_cast<subscription::scoped_subscription const*>(self));
    }
};

template <event_detail::subscription_func_check Func>
subscription(Func&& fn) -> subscription<Func>;

/**
 * @brief Declare an event subscription in the current scope
 */
#define NEO_SUBSCRIBE ::neo::subscription NEO_CONCAT(_local_neo_subscr_, __COUNTER__) = [&]

/**
 * @brief Subscribe to one or more thread-local events.
 *
 * Each argument must be a non-overloaded invocable object that accepts a single
 * argument of type 'E'. This will generate a thread-local event subscription to
 * events emitted of type 'E'.
 *
 * If 'E' already has a subscription in the current thread, this function will
 * replace that subscription for the scope of the return object. Once the return
 * object goes out-of-scope, the prior event handler for 'E' will be restored.
 *
 * @param fns Some number of unary-invocable objects
 * @return [unspecified] An object that keeps the subscriptions alive for its lifetime.
 */
template <event_detail::subscription_func_check... Funcs>
[[nodiscard]] auto subscribe(Funcs&&... fns) noexcept {
    return std::tuple<subscription<Funcs>...>(NEO_FWD(fns)...);
}

/**
 * @brief Emit one or more events of the given types.
 *
 * This will call the current thread-local registered event handler for the
 * type of each event. If no handler is registered, this function is a no-op.
 *
 * For each argument 'e' of type 'E':
 *
 * - if 'e' is invocable with zero arguments and returns non-void, it will be
 *   treated as an "event factory" function. If there is no event handler
 *   installed for 'invoke_result_t<E>', then 'e' will be ignored. Otherwise,
 *   it will be as-if 'emit(invoke(e))'. This permits events to be lazily
 *   constructed only if there is someone actively listening for those events.
 * - Otherwise, 'e' will be emitted directly to the subscribing handler for 'E',
 *   if one is listening.
 *
 * @param ev The events or event-factories
 */
template <typename... Events>
void emit(const Events&... ev) {
    (event_detail::emit_one(ev), ...);
}

/**
 * @brief Re-emit the given event to the parent handler in the event-handler
 * chain.
 *
 * There is a thread-local stack of event handlers listening for any type 'E'.
 * When an 'E' is emitted, only the most-recent handler will be invoked. If a
 * handler 'H' listening for 'E' calls 'bubble_event' with an 'E', then the
 * next handler in the chain before 'H' will receive the event.
 *
 * @tparam Event The event to emit.
 * @param ev
 */
template <typename Event>
void bubble_event(const Event& ev) {
    auto cur_handler = event_detail::tl_cur_handler<Event>;
    neo_assert(expects,
               !!cur_handler,
               "bubble_event() must only be called during the execution of an event handler of the "
               "same type");
    event_detail::subscr_agent::bubble_event(*cur_handler, ev);
}

/**
 * @brief Emit an event whose type is of the given expression.
 *
 * If there is no subscriber for the event in question, the expression will not be evaluated.
 */
#define NEO_EMIT(...) ::neo::emit([&] { return (__VA_ARGS__); })

/**
 * @brief Like a neo::subscription, but does not register the subscription immediately. Call
 * `.subscribe()` to activate it.
 *
 * Should be instantiated via CTAD.
 */
template <typename Handler>
class opt_subscription {
    Handler _handler;

    nano_opt<subscription<Handler&>> _opt;

    friend constexpr void do_repr(auto out, opt_subscription const* self) {
        using event_type = sole_arg_type_t<Handler>;
        if constexpr (decltype(out)::template can_repr<Handler>) {
            out.type("neo::opt_subscription<{}>", out.template repr_type<Handler>());
        } else {
            out.type("neo::opt_subscription<[...]>");
        }
        if constexpr (decltype(out)::template can_repr<event_type>) {
            out.type("[Event type '{}']", out.template repr_type<event_type>());
        }
        if (self) {
            bool is_enabled = self->_opt.has_value();
            if (!is_enabled) {
                out.value("[disabled]");
            } else {
                out.value("{}", out.repr_value(self->_opt.get()));
            }
        }
    }

public:
    opt_subscription() = default;

    opt_subscription(Handler&& h)
        : _handler(h) {}

    void subscribe() noexcept {
        neo_assert(expects,
                   !is_subscribed(),
                   "Called subscribe() on active opt_subscription",
                   *this);
        _opt.emplace(_handler);
    }
    void unsubscribe() noexcept {
        neo_assert(expects,
                   is_subscribed(),
                   "Called unsubscribe() on inactive opt_subscription",
                   *this);
        _opt.reset();
    }
    /// Check whether this subscription is active
    [[nodiscard]] bool is_subscribed() const noexcept { return _opt.has_value(); }
};

template <typename Handler>
opt_subscription(Handler &&) -> opt_subscription<Handler>;

/**
 * @brief Determine whether there is anyone listening for the given event type
 */
template <typename Event>
[[nodiscard]] bool has_subscriber() noexcept {
    return !!event_detail::tl_subscr<Event>;
}

}  // namespace neo
