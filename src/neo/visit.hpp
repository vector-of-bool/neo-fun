#pragma once

#include "./concepts.hpp"
#include "./fwd.hpp"
#include "./get.hpp"
#include "./invoke.hpp"
#include "./meta.hpp"
#include "./optional.hpp"

namespace neo {

template <typename Var>
concept visitable = requires(Var&& var) {
    { var.index() } noexcept -> std::integral;
    meta::len_v<std::remove_cvref_t<Var>>;
    try_get_nth<0>(NEO_FWD(var));
};

namespace visit_detail {

template <typename Var,
          typename Fn,
          typename ISeq = std::make_index_sequence<meta::len_v<std::remove_reference_t<Var>>>>
struct impl;

template <typename Variant, std::size_t N, typename Func, typename Return>
struct one_visitor {
    static constexpr Return
    apply(Variant&& var,
          Func&&    fn) noexcept(noexcept(NEO_INVOKE(NEO_FWD(fn), get_nth<N>(NEO_FWD(var))))) {
        return NEO_INVOKE(NEO_FWD(fn), get_nth<N>(NEO_FWD(var)));
    }
};

template <typename Var, typename Fn, auto... Ns>
struct impl<Var, Fn, std::index_sequence<Ns...>> {
    // Use common_reference to find the common return type of all the visit overloads
    using result_type = std::common_reference_t<invoke_result_t<Fn, get_nth_t<Var, Ns>>...>;

    // Apply the visitor to the variant.
    constexpr static result_type invoke(Var&& v, Fn&& fn) {
        const auto idx = static_cast<std::size_t>(v.index());
        // We do different things depending on the result type, just for optimization
        if constexpr (void_type<result_type>) {
            // The result type is void, so we don't need to record the return value. Just
            // invoke the appropriate overload by using pack expansion:
            static_cast<void>(
                ((idx == Ns
                  and (static_cast<void>(NEO_INVOKE(NEO_FWD(fn), get_nth<Ns>(NEO_FWD(v)))), true))
                 or ...));
        } else if constexpr (move_constructible<result_type>) {
            // The result type is move-constructible, so do the invoke by emplacing the
            // return value in an optional and then returning that value
            optional<result_type> r;
            static_cast<void>(
                ((idx == Ns and (r.emplace(NEO_INVOKE(NEO_FWD(fn), get_nth<Ns>(NEO_FWD(v)))), true))
                 or ...));
            result_type ret = *NEO_MOVE(r);
            return ret;
        } else {
            // The result type is not move constructible, so we can't do a pack expanion trick.
            // Instead, create a unique visit trampoline function in an array of function pointers
            // and invoke the correct one based on the variant's current index:
            constexpr result_type (*functions[])(Var&& variant, Fn&& fn)
                = {one_visitor<Var, Ns, Fn, result_type>::apply...};
            auto chosen = functions[idx];
            return chosen(NEO_FWD(v), NEO_FWD(fn));
        }
    }
};

}  // namespace visit_detail

/**
 * @brief Yield the result type of applying the given visitor to the given variant-like operand
 *
 * @tparam Var A variant-like type
 * @tparam Fn A visitor function for the variant.
 *
 * If the visitor is not valid for the variant, no type is defined
 */
template <visitable Var, typename Fn>
using visit_t = typename visit_detail::impl<Var, Fn>::result_type;

/**
 * @brief Match a visitor invocable to a variant to be visited
 *
 * @tparam Fn An invocable object, the prospective visitor
 * @tparam Var The variant to be visited
 */
template <typename Fn, typename Var>
concept visitor_of = visitable<Var> and requires { typename visit_t<Var, Fn>; };

template <visitable Var, visitor_of<Var> Fn>
constexpr visit_t<Var, Fn> visit(Var&& var, Fn&& fn) {
    using impl = visit_detail::impl<Var, Fn>;
    return impl::invoke(NEO_FWD(var), NEO_FWD(fn));
}

}  // namespace neo
