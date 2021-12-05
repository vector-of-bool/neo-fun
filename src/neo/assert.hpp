#pragma once

#include <neo/attrib.hpp>
#include <neo/platform.hpp>
#include <neo/pp.hpp>
#include <neo/repr.hpp>

#include <iterator>
#include <ostream>
#include <string_view>

#if !defined(NEO_ASSERT_THROWS)
#define NEO_ASSERT_THROWS 0
#endif

// A noexcept-expression that evaluates to `true` if throwing-assertions are restricted
#define NEO_ASSERT_IS_NOEXCEPT noexcept(!(NEO_ASSERT_THROWS))
// A noexcept-attribute that is only `noexcept` if the assertion handler does not throw
#define NEO_NOEXCEPT_ASSERTS noexcept(NEO_ASSERT_IS_NOEXCEPT)

#if NEO_COMPILER_IS_MSVC
// MSVC has an __assume built-in
#define NEO_ASSUME_1(...) (__assume(__VA_ARGS__), true)
#elif NEO_COMPILER_IS_CLANG
// Clang defines a similar built-in to the MSVC one
#define NEO_ASSUME_1(...) (__builtin_assume(__VA_ARGS__), true)
#else
// GCC and others do not have an `assume` built-in (AFAIK)
#define NEO_ASSUME_1(...) ((__VA_ARGS__) || (::neo::unreachable(), true))
#endif

/**
 * NEO_ASSUME(<expr>) is an optimization hint that the given expression will hold true at
 * this point. It is unspecified whether the expression is evaluated.
 */
#define NEO_ASSUME(...) NEO_ASSUME_1(__VA_ARGS__)

#if NEO_COMPILER_IS_MSVC || NEO_COMPILER_IS_CLANG
// On _MSC_VER and __clang__, we have an `assume` intrinsic that does not evaluate its argument.
#define NEO_UNEVAL_ASSUME_1(...) (NEO_ASSUME(__VA_ARGS__), true)
#else
// On other compilers, we don't know that NEO_ASSUME will leave its argument unevaluated.
#define NEO_UNEVAL_ASSUME_1(...) (true || NEO_ASSUME(__VA_ARGS__))
#endif

/**
 * NEO_UNEVAL_ASSUME(<expr>) is an optimization hint that the given expression will
 * be true at the point of usage. The expression is guaranteed to never be evaluated at
 * runtime.
 */
#define NEO_UNEVAL_ASSUME(...) NEO_UNEVAL_ASSUME_1(__VA_ARGS__)

namespace neo {

/**
 * Mark the given code path as unreachable. Calling this function, by
 * definition, is undefined behavior. Use this an optimization hint.
 */
[[noreturn]] NEO_ALWAYS_INLINE void unreachable() noexcept {
#if NEO_COMPILER_IS_GNU_LIKE
    __builtin_unreachable();
#elif NEO_COMPILER_IS_MSVC
    __assume(0);
#endif
}

/**
 * An "assume" function. The bool value `b` passed to this function *must* be
 * `true`. If `false` is passed to this function, the behavior of the program
 * is undefined.
 *
 * This function should be used as a hint to the optimizer that the value `b`
 * will always be `true`, and thus the condition from it was generated will
 * also yield a truthy value.
 *
 * It is unspecified whether the condition that generates `b` will be evaluated.
 *
 * NOTE: If the compiler CANNOT PROVE that the expression which generates `b`
 * will return normally and has no side effects, the compiler will be unable to
 * elide evaluation of that expression.
 *
 * In general, an opaque function call or call graph that fails to inline must
 * prevent a compiler from eliding that function call, as it must assume that
 * the expression could `throw`, `std::abort`, or `std::terminate` the program
 * before the result value ever reaches the neo::assume() function.
 *
 * For an assume() that is guaranteed to *never* evaluate its argument, use the
 * NEO_UNEVAL_ASSUME() macro.
 */
NEO_ALWAYS_INLINE void assume(bool b) noexcept {
    if (!b) {
        unreachable();
    }
}

/**
 * The "kind" of an assertion. This is used as inline documentation and tweaks
 * the diagnostic message printed by the assertion handler.
 */
enum class assertion_kind {
    /// An "expectation" or "precondition" of an API. This represents the expectations an API places
    /// on its caller to be well-behaved.
    expects = 1,
    /// An invariant of an API. Regular user action should not cause invariants to break.
    invariant,
    /// A guarantee of the result of an API. This should check that the result of a transition in
    /// program state matches the expectations of the author.
    ensures,
};

/**
 * @brief A source file location
 */
struct source_location {
    std::string_view pretty_func;
    std::string_view filename;
    int              line = 0;
};

/**
 * Class that encapsulates information about an assertion statement
 */
struct assertion_info {
    /// The kind of assertion (expects, invariant, or ensures)
    assertion_kind kind;
    /// The spelling of the condition expression
    std::string_view expression_spelling;
    /// The message associated with the assertion.
    std::string_view message;
    /// The location of the assertion
    source_location loc;

    static assertion_info
    make(assertion_kind k, std::string_view spell, std::string_view msg, source_location loc) {
        return {k, spell, msg, loc};
    }
};

/**
 * Class that encapsulates a captured expression from a failing assertion.
 */
struct assertion_expression {
    /// The source-code spelling of the expression that was captured
    virtual const char* spelling() const noexcept = 0;
    /// Formats the value of the expression into the given output stream
    virtual void write_into(std::ostream& o) const noexcept = 0;
};

namespace detail {

template <typename T>
concept ostream_writable = requires(std::ostream& out, const T& value) {
    out << value;
};

template <typename T>
constexpr void check_one_representable(T&&) noexcept {
    static_assert(reprable<T> || ostream_writable<T>,
                  "All assertion capture expressions should be representable by neo::repr(). "
                  "One or more capture expressions have a type that cannot be repr()'d.");
}

template <typename... Args>
constexpr void check_all_representable(Args&&... a) noexcept {
    (check_one_representable(a), ...);
}

/**
 * Implementaiton of assertion_expression for any particular type
 */
template <typename T>
class assertion_expression_impl : public assertion_expression {
    // The actual literal source-code spelling of the expression
    const char* const _spelling;
    // The value of that expression when the assertion fired.
    const T& _value;

public:
    assertion_expression_impl(const char* s, const T& v)
        : _spelling(s)
        , _value(v) {}

    const char* spelling() const noexcept override { return _spelling; }

    void write_into(std::ostream& out) const noexcept override {
        if constexpr (reprable<T>) {
            out << neo::repr(_value);
        } else if constexpr (ostream_writable<T>) {
            out << _value;
        }
    }
};

template <typename T>
assertion_expression_impl(const char*, const T&) -> assertion_expression_impl<T>;

}  // namespace detail

/**
 * An assertion_expression_list is a simple iterable range-like class that refers to a set of
 * captured expressions for a failing assertion.
 */
class assertion_expression_list {
private:
    using expr_ptrptr = const assertion_expression**;
    expr_ptrptr _begin;
    expr_ptrptr _end;

public:
    assertion_expression_list(expr_ptrptr b, expr_ptrptr e)
        : _begin(b)
        , _end(e) {}

    struct iterator {
        expr_ptrptr ptr;

        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = assertion_expression;
        using pointer           = const value_type*;
        using reference         = const value_type&;

        iterator& operator++() noexcept {
            ++ptr;
            return *this;
        }

        iterator operator++(int) noexcept {
            auto cp = *this;
            ++*this;
            return cp;
        }

        reference operator*() const noexcept { return **ptr; }
        pointer   operator->() const noexcept { return *ptr; }

        bool operator==(iterator o) const noexcept { return ptr == o.ptr; }
        bool operator!=(iterator o) const noexcept { return ptr != o.ptr; }
    };

    iterator begin() const noexcept { return iterator{_begin}; }
    iterator cbegin() const noexcept { return iterator{_begin}; }
    iterator end() const noexcept { return iterator{_end}; }
    iterator cend() const noexcept { return iterator{_end}; }
};

/// Type of function that handles assertion failures.
using assertion_handler_fn_ptr = void (*)(assertion_info, assertion_expression_list);

/**
 * Set the function which handles assertions. The given function pointer may not be nullptr.
 *
 * The assertion handler must not return normally when it is executed. If the
 * assertion handler `throw`s an exception, then the `NEO_ASSERT_THROWS` macro
 * must be defined to `1`, otherwise the `throw` will hit a `noexcept` boundary
 * and terminate the program. Using a throw-ing assertion handler should only
 * be used to test assertions in libraries, and should NEVER be used for
 * application code.
 */
void set_assertion_handler(assertion_handler_fn_ptr) noexcept;

/**
 * Fire an assertion failure. This executes the assertion handler that was set
 * by set_assertion_handler. If not assertion handler was set, a default is
 * used that prints the condition, message, and expressions, and then terminates
 * the program.
 *
 * If NEO_ASSERT_THROWS is set to `1`, then this function *may* throw an
 * exception.
 *
 * This function will never return normally.
 */
[[noreturn]] void fire_assertion(assertion_info,
                                 assertion_expression_list exprs) NEO_NOEXCEPT_ASSERTS;

/**
 * Fire an assertion, with a variadic number of captured expressions.
 * Expressions should be captured inline with the NEO_CAPTURE_EXPR() macro.
 */
template <typename... Ts>
[[noreturn]] void
fire_assertion(assertion_info info,
               detail::assertion_expression_impl<Ts>... exprs) NEO_NOEXCEPT_ASSERTS {
    // Create an array of pointers on the stack, each referring to one of the captured expressions.
    const assertion_expression* expressions[] = {(&exprs)..., nullptr};
    // Fire!
    fire_assertion(info,
                   assertion_expression_list(expressions + 0, expressions + sizeof...(exprs)));
}

/**
 * Check-level assertions are enabled if *either*:
 *   - NEO_ENABLE_CHECKS is defined to a truthy value
 *   - *or* NEO_ENABLE_CHECKS *and* NDEBUG are both not defined
 * This has the effect that neo_assert are both enabled in the same situation as the standard
 * assert() macro.
 *
 * NOTE: When check-assertions are disabled, it is unspecified whether the condition
 * will be evaluated.
 */
#if NEO_ENABLE_CHECKS || (!defined(NEO_ENABLE_CHECKS) && !defined(NDEBUG))
#define NEO_ASSERT_1(...) neo_assert_always(__VA_ARGS__)
#else
#define NEO_ASSERT_1(...) NEO_ASSERT_AS_ASSUMPTION(__VA_ARGS__)
#endif

/**
 * Audit-level assertions are only enabled if they are explicitly requested by
 * NEO_ENABLE_AUDITS.
 */
#if NEO_ENABLE_AUDITS
#define NEO_ASSERT_AUDIT_1(...) neo_assert_audit(__VA_ARGS__)
#else
#define NEO_ASSERT_AUDIT_1(...) NEO_ASSERT_AS_ASSUMPTION(__VA_ARGS__)
#endif

/**
 * Emulates a neo_assert that assumes its conditional expression.
 *
 * Despite never actually firing an assertion, this macro expands the full body *as if* it could
 * fire the assertion. This ensures that the arguments to the macro are still semantically valid,
 * and prevents unused-variable warnings that would be seen if the macro simply discarded its
 * arguments (as you see with <cassert>).
 *
 * The expression is guaranteed to never be evaluated at runtime.
 */
#define NEO_ASSERT_AS_ASSUMPTION(kind, expr, msg, ...)                                             \
    NEO_FN_MACRO_BEGIN                                                                             \
    const bool _neo_expr_cond_ = NEO_UNEVAL_ASSUME(expr);                                          \
    if (!(_neo_expr_cond_)) {                                                                      \
        ::neo::detail::check_all_representable(__VA_ARGS__);                                       \
        neo::fire_assertion(::neo::assertion_info::make(::neo::assertion_kind::kind,               \
                                                        NEO_STR(expr),                             \
                                                        (msg),                                     \
                                                        NEO_SOURCE_LOCATION())                     \
                                NEO_MAP(NEO_REPR_ASSERT_EXPR, ~, __VA_ARGS__));                    \
    }                                                                                              \
    NEO_FN_MACRO_END

#define NEO_CAPTURE_EXPR(Expression)                                                               \
    ::neo::detail::assertion_expression_impl { NEO_STR(Expression), (Expression) }

#define NEO_REPR_ASSERT_EXPR(_dummy, _counter, Expression) , NEO_CAPTURE_EXPR(Expression)

// neo_assert macros require a conforming C++20 preprocessor. MSVC has support thereof toggled by a
// compile flag. Check that we have it ready.
#if _MSVC_TRADITIONAL
#pragma message(                                                                                   \
    "warning: NOTE from neo-fun: Using the <neo/assert.hpp> macros with MSVC "                     \
    "requires a C++20 conformant preprocessor. Compile with "                                      \
    "/experimental:preprocessor")
#endif

/**
 * Define an assertion that is unconditionally checked. This should be used for
 * safety-critical situations and situations where performance is not important.
 */
#define neo_assert_always(kind, expr, msg, ...)                                                    \
    NEO_FN_MACRO_BEGIN                                                                             \
    const bool _neo_expr_cond_ = (expr);                                                           \
    if (!(_neo_expr_cond_)) {                                                                      \
        ::neo::detail::check_all_representable(__VA_ARGS__);                                       \
        ::neo::fire_assertion(::neo::assertion_info::make(::neo::assertion_kind::kind,             \
                                                          NEO_STR(expr),                           \
                                                          (msg),                                   \
                                                          NEO_SOURCE_LOCATION())                   \
                                  NEO_MAP(NEO_REPR_ASSERT_EXPR, ~, __VA_ARGS__));                  \
    }                                                                                              \
    NEO_FN_MACRO_END

#define NEO_SOURCE_LOCATION()                                                                      \
    ::neo::source_location { NEO_PRETTY_FUNC, __FILE__, __LINE__ }

/**
 * Define an assertion that is never checked, but still acts as an optimization
 * hint. The expression given to the assertion is never evaluated.
 *
 * The `msg` and any captured expressions are never evaluated either, but they
 * are still provided for interface uniformity.
 *
 * Even though unevaluated, all arguments must be semantically valid.
 */
#define neo_assert_assume(expr) NEO_ASSERT_AS_ASSUMPTION(invariant, (expr), "")

/**
 * neo_assert is the traditional assertion that should be used for regular debugging. It is disabled
 * by NDEBUG, and can be explicitly enabled with NEO_ENABLE_CHECKS.
 */
#define neo_assert NEO_ASSERT_1

/**
 * neo_assert_audit is for checks that are expensive but can offer useful insight to buggy
 * behavior in a pinch. These must be explicitly enabled via NEO_ENABLE_AUDITS.
 */
#define neo_assert_audit NEO_ASSERT_AUDIT_1

namespace detail {

class breadcrumb_base {
    std::string_view       _msg;
    source_location        _loc;
    const breadcrumb_base* _previous;

    static inline thread_local const breadcrumb_base* _top = nullptr;

    virtual void do_render(std::ostream&) const noexcept = 0;

    void _render(std::ostream& out, assertion_expression_list) const noexcept;

public:
    explicit breadcrumb_base(std::string_view msg, source_location loc) noexcept
        : _msg(msg)
        , _loc(loc)
        , _previous(std::exchange(_top, this)) {}

    ~breadcrumb_base() { _top = _previous; }

    template <typename... Ts>
    void render_into(std::ostream& out, int, const Ts&... exprs) const noexcept {
        const ::neo::assertion_expression* expressions[] = {(&exprs)..., nullptr};
        _render(out, assertion_expression_list(expressions + 0, expressions + sizeof...(exprs)));
    }

    static void render_all(std::ostream& out) noexcept;
};

template <typename Fn>
struct breadcrumb_impl : breadcrumb_base {
    Fn fn;

    breadcrumb_impl(std::string_view msg, source_location loc, Fn f)
        : breadcrumb_base(msg, loc)
        , fn(f) {}

    void do_render(std::ostream& out) const noexcept override { fn(*this, out); }
};

template <typename F>
breadcrumb_impl(std::string_view, source_location, F) -> breadcrumb_impl<F>;

}  // namespace detail

/**
 * @brief Render the current thread's breadcrumbs to the given output stream.
 */
void render_breadcrumbs(std::ostream& out) noexcept;

/**
 * @brief Declare a set of breadcrumbs for the current thread in the current scope.
 *
 * If an assertion fires in the current thread at any lower scope, the breadcrumb expressions passed
 * to this macro will be printed as part of the diagnostic output for the assertion. When the
 * current scope ends, the breadcrumbs in this message will be popped from the stack.
 */
#define neo_assertion_breadcrumbs(Message, ...)                                                    \
    ::neo::detail::breadcrumb_impl                                                                 \
        NEO_CONCAT_3(_neo_breadcrum_,                                                              \
                     __LINE__,                                                                     \
                     _scope)(Message,                                                              \
                             NEO_SOURCE_LOCATION(),                                                \
                             [&](const ::neo::detail::breadcrumb_base& self,                       \
                                 std::ostream&                         out) noexcept {                                     \
                                 self.render_into(out,                                             \
                                                  0 NEO_MAP(NEO_REPR_ASSERT_EXPR,                  \
                                                            ~,                                     \
                                                            __VA_ARGS__));                         \
                             })

}  // namespace neo
