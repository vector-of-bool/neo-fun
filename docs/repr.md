# `neo/repr.hpp` - Textual Representations of Objects and Types

The `repr` name comes from Python's own `repr()` built-in, which generates
string representations of objects at runtime. This is importantly different from
Python's `str()` converting constructor or C++ `to_string()` APIs. A `repr()`
of an object includes information about the structure and type of the object
being represented, and is mostly useful for debugging purposes.


## Basic Types and Simple Examples

For example, the following:

```c++
string s = "I am a string";
cout << s;
```

produces output `I am a string`, whereas:

```c++
string s = "I am a string";
cout << neo::repr(s);
```

produces the output `"I am a string"s`, where the string itself is quote-escaped
and the literal suffix `s` informs us that the value is of type `std::string`.
If instead:

```c++
string_view sv = "I am a string";
cout << neo::repr(sv);
```

this will produce `"I am a string"sv`, where the literal suffix `sv` tells us
that the value was a string view. Using the raw `ostream` `operator<<` would
produce the same output for the `s` and `sv` variables.

The same can be seen with integral values:

```c++
int32_t i = 53;
cout << i;            // Produces '53'
cout << neo::repr(i); // Produces '53:int32'
```

Where the integral value is annotated with it's exact type.


# The APIs

There are three callables in `neo/repr.hpp`:

- `neo::repr(v)` a callable object that takes a single value.
- `neo::repr_value(v)` a callable object that takes a single value.
- `neo::repr_type<T>()` and `neo::repr_type(v)` an overloaded function template
  that accepts a single type parameter *or* a single value.

The return types of these APIs is opaque and implementation-defined.
**Important:** The `neo::repr` and `neo::repr_value` callables return a
reference to their argument, so the arguments *must* live at least as long as
the return value of these callables.

The return-value of these APIs can be used in one of three ways:

- Feed into the `operator<<` stream-insertion operator of a `std::ostream&`.
- Feed as an interpolation argument of `neo::ufmt()`.
- Call the `.string()` method to return a new `std::string` of the
  representation of the argument.


## The `reprable` Concept

`neo/repr.hpp` defines a single public concept `reprable<T>`, which will
determine whether it is possible to pass an instance of `T` to any of the `repr`
APIs. If `T` is `reprable`, then it can be called "repr-able" or
"representable".


## Default `reprable` Types

`neo::repr` supports the following out-of-the-box:

- Integral types, floating point types, bool, and character types.
- Pointers. If the pointee is also representable and the pointer is non-null,
  then the pointed-to-value will be rendered inline.
- Any forward-range of representable objects.
- Associative containers like std::map whose key and mapped types are also
  representable.
- `std::basic_string` and `std::basic_string_view` objects.
- Any cvr-qualified type whose underlying type is also reprable.
- `pair` and `tuple`-like types whose elements are also representable.
- `optional`-like types whose inner type is also representable
- Path types like `std::filesystem::path`
- The type `void` can be passed to `repr_type<T>()` for `T`

Several other types in `neo-fun` are also `reprable` out.

Additional types can be made `reprable` through a simple and unintrusive customization API.


## Custom `repr()` for Your Types


To allow a type to be `repr()`'d, it must provide a non-member ADL-visible
`do_repr()` function which takes two arguments: The first is an
unspecified-type-deduced serializing object, and the second is a
pointer-to-const of the type to be serialized. The standard way to provide this
is as a hidden friend function defined within the class itself. For example, a
rectangle type:

```c++
namespace geo {
struct rectangle {
  int width = 0;
  int height = 0;

  // The do_repr() customization point function:
  friend constexpr void
  do_repr(auto out,
          const rectangle* self) noexcept
  {
      // ...
  }
};
}  // end namespace geo
```

Because the first parameter 'out' is deduced, the function itself is a template
function and need not pull in the `<neo/repr.hpp>` header in order to define
this customization point. Thus the provider of `rectangle` need not have neo-fun
as a direct dependency, and users of `rectangle` are not pulling in all of
`neo/repr.hpp` just to use the `rectangle` code.

The 'out' object is an unspecified type with the following interface:

```c++
struct __unspecified_repr_output_writer_type__ {
  constexpr static bool just_type;
  constexpr static bool just_value;

  template <typename T>
  constexpr static bool can_repr;

  void type(string_view fmt_str,
            formattable auto const&... args);
  void value(string_view fmt_str,
             formattable auto const&... args);
  void bracket_value(string_view fmt_str,
                     formattable auto const&... args);

  void append(std::string_view fmt_str,
              formattable auto const&... ts);

  template <reprable T>
  auto repr_type();
  auto repr(reprable auto const& object);
  auto repr_value(reprable auto const& value);
  auto repr_sub(reprable auto const& value);

  std::string& underlying_string();
};
```

The `out.append()` variadic method template takes a simplified
`std::format()`-style format string and a set of arguments to interpolate into
the string. The values are interpolated as-if by `std::format()`, and not as-if
being passed through `neo::repr()`. The format-string only supports the empty
format specifier `{}`.


### The Simple Case

The `out.type()`, `out.value()`, and `out.bracket_value()` variadic method
templates are similar to `append()` in that they take format-style strings and
interpolation parameters. The `type()`, `value()`, and `bracket_value()` methods
are intended for the most common use cases of implementing a very simple
`do_repr()` function. For example, A simple rendering of our `rectangle` class
may look like:

```c++
constexpr friend void
do_repr(auto out,
        const rectangle* self) noexcept {
  out.type("geo::rectangle");
  if (self) {
    out.bracket_value("width={}, height={}",
                      self->width,
                      self->height);
  }
}
```

> The `self` pointer must be checked against null, because `do_repr()` will also
> be called for `repr_type<T>()`, and there is no instance of `T` to point to!

Given a `rectangle r{3, 6}`, this simple `do_repr` function will emit the
following three representations:

- for `repr(r)`:

  > `geo::rectangle{width=3, height=6}`

- for `repr_type(r)`:

  > `geo::rectangle`

- for `repr_value(r)`:

  > `{width=3, height=6}`

The `bracket_value()` function will render the value within a pair of curly
brackets, and should be used if the value string contains whitespace or other
characters that would make the grouping non-obvious. The `value()` function will
omit the brackets in the case of `repr_value()`.

If using `.value()` or `.bracket_value()`, then either `.value()` or
`.bracket_value()` should be called *at most once*. If using `.type()`, then
`.type()` must be called *before* calling the `.value()` or `.bracket_value()`
function. Failure to follow these rules can result in a mangled `repr()` of the
object.


### `repr()` on sub-objects

In the interface of the `out` object, the `repr_type`, `repr`, and `repr_value`
methods are equivalent to their `neo::` equivalents. This allows you to use the
`neo::repr` APIs in your `do_repr()` function without needing to pull in the
`<neo/repr.hpp>` header. Because these are dependent names of a templated
parameter, the compiler won`t care to look up their definition until your
`do_repr()` function is used by `neo::repr` itself.

The `repr_sub()` function is special and depends on the value of `just_value`.
If `just_value` is `true`, then `repr_sub` is equivalent to `repr_value`,
otherwise it is equivalent to `repr`.

Whether or not you `repr()` a subobject or just pass it through a regular format
string depends on whether the type information is important or if the value
needs to be escaped. For example:

```c++
struct person {
  string name;
  int age;

  friend void do_repr(auto out, const person* self) {
    out.type("person");
    if (self) {
      out.value("name={}, age={}",
                out.repr_value(self->name),
                self->age);
    }
  }
};
```

In the above, we `out.repr_value(self->name)`, because we want the string
content to be quoted properly:

> `person{name="John Doe", age=41}`

If we *had not* passed `self->name` through `repr_value`, it would have appeared
as:

> `person{name=John Doe, age=41}`


## `do_repr()` Conventions

There is no fixed standard on how objects should be `repr`'d, but the following
is recommended:

- Types should be `repr`'d as concisely, yet accurately, as possible.
- Redundant type information should be omitted.
- If there are/could be multiple subobjects, then the value should be rendered
  within a pair of curly brackets.
- If rendering both type and value on a range-like type, the type of the
  subobjects should be included in the type rendering of the range, but not
  as part of the range values. For example, `vector<int>` should be
  rendered as `vector<int>{repr_value(values)...}`, since `repr_value` will
  not include type annotations of its rendered parameters.
- If template type parameters can be inferred from the types of subobjects, then
  the object type name should omit its template parameters and instead emit the
  type information on its subobjects. An exception is when `out.just_type` is
  true, in which case the template parameters should be rendered as
  appropriately. For example, `repr()` of
  `reference_wrapper<path>{"/some/path"}` should render as
  `reference_wrapper{path{"/some/path"}}`.
- For aggregate-like types with named subobjects, the aggregate's value should
  be enclosed in curly brackets, with each subobject's name included as
  `name=value` pairs, separated by commas.


## More Complex `repr()` Output

While `.type()`, `.value()`, and `.bracket_value()` will be useful for the majority of objects, we can do more complex representations as-needed. A `do_repr()` must be valid for three cases:

```c++
constexpr void do_repr(auto out, const my_object* self) {
  if constexpr (out.just_type) {
    // ... Only render our type
  } else if constexpr (out.just_value) {
    // ... Only render our value
  } else {
    // ... Render a value with annotated type
  }
}
```

The `.type()`, `.value()`, and `.bracket_value()` functions will handle these
three cases automatically. The reasoning for the above rules and the three
different cases is to reduce the appearance of redundant type information in the
generated output. For example, if we have a 'vector<geo::rectangle>', this will
be rendered as:

- `repr_type`:

  > `vector<geo::rectangle>`

- `repr(vec)`:

  >`vector<geo::rectangle>{{width=2, height=4}, {width=5, height=11}}`

- `repr_value(vec)`:

  > `{{width=2, height=4}, {width=5, height=11}}`

Because the `repr(vec)` already renders its element type in
`vector<geo::rectangle>`, we can omit the type information on each element of
that vector, rather than rendering it repeatedly.

For other types in which the type of named subobjects is not obvious as
rendered template arguments, we may want to include those in the rendered
subobjects by using 'out.repr()':

```c++
template <typename AgeUnits, typename HeightUnits>
class person {
public:
  std::string_view name();
  AgeUnits age();
  HeightUnits height();

  friend constexpr void do_repr(auto out, const person* self) {
    if constexpr (out.just_type) {
      out.append(
        "person<AgeUnits={}, HeightUnits={}>",
        out.template repr_type<AgeUnits>(),
        out.template repr_type<HeightUnits>());
    } else if constexpr (out.just_value) {
      // Only include the values without additional type information
      out.append(
        "{name={}, age={}, height={}}",
        out.repr_value(self->name()),
        out.repr_value(self->age()),
        out.repr_value(self->height()));
    } else {
      // Include types with 'out.repr'
      out.append(
        "person{name={}, age={}, height={}}",
        out.repr_value(self->name()),  // Type is not relevant
        out.repr(self->age()),
        out.repr(self->height()));
    }
  }
};
```

This will render `person<int, double> p` as:

- `repr_type(p)`:

  > `person<AgeUnits=int32, HeightUnits=double>`

- `repr(p)`:

  > `person{name="Joe", age=34:int32, height=2.1}`

- `repr_value(p)`:

  > `{name="joe", age=34, height=2.1}`

And therefore a `vector<person<int, double>> v` as:

- `repr_type(v)`:

  > `vector<person<AgeUnits=int, HeightUnits=double>>`

- `repr(v)`:

  > ```
  > vector<person<AgeUnits=int, HeightUnits=double>>{
  >     [name="joe", age=34, height=2.1],
  >     [name="jane", age=42, height=1.7]}
  > ```

- `repr_value(v)`:

  > `{[name="joe", age=34, height=2.1], [name="jane", age=42, height=1.7]}`

