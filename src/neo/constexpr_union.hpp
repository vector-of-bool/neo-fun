#pragma once

// clang-format off

// d8888b.  .d88b.       d8b   db  .d88b.  d888888b      d88888b d8888b. d888888b d888888b
// 88  `8D .8P  Y8.      888o  88 .8P  Y8. `~~88~~'      88'     88  `8D   `88'   `~~88~~'
// 88   88 88    88      88V8o 88 88    88    88         88ooooo 88   88    88       88
// 88   88 88    88      88 V8o88 88    88    88         88~~~~~ 88   88    88       88
// 88  .8D `8b  d8'      88  V888 `8b  d8'    88         88.     88  .8D   .88.      88
// Y8888D'  `Y88P'       VP   V8P  `Y88P'     YP         Y88888P Y8888D' Y888888P    YP

// This file was GENERATED from src/neo/constexpr_union.hpp.jinja

// This file is GENERATED! Edit constexpr_union.hpp.jinja instead.

#include <neo/unit.hpp>
#include <neo/attrib.hpp>
#include <neo/addressof.hpp>
#include <neo/meta.hpp>
#include <neo/type_traits.hpp>
#include <neo/fwd.hpp>

#include <memory>  // std::construct_at

namespace neo {

/**
 * @brief A variadic union template that is default-constructible and `constexpr`-ready.
 *
 * Defines three member function templates:
 *
 * - `get<N>() -> nth_type<N> [const] [&|&&]` obtain a reference to the `Nth` union member.
 * - `construct<N>(auto&&...) -> nth_type<N>&` activate the `Nth` union member by calling
 *   the constructor with the given arguments. No other union members are modified.
 * - `destroy<N>() -> void` - Destroy the `Nth` union member by calling its destructor.
 *
 * Every specialization of `constexpr_union` constains a `nil` alternative which is
 * activated upon default-construction of the union. The names of the variadic members are
 * the zero-based index of the member with an underscore prefix (e.g. `_0`, `_1`, `_2`, `_3`).
 * To access the `Nth` member generically, use the `get<N>()` member function template, which
 * returns a reference to that member with the cvref-qualifiers of the instance used to call the
 * `get` member. To get the `Nth` type of the union, use the `nth_type<N>` member alias
 * template.
 *
 * The union is not a "`variant`"": it is up to the user to track the active member and
 * to correctly call the constructors/destructors when needed. The sole purpose of
 * this class template is to simplify the implementation of `variant`/`optional`-style
 * class templates.
 *
 * Because of the implicit `nil` member, a `constexpr_union<T>` can be used as storage
 * for an optional `T`, with the `nil` member acting as a the "null" state.
 *
 * All union members are marked with `[[no_uninque_address]]`.
 *
 * @tparam Ts... The alternative of the union. Must not be empty.
 */
template <typename... Ts>
union constexpr_union;

namespace detail {

template <typename... Ts>
concept all_trivially_destructible =
    (neo_is_trivially_destructible(Ts) and ... and true);

template <std::size_t N, typename Ret, typename Onion>
NEO_ALWAYS_INLINE constexpr Ret get_member(Onion&& on) noexcept {
    if constexpr (N == 0) return static_cast<Ret>(on._0);
    if constexpr (N == 1) return static_cast<Ret>(on._1);
    if constexpr (N == 2) return static_cast<Ret>(on._2);
    if constexpr (N == 3) return static_cast<Ret>(on._3);
    if constexpr (N == 4) return static_cast<Ret>(on._4);
    if constexpr (N == 5) return static_cast<Ret>(on._5);
    if constexpr (N == 6) return static_cast<Ret>(on._6);
    if constexpr (N == 7) return static_cast<Ret>(on._7);
    if constexpr (N == 8) return static_cast<Ret>(on._8);
    if constexpr (N == 9) return static_cast<Ret>(on._9);
    if constexpr (N == 10) return static_cast<Ret>(on._10);
    if constexpr (N == 11) return static_cast<Ret>(on._11);
    if constexpr (N == 12) return static_cast<Ret>(on._12);
    if constexpr (N == 13) return static_cast<Ret>(on._13);
    if constexpr (N == 14) return static_cast<Ret>(on._14);
    if constexpr (N == 15) return static_cast<Ret>(on._15);
    if constexpr (N == 16) return static_cast<Ret>(on._16);
    if constexpr (N == 17) return static_cast<Ret>(on._17);
    if constexpr (N == 18) return static_cast<Ret>(on._18);
    if constexpr (N == 19) return static_cast<Ret>(on._19);
    if constexpr (N == 20) return static_cast<Ret>(on._20);
    if constexpr (N == 21) return static_cast<Ret>(on._21);
    if constexpr (N == 22) return static_cast<Ret>(on._22);
    if constexpr (N == 23) return static_cast<Ret>(on._23);
    if constexpr (N == 24) return static_cast<Ret>(on._24);
    if constexpr (N == 25) return static_cast<Ret>(on._25);
    if constexpr (N == 26) return static_cast<Ret>(on._26);
    if constexpr (N == 27) return static_cast<Ret>(on._27);
    if constexpr (N == 28) return static_cast<Ret>(on._28);
    if constexpr (N == 29) return static_cast<Ret>(on._29);
    if constexpr (N == 30) return static_cast<Ret>(on._30);
    if constexpr (N == 31) return static_cast<Ret>(on._31);
    if constexpr (N == 32) return static_cast<Ret>(on._32);
    if constexpr (N == 33) return static_cast<Ret>(on._33);
    if constexpr (N == 34) return static_cast<Ret>(on._34);
    if constexpr (N == 35) return static_cast<Ret>(on._35);
    if constexpr (N == 36) return static_cast<Ret>(on._36);
    if constexpr (N == 37) return static_cast<Ret>(on._37);
    if constexpr (N == 38) return static_cast<Ret>(on._38);
    if constexpr (N == 39) return static_cast<Ret>(on._39);
    if constexpr (N == 40) return static_cast<Ret>(on._40);
    if constexpr (N == 41) return static_cast<Ret>(on._41);
    if constexpr (N == 42) return static_cast<Ret>(on._42);
    if constexpr (N == 43) return static_cast<Ret>(on._43);
    if constexpr (N == 44) return static_cast<Ret>(on._44);
    if constexpr (N == 45) return static_cast<Ret>(on._45);
    if constexpr (N == 46) return static_cast<Ret>(on._46);
    if constexpr (N == 47) return static_cast<Ret>(on._47);
    if constexpr (N == 48) return static_cast<Ret>(on._48);
    if constexpr (N == 49) return static_cast<Ret>(on._49);
    if constexpr (N == 50) return static_cast<Ret>(on._50);
    if constexpr (N == 51) return static_cast<Ret>(on._51);
    if constexpr (N == 52) return static_cast<Ret>(on._52);
    if constexpr (N == 53) return static_cast<Ret>(on._53);
    if constexpr (N == 54) return static_cast<Ret>(on._54);
    if constexpr (N == 55) return static_cast<Ret>(on._55);
    if constexpr (N == 56) return static_cast<Ret>(on._56);
    if constexpr (N == 57) return static_cast<Ret>(on._57);
    if constexpr (N == 58) return static_cast<Ret>(on._58);
    if constexpr (N == 59) return static_cast<Ret>(on._59);
    if constexpr (N == 60) return static_cast<Ret>(on._60);
    if constexpr (N == 61) return static_cast<Ret>(on._61);
    if constexpr (N == 62) return static_cast<Ret>(on._62);
    if constexpr (N == 63) return static_cast<Ret>(on._63);
    if constexpr (N == 64) return static_cast<Ret>(on._64);
    if constexpr (N == 65) return static_cast<Ret>(on._65);
    if constexpr (N == 66) return static_cast<Ret>(on._66);
    if constexpr (N == 67) return static_cast<Ret>(on._67);
    if constexpr (N == 68) return static_cast<Ret>(on._68);
    if constexpr (N == 69) return static_cast<Ret>(on._69);
    if constexpr (N == 70) return static_cast<Ret>(on._70);
    if constexpr (N == 71) return static_cast<Ret>(on._71);
    if constexpr (N == 72) return static_cast<Ret>(on._72);
    if constexpr (N == 73) return static_cast<Ret>(on._73);
    if constexpr (N == 74) return static_cast<Ret>(on._74);
    if constexpr (N == 75) return static_cast<Ret>(on._75);
    if constexpr (N == 76) return static_cast<Ret>(on._76);
    if constexpr (N == 77) return static_cast<Ret>(on._77);
    if constexpr (N == 78) return static_cast<Ret>(on._78);
    if constexpr (N == 79) return static_cast<Ret>(on._79);
    if constexpr (N == 80) return static_cast<Ret>(on._80);
    if constexpr (N == 81) return static_cast<Ret>(on._81);
    if constexpr (N == 82) return static_cast<Ret>(on._82);
    if constexpr (N == 83) return static_cast<Ret>(on._83);
    if constexpr (N == 84) return static_cast<Ret>(on._84);
    if constexpr (N == 85) return static_cast<Ret>(on._85);
    if constexpr (N == 86) return static_cast<Ret>(on._86);
    if constexpr (N == 87) return static_cast<Ret>(on._87);
    if constexpr (N == 88) return static_cast<Ret>(on._88);
    if constexpr (N == 89) return static_cast<Ret>(on._89);
    if constexpr (N == 90) return static_cast<Ret>(on._90);
    if constexpr (N == 91) return static_cast<Ret>(on._91);
    if constexpr (N == 92) return static_cast<Ret>(on._92);
    if constexpr (N == 93) return static_cast<Ret>(on._93);
    if constexpr (N == 94) return static_cast<Ret>(on._94);
    if constexpr (N == 95) return static_cast<Ret>(on._95);
    if constexpr (N == 96) return static_cast<Ret>(on._96);
    if constexpr (N == 97) return static_cast<Ret>(on._97);
    if constexpr (N == 98) return static_cast<Ret>(on._98);
    if constexpr (N == 99) return static_cast<Ret>(on._99);
}

}

template <>
union constexpr_union<> {
    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_NO_UNIQUE_ADDRESS unit nil;
};

template <typename T0>
union constexpr_union<T0> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
};

template <typename T0, typename T1>
union constexpr_union<T0, T1> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
};

template <typename T0, typename T1, typename T2>
union constexpr_union<T0, T1, T2> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
};

template <typename T0, typename T1, typename T2, typename T3>
union constexpr_union<T0, T1, T2, T3> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4>
union constexpr_union<T0, T1, T2, T3, T4> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
union constexpr_union<T0, T1, T2, T3, T4, T5> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
        else if constexpr (N == 48)
            return (_48);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
        else if constexpr (N == 48)
            return (_48);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
        else if constexpr (N == 48)
            return (_48);
        else if constexpr (N == 49)
            return (_49);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        if constexpr (false) {}
        else if constexpr (N == 0)
            return (_0);
        else if constexpr (N == 1)
            return (_1);
        else if constexpr (N == 2)
            return (_2);
        else if constexpr (N == 3)
            return (_3);
        else if constexpr (N == 4)
            return (_4);
        else if constexpr (N == 5)
            return (_5);
        else if constexpr (N == 6)
            return (_6);
        else if constexpr (N == 7)
            return (_7);
        else if constexpr (N == 8)
            return (_8);
        else if constexpr (N == 9)
            return (_9);
        else if constexpr (N == 10)
            return (_10);
        else if constexpr (N == 11)
            return (_11);
        else if constexpr (N == 12)
            return (_12);
        else if constexpr (N == 13)
            return (_13);
        else if constexpr (N == 14)
            return (_14);
        else if constexpr (N == 15)
            return (_15);
        else if constexpr (N == 16)
            return (_16);
        else if constexpr (N == 17)
            return (_17);
        else if constexpr (N == 18)
            return (_18);
        else if constexpr (N == 19)
            return (_19);
        else if constexpr (N == 20)
            return (_20);
        else if constexpr (N == 21)
            return (_21);
        else if constexpr (N == 22)
            return (_22);
        else if constexpr (N == 23)
            return (_23);
        else if constexpr (N == 24)
            return (_24);
        else if constexpr (N == 25)
            return (_25);
        else if constexpr (N == 26)
            return (_26);
        else if constexpr (N == 27)
            return (_27);
        else if constexpr (N == 28)
            return (_28);
        else if constexpr (N == 29)
            return (_29);
        else if constexpr (N == 30)
            return (_30);
        else if constexpr (N == 31)
            return (_31);
        else if constexpr (N == 32)
            return (_32);
        else if constexpr (N == 33)
            return (_33);
        else if constexpr (N == 34)
            return (_34);
        else if constexpr (N == 35)
            return (_35);
        else if constexpr (N == 36)
            return (_36);
        else if constexpr (N == 37)
            return (_37);
        else if constexpr (N == 38)
            return (_38);
        else if constexpr (N == 39)
            return (_39);
        else if constexpr (N == 40)
            return (_40);
        else if constexpr (N == 41)
            return (_41);
        else if constexpr (N == 42)
            return (_42);
        else if constexpr (N == 43)
            return (_43);
        else if constexpr (N == 44)
            return (_44);
        else if constexpr (N == 45)
            return (_45);
        else if constexpr (N == 46)
            return (_46);
        else if constexpr (N == 47)
            return (_47);
        else if constexpr (N == 48)
            return (_48);
        else if constexpr (N == 49)
            return (_49);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94, typename T95>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
    NEO_NO_UNIQUE_ADDRESS T95 _95;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94, typename T95, typename T96>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
    NEO_NO_UNIQUE_ADDRESS T95 _95;
    NEO_NO_UNIQUE_ADDRESS T96 _96;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94, typename T95, typename T96, typename T97>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
    NEO_NO_UNIQUE_ADDRESS T95 _95;
    NEO_NO_UNIQUE_ADDRESS T96 _96;
    NEO_NO_UNIQUE_ADDRESS T97 _97;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94, typename T95, typename T96, typename T97, typename T98>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
    NEO_NO_UNIQUE_ADDRESS T95 _95;
    NEO_NO_UNIQUE_ADDRESS T96 _96;
    NEO_NO_UNIQUE_ADDRESS T97 _97;
    NEO_NO_UNIQUE_ADDRESS T98 _98;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42, typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51, typename T52, typename T53, typename T54, typename T55, typename T56, typename T57, typename T58, typename T59, typename T60, typename T61, typename T62, typename T63, typename T64, typename T65, typename T66, typename T67, typename T68, typename T69, typename T70, typename T71, typename T72, typename T73, typename T74, typename T75, typename T76, typename T77, typename T78, typename T79, typename T80, typename T81, typename T82, typename T83, typename T84, typename T85, typename T86, typename T87, typename T88, typename T89, typename T90, typename T91, typename T92, typename T93, typename T94, typename T95, typename T96, typename T97, typename T98, typename T99>
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99> {
    template <std::size_t N>
    using nth_type = meta::pack_at<N, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99>;

    NEO_ALWAYS_INLINE constexpr constexpr_union() noexcept
        : nil()
    {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires (not detail::all_trivially_destructible<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99>)
    {}

    template <std::size_t N, typename... Args>
    NEO_ALWAYS_INLINE constexpr nth_type<N>& construct(Args&&... args)
        noexcept(noexcept(nth_type<N>(static_cast<Args&&>(args)...)))
    {
        return *std::construct_at(NEO_ADDRESSOF(get<N>()), static_cast<Args&&>(args)...);
    }

    template <std::size_t N, typename T = nth_type<N>>
    NEO_ALWAYS_INLINE constexpr void destroy() noexcept {
        std::destroy_at(NEO_ADDRESSOF(get<N>()));
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>& get() & noexcept {
        return detail::get_member<N, nth_type<N>&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const& get() const & noexcept {
        return detail::get_member<N, nth_type<N> const&>(*this);
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N>&& get() && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    template <std::size_t N>
    [[nodiscard]] NEO_ALWAYS_INLINE constexpr nth_type<N> const&& get() const && noexcept {
        return NEO_MOVE(this->get<N>());
    }

    NEO_NO_UNIQUE_ADDRESS unit nil;

    constexpr_union(constexpr_union&&) = default;
    constexpr_union(constexpr_union const&) = default;
    constexpr_union& operator=(constexpr_union&&) = default;
    constexpr_union& operator=(constexpr_union const&) = default;
    
    NEO_NO_UNIQUE_ADDRESS T0 _0;
    NEO_NO_UNIQUE_ADDRESS T1 _1;
    NEO_NO_UNIQUE_ADDRESS T2 _2;
    NEO_NO_UNIQUE_ADDRESS T3 _3;
    NEO_NO_UNIQUE_ADDRESS T4 _4;
    NEO_NO_UNIQUE_ADDRESS T5 _5;
    NEO_NO_UNIQUE_ADDRESS T6 _6;
    NEO_NO_UNIQUE_ADDRESS T7 _7;
    NEO_NO_UNIQUE_ADDRESS T8 _8;
    NEO_NO_UNIQUE_ADDRESS T9 _9;
    NEO_NO_UNIQUE_ADDRESS T10 _10;
    NEO_NO_UNIQUE_ADDRESS T11 _11;
    NEO_NO_UNIQUE_ADDRESS T12 _12;
    NEO_NO_UNIQUE_ADDRESS T13 _13;
    NEO_NO_UNIQUE_ADDRESS T14 _14;
    NEO_NO_UNIQUE_ADDRESS T15 _15;
    NEO_NO_UNIQUE_ADDRESS T16 _16;
    NEO_NO_UNIQUE_ADDRESS T17 _17;
    NEO_NO_UNIQUE_ADDRESS T18 _18;
    NEO_NO_UNIQUE_ADDRESS T19 _19;
    NEO_NO_UNIQUE_ADDRESS T20 _20;
    NEO_NO_UNIQUE_ADDRESS T21 _21;
    NEO_NO_UNIQUE_ADDRESS T22 _22;
    NEO_NO_UNIQUE_ADDRESS T23 _23;
    NEO_NO_UNIQUE_ADDRESS T24 _24;
    NEO_NO_UNIQUE_ADDRESS T25 _25;
    NEO_NO_UNIQUE_ADDRESS T26 _26;
    NEO_NO_UNIQUE_ADDRESS T27 _27;
    NEO_NO_UNIQUE_ADDRESS T28 _28;
    NEO_NO_UNIQUE_ADDRESS T29 _29;
    NEO_NO_UNIQUE_ADDRESS T30 _30;
    NEO_NO_UNIQUE_ADDRESS T31 _31;
    NEO_NO_UNIQUE_ADDRESS T32 _32;
    NEO_NO_UNIQUE_ADDRESS T33 _33;
    NEO_NO_UNIQUE_ADDRESS T34 _34;
    NEO_NO_UNIQUE_ADDRESS T35 _35;
    NEO_NO_UNIQUE_ADDRESS T36 _36;
    NEO_NO_UNIQUE_ADDRESS T37 _37;
    NEO_NO_UNIQUE_ADDRESS T38 _38;
    NEO_NO_UNIQUE_ADDRESS T39 _39;
    NEO_NO_UNIQUE_ADDRESS T40 _40;
    NEO_NO_UNIQUE_ADDRESS T41 _41;
    NEO_NO_UNIQUE_ADDRESS T42 _42;
    NEO_NO_UNIQUE_ADDRESS T43 _43;
    NEO_NO_UNIQUE_ADDRESS T44 _44;
    NEO_NO_UNIQUE_ADDRESS T45 _45;
    NEO_NO_UNIQUE_ADDRESS T46 _46;
    NEO_NO_UNIQUE_ADDRESS T47 _47;
    NEO_NO_UNIQUE_ADDRESS T48 _48;
    NEO_NO_UNIQUE_ADDRESS T49 _49;
    NEO_NO_UNIQUE_ADDRESS T50 _50;
    NEO_NO_UNIQUE_ADDRESS T51 _51;
    NEO_NO_UNIQUE_ADDRESS T52 _52;
    NEO_NO_UNIQUE_ADDRESS T53 _53;
    NEO_NO_UNIQUE_ADDRESS T54 _54;
    NEO_NO_UNIQUE_ADDRESS T55 _55;
    NEO_NO_UNIQUE_ADDRESS T56 _56;
    NEO_NO_UNIQUE_ADDRESS T57 _57;
    NEO_NO_UNIQUE_ADDRESS T58 _58;
    NEO_NO_UNIQUE_ADDRESS T59 _59;
    NEO_NO_UNIQUE_ADDRESS T60 _60;
    NEO_NO_UNIQUE_ADDRESS T61 _61;
    NEO_NO_UNIQUE_ADDRESS T62 _62;
    NEO_NO_UNIQUE_ADDRESS T63 _63;
    NEO_NO_UNIQUE_ADDRESS T64 _64;
    NEO_NO_UNIQUE_ADDRESS T65 _65;
    NEO_NO_UNIQUE_ADDRESS T66 _66;
    NEO_NO_UNIQUE_ADDRESS T67 _67;
    NEO_NO_UNIQUE_ADDRESS T68 _68;
    NEO_NO_UNIQUE_ADDRESS T69 _69;
    NEO_NO_UNIQUE_ADDRESS T70 _70;
    NEO_NO_UNIQUE_ADDRESS T71 _71;
    NEO_NO_UNIQUE_ADDRESS T72 _72;
    NEO_NO_UNIQUE_ADDRESS T73 _73;
    NEO_NO_UNIQUE_ADDRESS T74 _74;
    NEO_NO_UNIQUE_ADDRESS T75 _75;
    NEO_NO_UNIQUE_ADDRESS T76 _76;
    NEO_NO_UNIQUE_ADDRESS T77 _77;
    NEO_NO_UNIQUE_ADDRESS T78 _78;
    NEO_NO_UNIQUE_ADDRESS T79 _79;
    NEO_NO_UNIQUE_ADDRESS T80 _80;
    NEO_NO_UNIQUE_ADDRESS T81 _81;
    NEO_NO_UNIQUE_ADDRESS T82 _82;
    NEO_NO_UNIQUE_ADDRESS T83 _83;
    NEO_NO_UNIQUE_ADDRESS T84 _84;
    NEO_NO_UNIQUE_ADDRESS T85 _85;
    NEO_NO_UNIQUE_ADDRESS T86 _86;
    NEO_NO_UNIQUE_ADDRESS T87 _87;
    NEO_NO_UNIQUE_ADDRESS T88 _88;
    NEO_NO_UNIQUE_ADDRESS T89 _89;
    NEO_NO_UNIQUE_ADDRESS T90 _90;
    NEO_NO_UNIQUE_ADDRESS T91 _91;
    NEO_NO_UNIQUE_ADDRESS T92 _92;
    NEO_NO_UNIQUE_ADDRESS T93 _93;
    NEO_NO_UNIQUE_ADDRESS T94 _94;
    NEO_NO_UNIQUE_ADDRESS T95 _95;
    NEO_NO_UNIQUE_ADDRESS T96 _96;
    NEO_NO_UNIQUE_ADDRESS T97 _97;
    NEO_NO_UNIQUE_ADDRESS T98 _98;
    NEO_NO_UNIQUE_ADDRESS T99 _99;
};

#if 0
// In principal, arbitrarily many types could be supported, but some compilers consider it UB to
// access the tail sub-union if the head sub-union has an active member. GCC accepts, but Clang
// rejects. This might not have a pretty solution.

// Specialization for at least ten types, which subdivides the arguments into
// smaller sub-unions.
template <typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename... More>
    requires(sizeof...(More) != 0)
union constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, More...> {
    using FirstNext = constexpr_union<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>;
    using Tail = constexpr_union<More...>;

    constexpr constexpr_union(constexpr_union&&)                 = default;
    constexpr constexpr_union(constexpr_union const&)            = default;
    constexpr constexpr_union& operator=(constexpr_union&&)      = default;
    constexpr constexpr_union& operator=(constexpr_union const&) = default;

    constexpr constexpr_union() noexcept
        : _first_ten() {}

    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union() = default;
    NEO_CONSTEXPR_DESTRUCTOR ~constexpr_union()
        requires(not neo_is_trivially_destructible(FirstNext)
              or not neo_is_trivially_destructible(Tail))
    {}

    template <std::size_t N>
    using nth_type = meta::at<constexpr_union, N>;

    template <std::size_t N>
    constexpr nth_type<N>& get() & noexcept {
        if constexpr (N < 10) {
            return _first_ten.template get<N>();
        } else {
            return _tail.template get<N - 10>();
        }
    }

    template <std::size_t N>
    constexpr nth_type<N> const& get() const& noexcept {
        if constexpr (N < 10) {
            return _first_ten.template get<N>();
        } else {
            return _tail.template get<N - 10>();
        }
    }

    template <std::size_t N>
    constexpr nth_type<N>&& get() && noexcept {
        if constexpr (N < 10) {
            return _first_ten.template get<N>();
        } else {
            return _tail.template get<N - 10>();
        }
    }

    template <std::size_t N>
    constexpr nth_type<N> const&& get() const&& noexcept {
        if constexpr (N < 10) {
            return _first_ten.template get<N>();
        } else {
            return _tail.template get<N - 10>();
        }
    }

    template <std::size_t N, typename... Args>
    constexpr nth_type<N>&
    construct(Args&&... args) noexcept(noexcept(nth_type<N>(static_cast<Args&&...>(args)...))) {
        if constexpr (N < 10) {
            return _first_ten.template construct<N>(static_cast<Args&&...>(args)...);
        } else {
            return _tail.template construct<N - 10>(static_cast<Args&&...>(args)...);
        }
    }

    template <std::size_t N>
        requires requires { typename nth_type<N>; }
    constexpr void destroy() noexcept {
        if constexpr (N < 10) {
            _first_ten.template destroy<N>();
        } else {
            _tail.template destroy<N-10>();
        }
    }

    NEO_NO_UNIQUE_ADDRESS FirstNext _first_ten;
    NEO_NO_UNIQUE_ADDRESS Tail _tail;
};

#endif // disabled

}  // namespace neo
