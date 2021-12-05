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
constexpr tag<Ts...> tag_v;

/**
 * Get the number of elements in a tag type.
 */
template <typename Tag>
requires is_type_parameterized_template_v<Tag>
int tag_size_v = 0;

template <template <class...> class Tmpl, typename... Ts>
requires is_type_parameterized_template_v<Tmpl<Ts...>>
constexpr unsigned tag_size_v<Tmpl<Ts...>> = static_cast<unsigned>(sizeof...(Ts));

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

}  // namespace neo
