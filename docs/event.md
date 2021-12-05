# `neo/event.hpp` - Generic Thread-Local Synchronous Event Dispatch

`neo/event.hpp` provides a generic, declarative, zero-allocation, and
(nearly-)zero-overhead framework for defining, emitting, and handling
synchronous events within a single thread.

As a single example, this can be used to deliver operation progress information
from a low-level API to a higher-level API:

```c++
struct ev_progress {
  double value;
};

void download(string url) {
  // Put a listener in scope
  neo::listener l = [](ev_progress ev) {
    print("Download is {}% complete", ev.value * 100);
  };
  // Run the download
  auto filename = compute_filename(url);
  download_to_file(url, filename);
}

void download_to_file(string url, path dest) {
  // Open a file
  ofstream ofile{dest};
  download_to_ostream(ofile, url);
}

void download_to_ostream(ostream& out, string url) {
  // Start the request
  auto resp = init_request(url);
  stream_response_body(out, resp);
}

void stream_response_body(ostream& out, http_response& resp) {
  // Loop until we finish
  while (!resp.done()) {
    // Save some of the response body to the file
    resp.recv_some_body(out);
    // Calculate the progress and emit an event
    double progress = resp.calculate_progress();
    neo::emit(ev_progress{progress});
  }
}
```

This is only a single example use case. One can define arbitrarily many event
types and use them for whatever one deems important.


## Example: `new` and `delete`

As another example, suppose we replace global `operator new` and
`operator delete` with versions that emit events:

```c++
struct ev_new {
  void*  ptr;
  size_t size;
};
struct ev_delete {
  void* ptr;
};

void* operator new(size_t size) {
  void* ptr = real_allocator(size);
  neo::emit(ev_new{ptr, size});
  return ptr;
}

void operator delete(void* ptr) noexcept {
  neo::emit(ev_delete{ptr});
  real_deallocate(ptr);
}
```

Because the `neo::emit()` will traverse arbitrarily many call stacks, we will
receive an event any time any component in the program calls the global `new`
and `delete`. This information could then be recorded and later reconstructed to
form a memory profile of the program.

Whereas one could simply do this logging in the `new` and `delete` functions
themselves, this is more flexible in that it allows the rest of the program to
change and modify the behavior of these event handlers, potentially at runtime.
Additionally, for example, one part of the program could override the thread's
event handlers to suppress or modify the behavior of these events.


## Example: `neo-sqlite3`

The `neo-sqlite3` library dispatches events for the purpose of logging and
auditing. For example, `neo::sqlite3::statement::step()`:

```c++
struct event::step {
  statement& stmt;
  errc       rc;
};
// ...
errable<void> statement::step() noexcept {
  auto rc = errc{::sqlite3_step(c_ptr())};
  neo::emit(event::step{*this, rc});
  return rc;
}
```

With this event, one can inspect every database operation, regardless of who
requested it:

```c++
int main() {
  neo::listener stmt_logger = [](neo::sqlite3::event::step ev) {
    print("statement::step(): statement={}, result={}, row={}",
          neo::repr(ev.stmt),
          neo::repr(ev.rc),
          neo::repr(stmt.row()));
  };
  // ... Rest of application goes here ...
}
```

This example will simply generate a log message each time someone advances a
database statement.


# The APIs

`neo/event.hpp` defines a small few APIs:

- `class listener<Handler, Event>` - A class template that holds an event
  handler and manages the listening stack. Use of CTAD is supported and
  recommended.
- `auto listen(Handlers...)` - A function that generates a set of listeners for
  the given handlers. Allows you to declare several listeners and store them in
  a single local variable.
- `auto listen<Event>(Handler)` - A function that listens for an event of type
  `Event` and dispatches that to `Handler`. Useful if `Handler` is generic.
- `void emit(Events...)` - Function that emits the given events to their
  appropriate handlers.
- `auto emit(Event)` - If given a single event, `neo::emit` can return a value
  from the listener.
- `auto bubble_event(Event)` - Called from within an event handler for `Event`,
  sends the given event to the next handler in the chain following the currently
  executing handler.


## Creating an Event Type

`neo/event.hpp` does not require that the event type be marked up in any way.
There is no base class to inherit from, nor any specially required members. It
is recommended that events be given a qualified name, either with an `ev_`
prefix or by living in an `event`/`events` `namespace`.

`neo/event.hpp` places no semantic or syntactic requirements on event types.


## Emitting an Event

There are two ways to generate an emit an event. The simplest is to pass the
event object directly:

```c++
neo::emit(ev_progress{.value = calc_progress()});
```

`neo::emit` will conditionally dispatch the given event. If there is no listener
in the current thread then event will be dropped.

Even still, this direct approach will construct the event object, which itself
may be expensive to compute. Unless the compiler can safely shift the event
construction to be within the conditional, we will pay for construction of the
event regardless of whether there is a listener.

As one solution conditionally call `emit()`:

```c++
if (neo::has_listener<ev_progress>()) {
  neo::emit(ev_progress{.value = calc_progress()});
}
```

but this might look a bit ugly, especially if we have a lot of events we wish to
emit. Instead, we have a second method of generating an event: `emit()` allows
one to pass an *event factory* function:

```c++
neo::emit([&] { return ev_progress{.value = calc_progress()}; });
```

The event factory should be invocable with zero arguments and return the event
that should be emitted.

With `neo/tl.hpp`, this can be further reduced to

```c++
neo::emit(NEO_TL(ev_progress{.value = calc_progress()}));
```

However, this is such a common and convenient pattern that `neo/event.hpp`
defines a macro:

```c++
NEO_EMIT(ev_progress{.value = calc_progress()});
```


## Handling an Event

If one wishes to handle an event, there are a few APIs, but they all boil down
to the `neo::listener<Handler, EventType>` class template. To start, a listener
can be declared and its type arguments deduced via CTAD:

```c++
neo::listener on_progress = [&](ev_progress pr) {
  update_progress_gui(pr.value);
};
```

Additionally, the `Handler` parameter can be fulfilled alone, and the
`EventType` will be deduced:

```c++
struct handler_group {
  neo::listener<decltype([](ev_progress) {
    update_progress_gui(pr.value);
  })> on_progress;
};
```

e.g. this is required to declare a listener as a member of a class (until we
someday get non-static data member type deduction).

One can also generate a listener by calling the `listen` function template:

```c++
auto on_progress = neo::listen([](ev_progress pr) {
  update_progress_gui(pr.value);
});
```

The `neo::listen` function can be called with multiple invocable objects, and
they will each generate a listener stored in a tuple in the return value of
`listen()`. The return type of `neo::listen` should be deduced and not specified
manually.

All of the above listener-declaration methods have a requirement in common: The
invocable handler *must* be a concrete invocable that accepts a single argument.
For example, this will not work:

```c++
neo::listener on_progress = [](auto pr) { ... };
```

CTAD has no way to figure out what event type the given closure is supposed to
handle.

There is still a solution though, and will be required if you wish to use a
generic handler:

```c++
struct event_logger {
  ostream& out;

  void operator()(const auto& event) {
    out << "Got an event: " << neo::repr(event) << '\n';
  }
};

int main() {
  event_logger logger{std::cerr};
  auto on_progress = neo::listen<ev_progress>(logger);
  auto on_warning  = neo::listen<ev_warning>(logger);
  auto on_error    = neo::listen<ev_error>(logger);
}
```

In the above, we create a single handler object `logger` which is has a generic
`operator()`. We generate three listeners by calling `neo::listen` and providing
an explicit template argument specifying the event type that we wish to listen
for.


## Event Bubbling

When `neo::emit()` finds a listener for an event, it will pass the event to that
handler and return. If we constructed a listener for an event `E` when there was
already a prior listener in the thread for `E`, the prior listener will not
receive events for `E`. That is, events do not "bubble up" be default.


### Manual Bubbling

An event handler can explicitly bubble-up the event during its own execution by
passing the event to `neo::bubble_event`:

```c++
neo::listener on_warning = [](ev_warning ev) {
  print("Something's fishy: {}", ev.message);
  neo::bubble_event(ev);
};
```

`bubble_event()` will synchronously dispatch the given event to the next
listener in the chain following the listener that called `bubble_event()`.
**Note:** If `bubble_event(E)` is called outside of the currently executing `E`
handler, the program will terminate!

The event object does not need to be the same object that was passed to the
handler. It need only be of the same type as the handler. Additionally, because
bubbling happens explicitly, one can make event bubbling conditional:

```c++
neo::listener warning_filter = [](ev_warning ev) {
  if (should_keep_warning(ev)) {
    neo::bubble_event(ev);
  }
};
```


### Auto-Bubbling Events

An event type itself can declare that it is auto-bubbling by providing a static
constant expression `event_bubbles`:

```c++
struct my_event {
  std::string_view message;
  enum { event_bubbles = true };
};
```

This will cause the event to bubble-up the chain of handlers automatically. It
is possible to check whether a given event type will auto-bubble using the
`neo::event_bubbles<E>` concept. It is illegal to call `bubble_event()` on an
auto-bubbling event.

If a handler wishes to prevent this auto-bubbling behavior, the handler may call
`neo::cancel_bubbling(e)`. Like with `bubble_event`, this may only be called
within the currently executing event handler for `e`. It is illegal to call
`cancel_bubbling()` for a type that does not auto-bubble.

**Note** that if an event is auto-bubbling (as above), it is not allowed to also
have a non-`void` `emit_result_t` (discussed below).


## Blocking Events

If you know that a callee is going to emit an event and you want to prevent it
from reaching beyond your current scope, you can block events using a
`neo::event_blocker<E>`:

```c++
void download_no_progress(string url) {
  // Prevent ev_progress from escaping
  neo::event_blocker<ev_progress> no_progress;
  download(url);
}
```

`neo::event_blocker` installs a listener that is a no-op and calls
`cancel_bubbling()` on events. This may be useful to prevent events from firing
during their own handler and causing an infinite recursion:

```c++
void do_operation_with_logging() {
  neo::listener on_error = [](ev_error err) {
    // Block event errors from the `log_error()` function
    neo::event_blocker block_errors{err};
    // log_error() might fire more ev_error events, and we don't want
    // to infinitely recurse on itself.
    log_error(err);
  };
  do_operation_with_errors();
}
```


## Events with Return Values

If an event type has a nested type `emit_result`, then it is treated as an event
with a "emit result".

The type of an event's emit-result can be taken from `neo::emit_result_t<E>`. If
`E` does not define a nested type `emit_result`, then `emit_result_t<E>` is
`void`. If the `E::emit_result` type is `void`, then it is indistinguishable
from an event that has no emit result.

If the `emit_result_t` is non-`void`, then the event's handler has the option to
return a value. The return value of the handler must be convertible to the
`emit_result_t` of the event that it handles.

When an event with a non-`void` emit-result is passed to `neo::emit` as the sole
argument, `neo::emit` will return an instance of that emit-result. If there is
an active and listening handler that returns non-`void`, the return value of
that handler will be returned from `neo::emit`. If there is no such handler,
then a default result value will be either default constructed or obtained by
calling the `.default_emit_result()` member function on the emitted event
object. For this reason, if `emit_result_t<E>` is non-`void`, then:

1. For `const E& ev`, `ev.default_emit_result()` must be a valid expression
   evaluating to a value convertible to `emit_result_t<E>`, **OR**
2. `emit_result_t<E>` must be default-constructible.

As an example use case, this can be used to implement generic cancellation for a
synchronous API:

```c++
struct ev_http_progress {
  size_t expected_size;
  size_t downloaded_size;
  // An event handler can tell us to either stop or to continue
  enum emit_result {
    keep_going,
    cancel,
  };
};

// Download a large file. Can be stopped with the given stop_token
void download_file(string url, stop_token stop) {
  neo::listen cancellation = [&](ev_http_progress progress) {
    // Update our UI
    update_progress_ui(progress);
    // Check whether we should stop or continue
    if (stop.stop_requested()) {
      return progress.cancel;
    } else {
      return progress.keep_going;
    }
  };
  do_long_running_download(url);
}

// ... Several API layers later ...

void write_http_response_body(http_repsonse resp, std::ostream& out) {
  size_t acc_size = 0;
  while (!resp.done()) {
    // Receive a part and write it
    auto part = resp.recv_some();
    out.write(part.data(), part.size());
    acc_size += size;
    // Send an event
    auto do_cancel = neo::emit(
        ev_http_progress{
          .expected_size = resp.content_length(),
          .downloaded_size = acc_size,
        });
    // Check if a handler requested that we stop
    if (do_cancel == ev_http_progress::cancel) {
      break;
    }
  }
}
```

**Note** that if an event has a non-`void` `emit_result_t`, then that event
cannot also be an auto-bubbling event.


## Polymorphic Events

Unlike exception handling, where `catch (const some_base_class&)` will handle
`throw some_derived_class()`, this event system will not see a link between a
base class and a derived class.

To work around this issue, we can instead use `emit_as` to tell `neo::emit` to
have `emit()` automatically cast the event to another type before propagating
the event. Here is an example using the tired "Animal" inheritance example:

```c++
struct animal_event {
  virtual string sound() const = 0;
  using emit_as = animal_event;
};

struct cat_event : animal_event {
  string sound() const override { return "meow"; }
};

struct dog_event : animal_event {
  string sound() const override { return "bark"; }
};
```

Now, if we ever `emit()` a subclass of `animal_event`, it will automatically be
emitted and handled as an `animal_event`:

```c++
void foo() {
  neo::listener = [](const animal_event& ev) {
    print("This animal says '{}'", ev.sound());
  };
  neo::emit(cat_event{});
  neo::emit(dog_event{});
}
```
