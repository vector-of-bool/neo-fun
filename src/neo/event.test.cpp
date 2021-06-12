#include <neo/event.hpp>
#include <neo/repr.hpp>

#include <catch2/catch.hpp>

using neo::repr_ostream_operator::operator<<;

struct my_event {
    int value;
};

TEST_CASE("Install a handler") {
    int emitted_value = 0;

    {
        auto _ = neo::subscribe([&](const int& v) { emitted_value = v; });
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
        auto sub = neo::subscribe([&](const my_event ev) { emitted_value = ev.value; });
        CHECK(emitted_value == 3);
        neo::emit(my_event{42});
        CHECK(emitted_value == 42);
        neo::emit(44);  // Different type, differen subscribers
        CHECK(emitted_value == 42);

        CHECK(neo::repr(sub).string()
              == "tuple{neo::scoped_subscription<...>{active=false, tail=true}}");
        CHECK(neo::repr_value(sub).string() == "{{active=false, tail=true}}");

        {
            auto sub2 = neo::subscribe([](const my_event&) {});  // Do nothing
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
            neo::subscription filter_sub = [](my_event e) {
                // Only bubble-up the event if the value is odd
                if (e.value % 2 == 1) {
                    neo::bubble_event(e);
                }
            };
            NEO_SUBSCRIBE(my_event ev) { neo::bubble_event(ev); };
            neo::emit(my_event{312});
            CHECK(emitted_value == 33);  // Didn't change
            NEO_EMIT(my_event{311});
            CHECK(emitted_value == 311);  // Changed!
        }
    }
}

static int S_emitted_value = 0;

TEST_CASE("Optional handler") {
    int emitted_value = 0;

    neo::opt_subscription sub = [&](my_event ev) { emitted_value = ev.value; };
    NEO_EMIT(my_event{23});
    CHECK(emitted_value == 0);  // Not active yet
    sub.subscribe();
    NEO_EMIT(my_event{42});
    CHECK(emitted_value == 42);  // Subscribed this time
    sub.unsubscribe();
    NEO_EMIT(my_event{41});
    CHECK(emitted_value == 42);  // Did not change

    neo::opt_subscription<decltype([](my_event ev) { S_emitted_value = ev.value; })> sub2;
    CAPTURE(sub2);
    NEO_EMIT(my_event{5});
    CHECK(S_emitted_value == 0);  // Not fired
    sub2.subscribe();
    CHECK(sub2.is_subscribed());
    NEO_EMIT(my_event{31});
    CHECK(S_emitted_value == 31);
    sub2.unsubscribe();
    CHECK_FALSE(sub2.is_subscribed());
    NEO_EMIT(my_event{41});
    CHECK(S_emitted_value == 31);  // Didn't change
}
