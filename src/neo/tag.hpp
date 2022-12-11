#pragma once

namespace neo {

template <typename T>
struct is_type_parameterized_template {
    constexpr static bool value = false;
};

template <template <class...> class Tmpl, typename... Ts>
struct is_type_parameterized_template<Tmpl<Ts...>> {
    constexpr static bool value = true;
};

template <typename T>
constexpr bool is_type_parameterized_template_v = is_type_parameterized_template<T>::value;

/**
 * A simple tag type that holds an arbitrary number of types.
 */
template <typename...>
struct tag {};

/**
 * Variable template of a tag type
 */
template <typename... Ts>
constexpr tag<Ts...> tag_v = {};

template <auto...>
struct vtag {};

template <auto... Vs>
constexpr vtag<Vs...> vtag_v = {};

/**
 * Get the number of elements in a tag type.
 */
template <typename Tag>
auto tag_size_v = nullptr;

template <template <class...> class L, typename... Ts>
constexpr unsigned tag_size_v<L<Ts...>> = static_cast<unsigned>(sizeof...(Ts));

template <template <auto...> class L, auto... Vs>
constexpr unsigned tag_size_v<L<Vs...>> = static_cast<unsigned>(sizeof...(Vs));

/**
 * @brief An object that represents a compile-type type.
 *
 * It can be equality-compared with another type tag to check if they refer to the same type.
 */
class type_tag {
    int* _val{};

public:
    explicit type_tag(int* p)
        : _val(p) {}

    bool operator==(type_tag p) const noexcept { return _val == p._val; }

    template <typename T>
    struct make {
        static inline int     tag_object;
        constexpr static auto val = &tag_object;
    };
};

/** Create an instance of a type_tag for the given type */
template <typename T>
type_tag type_tag_v = type_tag(type_tag::make<T>::val);

/**
 * @brief Generate a tag object for the type arguments given to the provided template specialization
 *
 * @tparam T A template specialization that has only type parameters
 */
template <typename T>
struct template_args_tag {};

template <template <class...> class Tmpl, typename... Args>
struct template_args_tag<Tmpl<Args...>> {
    /// A tag representing the parameters of the template
    using type = tag<Args...>;
};

template <typename T>
using template_args_tag_t = typename template_args_tag<T>::type;

template <typename T>
constexpr auto template_args_tag_v = template_args_tag_t<T>{};

/**
 * @brief Get the type of the sole template argument to the given template specialization
 *
 * If the given type is a template specialization with a single argument, has a nested ::type member
 * that represents that argument.
 *
 * @tparam T A template specialization type.
 */
template <typename T>
struct sole_type {};

template <template <class...> class Tmpl, typename T>
struct sole_type<Tmpl<T>> {
    /// The type of the single template argument
    using type = T;
};

template <typename T>
using sole_type_t = typename sole_type<T>::type;

template <typename Seq>
struct concat_2;

// Concatenate sequences of just types
template <template <class...> class L, typename... Ts>
struct concat_2<L<Ts...>> {
    template <typename O>
    struct with;

    template <typename... Us>
    struct with<L<Us...>> {
        using type = L<Ts..., Us...>;
    };
};

// Concatenate sequences of constants
template <template <class C, C...> class L, class C, C... Ns>
struct concat_2<L<C, Ns...>> {
    template <typename O>
    struct with;

    template <C... Vs>
    struct with<L<C, Vs...>> {
        using type = L<C, Ns..., Vs...>;
    };
};

template <typename... Tags>
struct tag_concat;

// A single element is just itself:
template <typename One>
struct tag_concat<One> {
    using type = One;
};

// Concat two items:
template <typename L, typename R>
struct tag_concat<L, R> : concat_2<L>::template with<R> {};

// Concat three items:
template <typename A, typename B, typename C>
struct tag_concat<A, B, C> : tag_concat<typename tag_concat<A, B>::type, C> {};

// Concat four items:
template <typename A, typename B, typename C, typename D>
struct tag_concat<A, B, C, D>
    : tag_concat<typename tag_concat<A, B>::type, typename tag_concat<C, D>::type> {};

// Concat five or more items:
template <typename A, typename B, typename C, typename D, typename... Rest>
struct tag_concat<A, B, C, D, Rest...> : tag_concat<typename tag_concat<A, B>::type,
                                                    typename tag_concat<C, D>::type,
                                                    typename tag_concat<Rest>::type...> {};

}  // namespace neo
