#pragma once

#include <neo/attrib.hpp>
#include <neo/compare.hpp>
#include <neo/get.hpp>
#include <neo/object_t.hpp>
#include <neo/type_traits.hpp>

#include <compare>
#include <tuple>
#include <utility>

namespace neo::core::_tuple_detail {

/// An indexed box to hold tuple elements
template <std::size_t, typename T>
struct nth_box {
    // The value for this element
    NEO_NO_UNIQUE_ADDRESS object_t<T> _obox;

    using lref = add_lvalue_reference_t<nonvoid_t<T>>;
    using cref = add_const_reference_t<nonvoid_t<T>>;

    constexpr nth_box() = default;
    constexpr explicit nth_box(std::in_place_t, auto&& t)
        : _obox(NEO_FWD(t)) {}

    bool operator==(nth_box const&) const  = default;
    auto operator<=>(nth_box const&) const = default;

    constexpr lref get() noexcept { return static_cast<lref>(_obox); }
    constexpr cref get() const noexcept { return static_cast<cref>(_obox); }
};

template <typename Tpl, typename Seq = std::make_index_sequence<meta::len_v<Tpl>>>
struct boxes;

template <std::size_t... Is, neo_ttparam Tpl, typename... Ts>
struct boxes<Tpl<Ts...>, std::index_sequence<Is...>> : nth_box<Is, Ts>... {
    constexpr boxes() = default;

    template <typename... Args>
    constexpr explicit boxes(std::in_place_t tag, Args&&... args)  //
        : nth_box<Is, Ts>(tag, NEO_FWD(args))... {}

    bool operator==(boxes const&) const  = default;
    auto operator<=>(boxes const&) const = default;
};

template <std::size_t... Ns>
constexpr bool equal(const auto& left, const auto& right, std::index_sequence<Ns...>) noexcept {
    return (static_cast<bool>(left.template get<Ns>() == right.template get<Ns>()) and ...);
}

template <typename T, typename U, std::size_t... Ns>
constexpr auto compare_three_way(const T& lhs, const U& rhs, std::index_sequence<Ns...>) noexcept {
    using Category = std::common_comparison_category_t<
        std::compare_three_way_result_t<decltype(lhs.template get<Ns>()),
                                        decltype(rhs.template get<Ns>())>...>;
    Category ret = std::strong_ordering::equal;
    static_cast<void>(
        ((0 != (ret = synth_three_way(lhs.template get<Ns>(), rhs.template get<Ns>()))) or ...
         or true));
    return ret;
}

template <typename L>
    requires requires { std::tuple_size<L>::value; }
using index_sequence_for_tpl = std::make_index_sequence<std::tuple_size_v<L>>;

template <typename Tpl, std::size_t... Ns>
    requires(can_get_nth<Tpl, Ns> and ...)
constexpr void tuple_like_check(Tpl&& tpl, std::index_sequence<Ns...>);

template <typename Tpl>
concept tuple_like = requires(Tpl&& tpl) {
    typename index_sequence_for_tpl<remove_cvref_t<Tpl>>;
    tuple_like_check(static_cast<Tpl&&>(tpl), index_sequence_for_tpl<remove_cvref_t<Tpl>>{});
};

template <typename OtherTpl, typename BaseTpl>
concept matching_tuple_like = tuple_like<OtherTpl>
    and std::tuple_size_v<remove_cvref_t<OtherTpl>> == std::tuple_size_v<BaseTpl>;

template <typename Tpl, typename Argument, std::size_t... Ns>
    requires(
        (std::constructible_from<object_t<std::tuple_element_t<Ns, Tpl>>, get_nth_t<Argument, Ns>>
         and ...))
void check_tuple_constructible(std::index_sequence<Ns...>);

template <typename Tpl, typename Argument>
concept tuple_constructible = matching_tuple_like<Argument, Tpl> and requires(Argument&& from) {
    check_tuple_constructible<Tpl, Argument>(index_sequence_for_tpl<Tpl>{});
};

template <typename FromTpl, typename ToTpl, std::size_t... Ns>
    requires((std::convertible_to<get_nth_t<FromTpl, Ns>, object_t<std::tuple_element_t<Ns, ToTpl>>>
              and ...))
void check_tuple_convertible(std::index_sequence<Ns...>);

template <typename FromTpl, typename ToTpl>
concept tuple_convertible = tuple_constructible<ToTpl, FromTpl>
    and requires { check_tuple_convertible<FromTpl, ToTpl>(index_sequence_for_tpl<ToTpl>{}); };

template <typename ToTpl, typename Argument, std::size_t... Ns>
    requires((nothrow_constructible_from<object_t<std::tuple_element_t<Ns, ToTpl>>,
                                         get_nth_t<Argument, Ns>>
              and ...))
void check_tuple_construct_nothrow(std::index_sequence<Ns...>);

template <typename ToTpl, typename Argument>
concept tuple_construct_nothrow
    = tuple_constructible<ToTpl, Argument> and requires(Argument&& from) {
          check_tuple_construct_nothrow<ToTpl, Argument>(index_sequence_for_tpl<ToTpl>{});
      };

}  // namespace neo::core::_tuple_detail
