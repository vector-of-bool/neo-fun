#pragma once

#include <cinttypes>
#include <cstddef>

namespace neo {

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
constexpr std::size_t tag_size_v;

template <typename... Ts>
constexpr std::size_t tag_size_v<tag<Ts...>> = sizeof...(Ts);

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

template <typename T>
inline auto type_tag_v = type_tag(type_tag::make<T>::val);

}  // namespace neo
