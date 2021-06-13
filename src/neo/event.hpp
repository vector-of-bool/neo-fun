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
class scoped_listener;

template <typename T>
using scoped_subscriber [[deprecated("Use scoped_listener<T>")]] = scoped_listener<T>;

namespace event_detail {

/// The top-most subscriber for events of type T, or null if no one is subscribed
template <typename T>
thread_local opt_ref<scoped_listener<T>> tl_tail_listener;

/// The currently executing event handler for the given type 'T'
template <typename T>
thread_local opt_ref<scoped_listener<T>> tl_currently_running_handler;

// helper for emit_as_t
template <typename T>
struct emits_as {
    using type = T;
};

template <typename T>
requires requires {
    typename T::emit_as;
}
struct emits_as<T> {
    using type = T::emit_as;
};

// helper for emit_result_t
template <typename>
struct emit_result {
    using type = void;
};

template <typename T>
requires requires {
    typename T::emit_result;
}
struct emit_result<T> {
    using type = T::emit_result;
};

/// Agent class to invoke and bubble events, as the invoke() and bubble_event() methods are private
class subscr_agent {
public:
    template <typename S, typename Ev>
    static decltype(auto) invoke(S&& s, Ev&& e) {
        return s.invoke(e);
    }

    template <typename S, typename Ev>
    static decltype(auto) bubble_event(S&& s, Ev&& e) {
        return s.bubble_event(e);
    }
};

}  // namespace event_detail

/**
 * @brief Given an event type, get the return type it expects from handlers and from emit()
 *
 * An event type may define a nested type name 'emit_result', which is the type that it
 * expects to see returned from any event handlers for that event. If the type is absent, the
 * default is 'void'.
 */
template <typename T>
using emit_result_t = event_detail::emit_result<T>::type;

/**
 * @brief Given an event type, find the type that is used for dispatch.
 *
 * An emitted event type may define a nested type name 'emit_as', which will be
 * used to key the event dispatch mechanism, rather than its own type. The
 * event type should be able to bind to a const& of its emit_as type.
 */
template <typename T>
using emits_as_t = event_detail::emits_as<T>::type;

/**
 * @brief Obtain a reference to the current listener for the given event type, or 'null' if no one
 * is listening
 *
 * @tparam T The type of the event that is being listened for
 */
template <typename T>
opt_ref<scoped_listener<T>> get_event_subscriber() noexcept {
    return event_detail::tl_tail_listener<T>;
}

/// impl
template <typename T>
class scoped_listener {
    // Private access to the subscription agent to invoke our private methods
    friend event_detail::subscr_agent;

public:
    // The return type that we expect from our handler
    using emit_result = emit_result_t<T>;

private:
    /**
     * @brief Actually execute the event handler
     */
    emit_result invoke(const T& v) {
        // Store the prior executing handler
        auto prev_handler = event_detail::tl_currently_running_handler<T>;
        // Announce that we are the current handler
        event_detail::tl_currently_running_handler<T> = *this;
        // When scope leaves, restore the prior executing handler
        neo_defer { event_detail::tl_currently_running_handler<T> = prev_handler; };
        // Go!
        neo_assertion_breadcrumbs("Executing event handler", *this);
        return do_invoke(v);
    }

    /**
     * @brief Pass the event event to the parent handler in the stack, if it exists
     */
    emit_result bubble_event(const T& v) {
        if (_prev_ref) {
            return _prev_ref->invoke(v);
        } else {
            if constexpr (!std::is_void_v<emit_result>) {
                return v.default_emit_result();
            }
        }
    }

    // Keep a reference to the prior handler in the thread-local stack
    opt_ref<scoped_listener<T>> _prev_ref = std::exchange(event_detail::tl_tail_listener<T>, *this);

public:
    scoped_listener() = default;

    virtual ~scoped_listener() {
        neo_assert(expects,
                   event_detail::tl_tail_listener<T>.pointer() == this,
                   "neo::subscribe() objects are being destructed out-of-order, which is illegal.",
                   this,
                   event_detail::tl_tail_listener<T>.pointer());
        // Restore the prior event handler
        event_detail::tl_tail_listener<T> = _prev_ref;
    }

    // We are immobile
    scoped_listener(const scoped_listener&) = delete;

    friend constexpr void do_repr(auto out, scoped_listener const* self) {
        if constexpr (decltype(out)::template can_repr<T>) {
            out.type("neo::scoped_listener", out.template repr_type<T>());
        } else {
            out.type("neo::scoped_listener<...>");
        }
        if (self) {
            bool is_active = self == event_detail::tl_currently_running_handler<T>.pointer();
            bool is_tail   = self == event_detail::tl_tail_listener<T>.pointer();
            out.bracket_value("active={}, tail={}", is_active, is_tail);
        }
    }

private:
    // Private virt interface implemented by concrete instances
    virtual emit_result do_invoke(const T& value) const = 0;
};

namespace event_detail {

/// Emit a single instance of the given event
template <typename Event>
decltype(auto) emit_one(const Event& ev) {
    using emit_type = emits_as_t<Event>;
    auto& handler   = event_detail::tl_tail_listener<emit_type>;
    static_assert(
        std::convertible_to<const Event&, const emit_type&>,
        "The event type defines an emits_as alias, but the event object cannot bind to a reference "
        "to that type. Check that the emit_as alias is an accessible base class of the event type "
        "or that the appropriate conversion operator is defined.");
    const emit_type& downcast_event = ev;
    if (handler) {
        return subscr_agent::invoke(*handler, downcast_event);
    } else {
        if constexpr (!std::is_void_v<emit_result_t<emit_type>>) {
            return downcast_event.default_emit_result();
        }
    }
}

/// Emit a single event, but lazily call a factory function that will produce the event object
template <typename EventReturner>
void emit_one(const EventReturner& func) requires(
    !std::is_void_v<neo::invoke_result_t<EventReturner>>) {
    // The actual event type:
    using EventType = emits_as_t<neo::invoke_result_t<EventReturner>>;
    // If we have a handler, invoke the factory and emit the event
    if (!!event_detail::tl_tail_listener<std::remove_cvref_t<EventType>>) {
        emit_one(neo::invoke(func));
    }
}

/// Check that the given handler function is valid to be used as a handler type
template <typename Handler>
concept listen_handler_check = fixed_invocable<Handler>&& invocable_arity_v<Handler> == 1;

}  // namespace event_detail

/**
 * @brief Deduce the event type which that should be listened for with the given handler
 */
template <event_detail::listen_handler_check Handler>
using handler_listen_type_t = std::remove_cvref_t<sole_arg_type_t<Handler>>;

/**
 * @brief Class template that holds a subscription to an event.
 *
 * Should be instantiated as a local object via CTAD
 */
template <typename Handler, typename ListenEvent = handler_listen_type_t<Handler>>
requires invocable<Handler, const ListenEvent&>  //
    class listener : scoped_listener<ListenEvent> {

    [[no_unique_address]] Handler _handler;

    using typename listener::scoped_listener::emit_result;

    static emit_result listener_default_return() requires requires {
        ListenEvent::default_handle_result();
    }
    { return ListenEvent::default_handle_result(); }

    emit_result do_invoke(const ListenEvent& event) const override {
        using GivenHandlerRetType = neo::invoke_result_t<Handler&, const ListenEvent&>;
        if constexpr (std::convertible_to<GivenHandlerRetType, emit_result>) {
            if constexpr (std::is_void_v<emit_result>) {
                static_assert(std::is_void_v<GivenHandlerRetType>,
                              "The handler for this event type should not return a value");
            }
            return neo::invoke(_handler, event);
        } else if constexpr (std::is_void_v<GivenHandlerRetType>) {
            neo::invoke(_handler, event);
            return event.default_emit_result();
        } else {
            static_assert(std::is_void_v<GivenHandlerRetType>,
                          "The event handler's return type cannot convert to the type that is "
                          "expected by the event's emitter");
        }
    }

public:
    listener() = default;
    listener(Handler&& h)
        : _handler(NEO_FWD(h)) {}

    constexpr friend void do_repr(auto out, listener const* self) noexcept {
        do_repr(out, static_cast<listener::scoped_listener const*>(self));
    }
};

template <event_detail::listen_handler_check Handler>
listener(Handler&& fn) -> listener<Handler, handler_listen_type_t<Handler>>;

template <typename Handler>
using subscription [[deprecated("Use listener<Handler>")]]
= listener<Handler, handler_listen_type_t<Handler>>;

/**
 * @brief Declare an event subscription in the current scope
 */
#define NEO_SUBSCRIBE ::neo::listener NEO_CONCAT(_local_neo_subscr_, __COUNTER__) = [&]

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
template <event_detail::listen_handler_check... Funcs>
[[nodiscard]] auto listen(Funcs&&... fns) noexcept {
    if constexpr (sizeof...(fns) == 1) {
        return listener(NEO_FWD(fns)...);
    } else {
        return std::tuple<listener<Funcs, handler_listen_type_t<Funcs>>...>(NEO_FWD(fns)...);
    }
}

template <typename EventType, typename Handler>
[[nodiscard]] auto listen(Handler&& fn) noexcept {
    static_assert(
        neo::invocable<Handler, const EventType&>,
        "Handler function must be invocable with a single argument of a const& of the event type");
    return listener<Handler, EventType>(NEO_FWD(fn));
}

template <event_detail::listen_handler_check... Funcs>
[[nodiscard, deprecated("Use neo::listen()")]] auto subscribe(Funcs&&... fns) noexcept {
    return listen(NEO_FWD(fns)...);
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
decltype(auto) emit(const Events&... ev) {
    if constexpr (sizeof...(ev) == 1) {
        return event_detail::emit_one(ev...);
    } else {
        (event_detail::emit_one(ev), ...);
    }
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
decltype(auto) bubble_event(const Event& ev) {
    auto cur_handler = event_detail::tl_currently_running_handler<Event>;
    neo_assert(expects,
               !!cur_handler,
               "bubble_event() must only be called during the execution of an event handler of the "
               "same type");
    return event_detail::subscr_agent::bubble_event(*cur_handler, ev);
}

/**
 * @brief Emit an event whose type is of the given expression.
 *
 * If there is no subscriber for the event in question, the expression will not be evaluated.
 */
#define NEO_EMIT(...) ::neo::emit([&] { return (__VA_ARGS__); })

/**
 * @brief Like a neo::listener, but does not register the listener immediately. Call
 * `.subscribe()` to activate it.
 *
 * Should be instantiated via CTAD.
 */
template <typename Handler, typename ListenEvent = handler_listen_type_t<Handler>>
class opt_listener {
    Handler _handler;

    nano_opt<listener<Handler&, ListenEvent>> _opt;

    friend constexpr void do_repr(auto out, opt_listener const* self) {
        using event_type = sole_arg_type_t<Handler>;
        if constexpr (decltype(out)::template can_repr<Handler>) {
            out.type("neo::opt_listener<{}>", out.template repr_type<Handler>());
        } else {
            out.type("neo::opt_listener<[...]>");
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
    opt_listener() = default;

    opt_listener(Handler&& h)
        : _handler(h) {}

    void start_listening() noexcept {
        neo_assert(expects,
                   !is_listening(),
                   "Called start_listening() on active opt_listener",
                   *this);
        _opt.emplace(_handler);
    }
    void stop_listening() noexcept {
        neo_assert(expects,
                   is_listening(),
                   "Called stop_listening() on inactive opt_listener",
                   *this);
        _opt.reset();
    }

    [[deprecated("Use start_listening()")]] void subscribe() noexcept { start_listening(); }
    [[deprecated("Use stop_listening()")]] void  unsubscribe() noexcept { stop_listening(); }
    /// Check whether this listener is active
    [[nodiscard]] bool is_listening() const noexcept { return _opt.has_value(); }

    [[nodiscard, deprecated("Use is_listening()")]] bool is_subscribed() const noexcept {
        return is_listening();
    }
};

template <typename Handler>
opt_listener(Handler &&) -> opt_listener<Handler, handler_listen_type_t<Handler>>;

template <typename H>
using opt_subscription [[deprecated("Use opt_listener<T>")]]
= opt_listener<H, handler_listen_type_t<H>>;

/**
 * @brief Determine whether there is anyone listening for the given event type
 */
template <typename Event>
[[nodiscard]] bool has_listener() noexcept {
    return !!event_detail::tl_tail_listener<Event>;
}

}  // namespace neo
