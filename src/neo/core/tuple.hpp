#pragma once

#include "./tuple.detail.hpp"

#include <neo/attrib.hpp>
#include <neo/concepts.hpp>
#include <neo/get.hpp>
#include <neo/meta.hpp>

#include <utility>

namespace neo {

// Tag used to select the piece-wise constructor for a tuple
struct tuple_construct_tag {};
// Tag to select a tuple->tuple converting constructor
struct tuple_convert_tag {};

}  // namespace neo

namespace neo::core {

/**
 * @brief A core::tuple is like a full neo::tuple but provides a simpler interface
 * upon which the more expressive interface is constructed.
 */
template <typename... Ts>
class tuple {
public:
    using index_sequence = std::make_index_sequence<sizeof...(Ts)>;

    template <std::size_t N>
    using nth_type = nonvoid_t<meta::pack_at<N, Ts...>>;

private:
    using boxes = _tuple_detail::boxes<tuple>;

    NEO_NO_UNIQUE_ADDRESS boxes _boxes;

    template <std::size_t N>
    using _nth_box = _tuple_detail::nth_box<N, meta::pack_at<N, Ts...>>;

public:
    tuple() = default;

    /**
     * @brief Tuple per-element-converting constructor.
     *
     * @param tag A tag to select this constructor
     * @param args... Arguments that will be converted to the tuple elements.
     */
    template <typename... Args>
    constexpr explicit tuple(tuple_construct_tag,
                             Args&&... args)  //
        noexcept((nothrow_constructible_from<nonvoid_t<Ts>, Args> and ...))
        : _boxes(std::in_place, NEO_FWD(args)...) {}

    /**
     * @brief From-tuple converting constructor. Converts from another tuple-like
     * object (using neo::get_nth<N>) for each tuple element.
     *
     * @param tag The tag to select this constructor
     * @param tpl The tuple-like object to construct from. Elements will be perfect-forwarded from
     * this tuple
     */
    template <typename Tpl, auto... N>
    constexpr explicit tuple(tuple_convert_tag,
                             Tpl&& tpl,
                             std::index_sequence<N...>)  //
        noexcept((nothrow_constructible_from<nonvoid_t<Ts>, get_nth_t<Tpl, N>> and ...))
        : _boxes(std::in_place, get_nth<N>(NEO_FWD(tpl))...) {}

    template <std::size_t N>
    constexpr add_lvalue_reference_t<nth_type<N>> get() & noexcept {
        auto& box = static_cast<_nth_box<N>&>(_boxes);
        return static_cast<add_lvalue_reference_t<nth_type<N>>>(box.get());
    }

    template <std::size_t N>
    constexpr add_lvalue_reference_t<const nth_type<N>> get() const& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_boxes);
        return static_cast<add_lvalue_reference_t<const nth_type<N>>>(box.get());
    }

    template <std::size_t N>
    constexpr add_rvalue_reference_t<nth_type<N>> get() && noexcept {
        auto& box = static_cast<_nth_box<N>&>(_boxes);
        return static_cast<add_rvalue_reference_t<nth_type<N>>>(box.get());
    }

    template <std::size_t N>
    constexpr add_rvalue_reference_t<const nth_type<N>> get() const&& noexcept {
        auto& box = static_cast<const _nth_box<N>&>(_boxes);
        return static_cast<add_rvalue_reference_t<const nth_type<N>>>(box.get());
    }

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;
};

// Specialization of an empty tuple
template <>
class tuple<> {
public:
    using index_sequence = std::index_sequence<>;

    tuple() = default;
    constexpr explicit tuple(tuple_construct_tag) noexcept {}
    template <typename Tpl>
        requires(not can_get_nth<Tpl, 0>)
    constexpr explicit tuple(tuple_convert_tag, Tpl&&, std::index_sequence<>) noexcept {}

    template <std::size_t>
    void get() = delete;

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;
};

// Specialization for a 1-element tuple
template <typename Single>
class tuple<Single> {
    using Type = nonvoid_t<Single>;
    NEO_NO_UNIQUE_ADDRESS object_t<Single> _value;

public:
    using index_sequence = std::index_sequence<0>;

    tuple() = default;

    template <explicit_convertible_to<Type> Arg>
    constexpr explicit tuple(tuple_construct_tag,
                             Arg&& arg)  //
        noexcept(nothrow_constructible_from<Type, Arg>)
        : _value(NEO_FWD(arg)) {}

    template <typename Tpl>
        requires(can_get_nth<Tpl, 0>)
    constexpr explicit tuple(tuple_convert_tag, Tpl&& tpl, std::index_sequence<0>) noexcept(
        nothrow_constructible_from<Type, get_nth_t<Tpl, 0>>)
        : _value(get_nth<0>(NEO_FWD(tpl))) {}

    template <std::size_t N>
        requires(N == 0)
    constexpr add_lvalue_reference_t<Type> get() & noexcept {
        return static_cast<add_lvalue_reference_t<Type>>(_value);
    }

    template <std::size_t N>
        requires(N == 0)
    constexpr add_const_reference_t<Type> get() const& noexcept {
        return static_cast<add_const_reference_t<Type>>(_value);
    }

    template <std::size_t N>
        requires(N == 0)
    constexpr add_rvalue_reference_t<Type> get() && noexcept {
        return static_cast<add_rvalue_reference_t<Type>>(_value);
    }

    template <std::size_t N>
        requires(N == 0)
    constexpr add_rvalue_reference_t<add_const_t<Type>> get() const&& noexcept {
        return static_cast<add_rvalue_reference_t<add_const_t<Type>>>(_value);
    }

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;
};

// Specialization for a 2-element tuple
template <typename T, typename U>
class tuple<T, U> {
    NEO_NO_UNIQUE_ADDRESS object_t<T> _first;
    NEO_NO_UNIQUE_ADDRESS object_t<U> _second;

    using Tnv = nonvoid_t<T>;
    using Unv = nonvoid_t<U>;

    template <std::size_t N>
    using _type = conditional_t<N, Unv, Tnv>;

public:
    using index_sequence = std::index_sequence<0, 1>;

    tuple() = default;

    template <convertible_to<Tnv> Arg1, convertible_to<Unv> Arg2>
    constexpr explicit tuple(tuple_construct_tag,
                             Arg1&& arg1,
                             Arg2&& arg2)  //
        noexcept(nothrow_constructible_from<Tnv, Arg1> and nothrow_constructible_from<Unv, Arg2>)
        : _first(NEO_FWD(arg1))
        , _second(NEO_FWD(arg2)) {}

    template <typename Tpl>
        requires(can_get_nth<Tpl, 1>)
    constexpr explicit tuple(tuple_convert_tag, Tpl&& tpl, std::index_sequence<0, 1>) noexcept(
        nothrow_constructible_from<Tnv, get_nth_t<Tpl, 0>>
        and nothrow_constructible_from<Unv, get_nth_t<Tpl, 1>>)
        : _first(get_nth<0>(NEO_FWD(tpl)))
        , _second(get_nth<1>(NEO_FWD(tpl))) {}

    template <std::size_t N>
    constexpr _type<N>& get() & noexcept {
        if constexpr (N)
            return static_cast<Unv&>(_second);
        else
            return static_cast<Tnv&>(_first);
    }

    template <std::size_t N>
    constexpr _type<N> const& get() const& noexcept {
        if constexpr (N)
            return static_cast<Unv const&>(_second);
        else
            return static_cast<Tnv const&>(_first);
    }

    template <std::size_t N>
    constexpr _type<N>&& get() && noexcept {
        if constexpr (N)
            return static_cast<Unv&&>(_second);
        else
            return static_cast<Tnv&&>(_first);
    }

    template <std::size_t N>
    constexpr _type<N> const&& get() const&& noexcept {
        if constexpr (N)
            return static_cast<const Unv&&>(_second);
        else
            return static_cast<const Tnv&&>(_first);
    }

    bool operator==(tuple const&) const  = default;
    auto operator<=>(tuple const&) const = default;
};

}  // namespace neo::core

template <typename... Ts>
struct std::tuple_size<neo::core::tuple<Ts...>> {
    constexpr static inline std::size_t value = sizeof...(Ts);
};

template <std::size_t N, typename... Ts>
struct std::tuple_element<N, neo::core::tuple<Ts...>>
    : neo::meta::tag<neo::meta::at<neo::core::tuple<Ts...>, N>> {};
