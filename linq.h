

#ifndef INCLUDE_GUARD_LINQ_H
#define INCLUDE_GUARD_LINQ_H

#include <utility>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

//
//
// Some preprocessor utilites
//
//
#define LINQ_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

//
// LINQ_IS_PAREN is used to detect if the first token is a parenthesis.
// It expands to 1 if it is, otherwise it expands to 0.
//
#define LINQ_IS_PAREN(x) LINQ_IS_PAREN_CHECK(LINQ_IS_PAREN_PROBE x)
#define LINQ_IS_PAREN_CHECK(...) LINQ_IS_PAREN_CHECK_N(__VA_ARGS__,0)
#define LINQ_IS_PAREN_PROBE(...) ~, 1,
#ifndef _MSC_VER
#define LINQ_IS_PAREN_CHECK_N(x, n, ...) n
#else
// MSVC workarounds
#define LINQ_IS_PAREN_CHECK_RES(x) x
#define LINQ_IS_PAREN_CHECK_II(x, n, ...) n
#define LINQ_IS_PAREN_CHECK_I(x) LINQ_IS_PAREN_CHECK_RES(LINQ_IS_PAREN_CHECK_II x)
#define LINQ_IS_PAREN_CHECK_N(...) LINQ_IS_PAREN_CHECK_I((__VA_ARGS__))
#endif

//
// LINQ_IS_EMPTY will expands to 1 if the parameter is empty, otherwise
// it expands to 0. This will work even if the parameter given is a set
// of parenthesis.
//
#define LINQ_IS_EMPTY(x) BOOST_PP_CAT(LINQ_IS_EMPTY_, LINQ_IS_PAREN(x))(x)
#define LINQ_IS_EMPTY_0(x) BOOST_PP_IS_EMPTY(x)
#define LINQ_IS_EMPTY_1(x) 0 

//
// LINQ_HEAD retrieves the first element of a sequence.
// Example:
//
//     LINQ_HEAD((1)(2)(3)) // Expands to (1)
//
#define LINQ_HEAD(x) LINQ_PICK_HEAD(LINQ_MARK x)
#define LINQ_MARK(...) (__VA_ARGS__), 
#define LINQ_PICK_HEAD(...) LINQ_PICK_HEAD_I(__VA_ARGS__,)
#ifndef _MSC_VER
#define LINQ_PICK_HEAD_I(x, ...) x
#else
// MSVC workarounds
#define LINQ_PICK_HEAD_III(x, ...) x
#define LINQ_PICK_HEAD_II(x) LINQ_PICK_HEAD_III x
#define LINQ_PICK_HEAD_I(...) LINQ_PICK_HEAD_II((__VA_ARGS__))
#endif


//
// LINQ_TAIL retrieves the tail of a sequence.
// Example:
//
//     LINQ_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define LINQ_TAIL(x) LINQ_EAT x
// Various utilities
#define LINQ_EAT(...)
#define LINQ_REM(...) __VA_ARGS__
#define LINQ_EXPAND(...) __VA_ARGS__

//
// LINQ_KEYWORD transforms the keyword. Keywords are generally defined
// like this:
//
//     //This defines a `my_keyword` macro
//     //NOTE: The space between the keyword and parenthesis is necessary
//     #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//
// Here is an example:
//
//     LINQ_KEYWORD(my_keyword foo) // Expands to (MY_KEYWORD_MACRO) foo
//
#define LINQ_KEYWORD(x) BOOST_PP_CAT(LINQ_KEYWORD_, x)

//
// LINQ_IS_KEYWORD will expand to 1 if the first token is a valid keyword
//
#define LINQ_IS_KEYWORD(x) LINQ_IS_PAREN(LINQ_KEYWORD(x))

//
// LINQ_PLACE retrieves whats placed in parenthesis. In essence it does
// this:
//
//     LINQ_PLACE((1) foo) //Expands to 1
//
// But when its used in the contexts of keywords, it will retrieve whats
// been defined inside the parenthesis of a keyword. Heres an example:
//
//     //This defines a `my_keyword` macro
//     //NOTE: The space between the keyword and parenthesis is necessary
//     #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//     LINQ_PLACE(LINQ_KEYWORD(my_keyword foo)) // Expands to MY_KEYWORD_MACRO
//
#define LINQ_PLACE(x) LINQ_EXPAND(LINQ_REM LINQ_PICK_HEAD(LINQ_MARK x))

namespace linq { namespace detail {

//This is used to workaround a bug in boost 1.49
template<class Fun>
struct transformer
{
    Fun f;

    transformer(Fun f): f(f)
    {}

    template<class F>
    struct result
    {};

    template<class F, class T>
    struct result<F(T)>
    {
        typedef decltype(std::declval<Fun>()(std::declval<T>())) type;
    };

    template<class T>
    auto operator()(T && x) const LINQ_RETURNS(f(std::forward<T>(x)))

    template<class T>
    auto operator()(T && x) LINQ_RETURNS(f(std::forward<T>(x)))
};

template<class F>
transformer<F> make_transformer(F f)
{
    return transformer<F>(f);
}

// These are used to avoid using parenthessis with lambdas, since it
// could perhaps result in a pathological case for the preprocessor.
struct where_t
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(boost::adaptors::filtered(p))
};

struct select_t
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(boost::adaptors::transformed(make_transformer(t)))
};
}

detail::select_t select = {};
detail::where_t where = {};
}

// Helps in defining lambdas in two steps. It also dedcues the type held
// by the container.
#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_HEADER(var, col) [&](decltype( *(boost::begin(col)) ) var)

// These macros help in defining the clauses. So instead of writing this:
//
//     auto r = numbers | boost::adaptors::filtered([](int i) { return i > 2});
//
// This can be written:
//
//     auto r = numbers | LINQ_WHERE(i, numbers)(i > 2);
//
#define LINQ_WHERE(var, col) linq::where + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT(var, col) linq::select + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK

//
// Keywords used by linq
//
#define LINQ_KEYWORD_from ()
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)

//
// LINQ_PROCESS will process a tuple as it parses the linq syntax. The
// tuple has four parameters:
// 
// var: is name a the variable passed in from the from clause
// coi : is the container passed in from the from clause
// prev: is the previously parsed linq query
// tail: is whats left, that needs to be parsed
//
#define LINQ_PROCESS(var, col, prev, tail) \
BOOST_PP_IF(LINQ_IS_PAREN(tail), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(var, col, prev, tail)

#define LINQ_PROCESS_PAREN(var, col, prev, tail) \
(var, col, prev LINQ_HEAD(tail), LINQ_TAIL(tail))

#define LINQ_PROCESS_KEYWORD(var, col, prev, tail) \
LINQ_PROCESS_REPLACE(var, col, prev, LINQ_KEYWORD(tail))

#define LINQ_PROCESS_REPLACE(var, col, prev, tail) \
(var, col, prev | LINQ_PLACE(tail)(var, col), LINQ_TAIL(tail))

// Predicate is used to determine if more needs to be parsed
#define LINQ_PREDICATE(var, col, prev, tail) BOOST_PP_NOT(LINQ_IS_EMPTY(tail))
// The final parsed output is the prev parameter
#define LINQ_OUT(var, col, prev, tail) prev

//
// While statement macros
//
#define LINQ_WHILE_X(...) __VA_ARGS__
#define LINQ_WHILE_P(r, state) LINQ_WHILE_X(LINQ_PREDICATE state)
#define LINQ_WHILE_O(r, state) LINQ_WHILE_X(LINQ_PROCESS state)
#define LINQ_WHILE_M(state) LINQ_WHILE_X(LINQ_OUT state)
#define LINQ_WHILE(var, col, tail) \
LINQ_WHILE_M(BOOST_PP_WHILE(LINQ_WHILE_P, LINQ_WHILE_O, (var, col, col, tail)))

// These retrieve the variable and container from the from clause, and
// start the while loop
#define LINQ_VAR(var, col) var
#define LINQ_COL(var, col) col
#define LINQ_FROM(from, tail) LINQ_WHILE(LINQ_VAR from, LINQ_COL from, tail)
#define LINQ_X(tail) LINQ_FROM(LINQ_HEAD(tail), LINQ_TAIL(tail))

// And finally the actual LINQ macro
#define LINQ(x) LINQ_X(LINQ_TAIL(LINQ_KEYWORD(x)))




#endif