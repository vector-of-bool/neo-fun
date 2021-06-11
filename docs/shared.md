# `neo/shared.hpp` - Grant Shared-State Semantics to Arbitrary Types

[Smart pointers make for bad public APIs](https://vector-of-bool.github.io/2018/12/02/smart-pointer-apis.html).
Nevertheless, we often wish to grant the semantics of "shared-ness" to an
object. Instead of just wrapping things in `std::shared_ptr`, you can use
`neo/repr.hpp` to create a class that encapsulates the "shared-ness" of a
`shared_ptr` without the downsides, and with several upsides.

The downsides of using `std::shared_ptr` that are fixed with `neo/shared.hpp`:

- `shared_ptr<T>` has a null-state, which makes it more difficult to understand
  the invariants of your program.
- Duplicating or "un-sharing" a `shared_ptr` requires a dance with
  `make_shared`.
- The intention behind a `shared_ptr<T>` is less clear than if you just had a
  `T`.

`neo/shared.hpp` offers several other benefits and additional APIs, outlined in this document below.


# Creating a Shared Type

To first create a shared type, one must declare a class which represents that
shared-state. For example, a rectangle:

```c++
struct rectangle {
    int width = 1;
    int height = 1;

    constexpr bool is_square() const noexcept {
        return width == height and width != 0;
    }
};
```

Now, to create a "shared" version, define a new class which inherits from the
`neo::shared_state` class template:

```c++
struct shared_rectangle
    : neo::shared_state<shared_rectangle,  // First: our own class
                        rectangle>         // Second: the shared-state
{
    using shared_state::shared_state;  // Inherit the constructors
};
```

And that's all there is to it! `shared_rectangle` is now a class which imposes
shared-state semantics with `rectangle`. There are a few things to note about
the new `shared_rectangle` type:

- To access the members of the `rectangle` shared state, one must use
  `operator->` on instances of `shared_rectangle`
- `shared_rectangle`, despite using `operator->`, is not "pointer-like." It does
  not have an `operator*`, and it is never "null." A default-constructed
  `shared_rectangle` contains a default-constructed shared state.


## Semantics and Operations on Shared-State Objects

The following semantics are granted to `shared_rectangle` by its inheritance of
`neo::shared_state`:

- Copying a `shared_rectangle` is equivalent to copying a `shared_ptr`: It is
  always `noexcept`, itis a constant-time operation, and the new object shares
  the same underlying state. Manipulating the shared state through `operator->`
  will cause all descendent copies of `shared_rectangle` to "see" the same
  updates, since they point to the same object.
- Assigning a `shared_rectangle` from another will "rebind" the shared state of
  the assigned-to object to the shared state of the assigned-from object.
- The underlying shared state will live until the last descendent instance of
  `shared_rectangle` is destroyed.
- Because it is not a pointer and has "never-null" semantics, `shared_rectangle`
  will bind to references to the underlying shared state. That is, the following
  is valid:

  ```c++
  void foo(const rectangle& rect);

  void bar(shared_rectangle r) {
      // Bind the shared-state within 'r' to
      // the 'rect' reference parameter:
      foo(r);
  }
  ```
- The `shared_rectangle` has reference semantics, and is **shallow-const**. This
  means that the following *is valid!*

  ```c++
  void bar(const shared_rectangle& r) {
      r->width = 6;  // Okay!
  }
  ```

  To use reference-to-const semantics, one must use `neo::ref_to_const<T>`. See
  below for more information.


## Shared-State Manipulation and Operation

The following APIs are available on an instance of any class that inherits from
a `neo::shared_state`. (Note that these are attached to the reference, and not
to the shared state, so should be accessed via regular `.` dot).

- `shared_instance.clone()` creates a new instance of `shared_type` by *copying*
  the underlying shared state. The new instance does not share its state with
  the original, but has the same initial contents.
- `shared_instance.unshare()` will duplicate the shared-state in the instance
  and give this instance its own new copy to work with. Equivalent to
  `shared_instance = shared_instance.clone()`.
- `shared_instance.use_count()` returns the number of strong references to the
  underlying shared state.


## References-to-Const

While a `shared_rectangle` can bind to a `const rectangle&`, this may not be
what we want, as the new reference will not extend the lifetime of the
shared-state that it refers to.

Instead, we can use `neo::ref_to_const<shared_rectangle>`. This has the same API
as the `shared_rectangle`, except that `operator->` returns a pointer-to-const
of the shared state. It is impossible to manipulate the underlying shared state
through a `neo::ref_to_const`. It is still possible to `clone()` a
`ref_to_const`, and the new instance will have its own state that is
non-`const`. The template parameter of `ref_to_const` should be the
shared-reference class, and not the shared-state class: i.e.
`ref_to_const<shared_rectangle>` is valid, but `ref_to_const<rectangle>` is
*invalid*.

A `ref_to_const<T>` can bind to a `const T::state_type&`, but cannot bind to a
non-const `T::state_type&`.

`ref_to_const` supports CTAD directly:

```c++
sharec_rectangle  rect;
neo::ref_to_const cref = rect;  /// Works as expects
```


## Weak References and Weak References-to-Const

Like `shared_ptr` has `weak_ptr`, `neo/repr.hpp` has `neo::weak_ref`. This is a
class template, and the template argument should be a (possibly-`const`
qualified) shared-reference class (e.g. `shared_rectangle`).

Like `weak_ptr`, one cannot directly access the pointed-to data. Instead, one
must call `.lock()` on the `weak_ref<T>`, which will return a `std::optional`
for the shared-reference type. If there are no non-weak `T` instances referring
to the shared state, `lock()` will return `std::nullopt`.

If the template argument to `weak_ref` is a `const T`, then `lock()` returns a
`std::optional<ref_to_const<T>>`, otherwise it returns a `std::optional<T>`.

Like `ref_to_const`, `weak_ref` supports CTAD, and is convertible from both a
shared-reference type `T` and a `ref_to_const<T>`. A `weak_ref<const T>` can be
created from a `T` or a `ref_to_const<T>`, but a `weak_ref<T>` (with non-`const`
`T`) cannot be created from a `ref_to_const<T>`.

```c++
shared_rectangle rect;
neo::weak_ref          weak1 = rect; // Okay
neo::weak_ref<T>       weak2 = rect; // Okay
neo::weak_ref<const T> weak3 = rect; // Okay

// NOT Okay:
neo::weak_ref<T> weak4 = neo::ref_to_const{rect};
```