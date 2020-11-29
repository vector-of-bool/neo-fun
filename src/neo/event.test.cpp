#include <neo/event.hpp>

#include <catch2/catch.hpp>

void handler() {}

struct my_event {
    int value;
};

TEST_CASE("Install a handler") {
    int emitted_value = 0;

    {
        auto _ = neo::subscribe([&](int v) { emitted_value = v; });
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
        auto sub = neo::subscribe([&](my_event ev) { emitted_value = ev.value; });
        CHECK(emitted_value == 3);
        neo::emit(my_event{42});
        CHECK(emitted_value == 42);
        neo::emit(44);  // Different type, differen subscribers
        CHECK(emitted_value == 42);

        {
            auto sub2 = neo::subscribe([](my_event) {});  // Do nothing
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
            auto filter_sub = neo::subscribe([](my_event e) {
                // Only bubble-up the event if the value is odd
                if (e.value % 2 == 1) {
                    neo::bubble_event(e);
                }
            });
            neo::emit(my_event{312});
            CHECK(emitted_value == 33);  // Didn't change
            neo::emit(my_event{311});
            CHECK(emitted_value == 311);  // Changed!
        }
    }
}
