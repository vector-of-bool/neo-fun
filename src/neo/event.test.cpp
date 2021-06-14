#include <neo/event.hpp>
#include <neo/repr.hpp>

#include <catch2/catch.hpp>

using neo::repr_ostream_operator::operator<<;

struct my_event {
    int value;
};

TEST_CASE("Basic event handling") {
    int emitted_value = 0;

    {
        auto _ = neo::listen([&](const int& v) { emitted_value = v; });
        neo::emit(12);
        CHECK(emitted_value == 12);
        neo::emit(3);
        CHECK(emitted_value == 3);
        neo::emit("hi");
    }

    // The last emit won't do anything, because the subscription is destroyed
    neo::emit(45);
    CHECK(emitted_value == 3);

    {
        neo::emit(my_event{12});
        auto sub = neo::listen([&](const my_event ev) { emitted_value = ev.value; });
        CHECK(emitted_value == 3);
        neo::emit(my_event{42});
        CHECK(emitted_value == 42);
        neo::emit(44);  // Different type, differen subscribers
        CHECK(emitted_value == 42);

        CHECK(neo::repr(sub).string() == "neo::scoped_listener<...>{active=false, tail=true}");
        CHECK(neo::repr_value(sub).string() == "{active=false, tail=true}");

        {
            auto sub2 = neo::listen([](const my_event&) {});  // Do nothing
            neo::emit(my_event{7});
            CHECK(emitted_value == 42);
        }

        // Restore the previous handler
        neo::emit(my_event{9191});
        CHECK(emitted_value == 9191);

        neo::emit([] { return std::string("I am a string"); });
        // Emit with a value-generating function:
        neo::emit([] { return my_event{33}; });
        CHECK(emitted_value == 33);

        {
            neo::listener filter_sub = [](my_event e) {
                // Only bubble-up the event if the value is odd
                if (e.value % 2 == 1) {
                    neo::bubble_event(e);
                }
            };
            NEO_LISTEN(my_event ev) { neo::bubble_event(ev); };
            neo::emit(my_event{312});
            CHECK(emitted_value == 33);  // Didn't change
            NEO_EMIT(my_event{311});
            CHECK(emitted_value == 311);  // Changed!
        }
    }

    {
        NEO_LISTEN(my_event ev) { emitted_value = ev.value; };
        neo::emit(my_event{33});
        CHECK(emitted_value == 33);
        {
            neo::event_blocker<my_event> block;
            neo::emit(my_event{61});
            CHECK(emitted_value == 33);  // Blocked!
        }
    }
}

static int S_emitted_value = 0;

TEST_CASE("Optional handler") {
    int emitted_value = 0;

    neo::opt_listener sub = [&](my_event ev) { emitted_value = ev.value; };
    NEO_EMIT(my_event{23});
    CHECK(emitted_value == 0);  // Not active yet
    sub.start_listening();
    NEO_EMIT(my_event{42});
    CHECK(emitted_value == 42);  // Subscribed this time
    sub.stop_listening();
    NEO_EMIT(my_event{41});
    CHECK(emitted_value == 42);  // Did not change

    neo::opt_listener<decltype([](my_event ev) { S_emitted_value = ev.value; })> sub2;
    CAPTURE(sub2);
    NEO_EMIT(my_event{5});
    CHECK(S_emitted_value == 0);  // Not fired
    sub2.start_listening();
    CHECK(sub2.is_listening());
    NEO_EMIT(my_event{31});
    CHECK(S_emitted_value == 31);
    sub2.stop_listening();
    CHECK_FALSE(sub2.is_listening());
    NEO_EMIT(my_event{41});
    CHECK(S_emitted_value == 31);  // Didn't change
}

struct bubbling_event {
    int value = 44;
    enum { event_bubbles = true };
};

TEST_CASE("Auto-bubbling event") {
    static_assert(neo::event_bubbles<bubbling_event>);
    int saw_value = 0;
    {
        // Write the seen value
        NEO_LISTEN(bubbling_event ev) { saw_value = ev.value; };
        neo::emit(bubbling_event{3});
        CHECK(saw_value == 3);
        // Write the seen value + 22
        NEO_LISTEN(bubbling_event ev) { saw_value = ev.value + 22; };
        neo::emit(bubbling_event{61});
        // Even though we added 22, we still bubbled up to the prior handler
        CHECK(saw_value == 61);
        // Set the seen value to 42, and stop event bubbling
        {
            NEO_LISTEN(bubbling_event ev) {
                neo::cancel_bubbling(ev);
                saw_value = 42;
            };
            neo::emit(bubbling_event{7});
            CHECK(saw_value == 42);
        }
        neo::emit(bubbling_event{71});
        CHECK(saw_value == 71);
    }
}

struct abstract_event_base {
    virtual std::string_view name() const = 0;
};

template <typename T>
struct event_impl : abstract_event_base {
    using emit_as = abstract_event_base;

    std::string_view name() const override {
        static std::string type_name = neo::repr_type<T>().string();
        return type_name;
    }
};

TEST_CASE("Handle events with differing types") {
    {
        std::string_view saw_name;
        neo::listener    l = [&](const abstract_event_base& ev) { saw_name = ev.name(); };
        neo::emit(event_impl<std::int32_t>());
        CHECK(saw_name == "int32");
    }
    {
        std::string_view saw_name;
        auto l = neo::listen([&](const abstract_event_base& ev) { saw_name = ev.name(); });
        neo::emit(event_impl<double>{});
        CHECK(saw_name == "double");
    }
    {
        std::string_view saw_name;
        auto l = neo::listen<abstract_event_base>([&](auto&& ev) { saw_name = ev.name(); });
        neo::emit(event_impl<std::string>{});
        CHECK(saw_name == "std::string");
    }
}

struct event_with_return {
    using emit_result = int;

    int default_emit_result() const { return 42; }
};

TEST_CASE("Events that want a return type") {
    {
        auto l = neo::listen([](const event_with_return) { return 12; });
        CHECK(neo::emit(event_with_return{}) == 12);
    }
    // There is no handler, so we have a default return type.
    CHECK(neo::emit(event_with_return{}) == 42);
    {
        auto l = neo::listen([](event_with_return) {});
        // The handler does not return a value, so we return the default
        CHECK(neo::emit(event_with_return{}) == 42);
    }
    // Check that bubbling keeps the return value moving
    {
        auto l = neo::listen([](event_with_return const&) { return 88; });
        CHECK(neo::emit(event_with_return{}) == 88);
        auto l2 = neo::listen([](event_with_return ev) { return neo::bubble_event(ev) + 3; });
        CHECK(neo::emit(event_with_return{}) == 91);
    }
}
