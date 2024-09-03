#pragma once

#include "./concepts.hpp"
#include "./declval.hpp"
#include "./type_traits.hpp"

namespace neo {

/**
 * @brief Match a query object
 *
 * @tparam Q A type that can be used as a query
 *
 * The semantics of the given object require that all instances are equivalent
 * and that there exist an `operator()` that returns an appropriate query result.
 */
template <typename Q>
concept query_object = neo::copyable<Q>;

/**
 * @brief Match a query type object that can be executed on the given object
 *
 * @tparam Q A query type to be tested
 * @tparam T The primary argument object to be queried
 * @tparam Args Supplemental arguments to the query call
 */
template <typename Q, typename T, typename... Args>
concept valid_query_for = query_object<Q> and requires(const Q q, const T obj, Args&&... args) {
    { q(obj, static_cast<Args&&>(args)...) } noexcept -> detail::can_reference;
};

/**
 * @brief Obtain the result of executing the given query on an object
 */
template <query_object Q, typename T, typename... Args>
    requires valid_query_for<Q, T, Args...>
using query_t = decltype(NEO_DECLVAL(const Q&)(NEO_DECLVAL(const T&), NEO_DECLVAL(Args&&)...));

/**
 * @brief Implement a query interface that supports a direct query and an indirect query
 *
 * @tparam Derived The derived type, or `void` to deduce the instance type
 *
 * When invoked, attempts to call a `exec()` static method on the derived class that
 * should be constrained appropriately to execute the direct query. If the
 * direct query via `exec()` is not supported, attempts to invoke `obj.query(*this, ...)`
 * on the queried object.
 */
template <typename Derived = void>
class query_interface {
private:
    constexpr const Derived& _self() const noexcept { return static_cast<const Derived&>(*this); }

public:
    template <typename Object, typename... Args>
    constexpr decltype(auto) operator()(const Object& obj, Args&&... args) const noexcept
        requires requires { Derived::exec(obj, NEO_FWD(args)...); }
    {
        return Derived::exec(obj, NEO_FWD(args)...);
    }

    template <typename Object, typename... Args>
    constexpr decltype(auto) operator()(const Object& obj, Args&&... args) const noexcept
        requires requires(const Derived& q) {
            obj.query(q, NEO_FWD(args)...);
            requires not requires { Derived::exec(obj, NEO_FWD(args)...); };
        }
    {
        return obj.query(static_cast<const Derived&>(*this), NEO_FWD(args)...);
    }
};

#ifdef __cpp_explicit_this_parameter
template <>
class query_interface<void> {
public:
    template <typename Self, typename Object, typename... Args>
    constexpr decltype(auto)
    operator()(this Self const&, const Object& obj, Args&&... args) noexcept
        requires requires { Self::exec(obj, NEO_FWD(args)...); }
    {
        return Self::exec(obj, NEO_FWD(args)...);
    }

    template <typename Self, typename Object, typename... Args>
    constexpr decltype(auto)
    operator()(this Self const& self, const Object& obj, Args&&... args) noexcept
        requires requires {
            obj.query(self, NEO_FWD(args)...);
            requires not requires { Self::exec(obj, NEO_FWD(args)...); };
        }
    {
        return obj.query(self, NEO_FWD(args)...);
    }
};
#endif

template <query_object Q, typename T, typename Default>
constexpr decltype(auto) query_or(Q q, const T& obj, Default&& dflt) {
    if constexpr (valid_query_for<Q, T>) {
        return q(obj);
    } else {
        return static_cast<Default>(dflt);
    }
}

}  // namespace neo
