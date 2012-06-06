

#ifndef INCLUDE_GUARD_LINQ_H
#define INCLUDE_GUARD_LINQ_H

#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

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


#define LINQ_IS_EMPTY(x) BOOST_PP_CAT(LINQ_IS_EMPTY_, LINQ_IS_PAREN(x))(x)
#define LINQ_IS_EMPTY_0(x) BOOST_PP_IS_EMPTY(x)
#define LINQ_IS_EMPTY_1(x) 0 

#define LINQ_MARK(...) (__VA_ARGS__), 
#define LINQ_PICK_HEAD_I(x, ...) x
#define LINQ_PICK_HEAD(...) LINQ_PICK_HEAD_I(__VA_ARGS__,)
#define LINQ_HEAD(x) LINQ_PICK_HEAD(LINQ_MARK x)

#define LINQ_TAIL(x) LINQ_EAT x
#define LINQ_EAT(...)
#define LINQ_REM(...) __VA_ARGS__
#define LINQ_EXPAND(...) __VA_ARGS__

#define LINQ_KEYWORD(x) BOOST_PP_CAT(LINQ_KEYWORD_, x)

#define LINQ_IS_KEYWORD(x) LINQ_IS_PAREN(LINQ_KEYWORD(x))

#define LINQ_REPLACE(x) LINQ_EXPAND(LINQ_REM LINQ_PICK_HEAD(LINQ_MARK x))

namespace linq { namespace detail {

struct where_t
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(boost::adaptors::filtered(p))
};

struct select_t
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(boost::adaptors::transformed(t))
};
}

detail::select_t select = {};
detail::where_t where = {};
}

#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_HEADER(var, col) [&](decltype( *(boost::begin(col)) ) var)
#define LINQ_WHERE(var, col) linq::where + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT(var, col) linq::select + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK


#define LINQ_KEYWORD_from ()
#define LINQ_KEYWORD_for ()
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)
#define LINQ_KEYWORD_if (LINQ_WHERE)



#define LINQ_PROCESS(var, col, prev, tail) \
BOOST_PP_IF(LINQ_IS_PAREN(tail), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(var, col, prev, tail)

#define LINQ_PROCESS_PAREN(var, col, prev, tail) \
(var, col, prev LINQ_HEAD(tail), LINQ_TAIL(tail))

#define LINQ_PROCESS_KEYWORD(var, col, prev, tail) \
LINQ_PROCESS_REPLACE(var, col, prev, LINQ_KEYWORD(tail))

#define LINQ_PROCESS_REPLACE(var, col, prev, tail) \
(var, col, prev | LINQ_REPLACE(tail)(var, col), LINQ_TAIL(tail))

#define LINQ_PREDICATE(var, col, prev, tail) BOOST_PP_NOT(LINQ_IS_EMPTY(tail))
#define LINQ_OUT(var, col, prev, tail) prev

#define LINQ_WHILE_X(...) __VA_ARGS__
#define LINQ_WHILE_P(r, state) LINQ_WHILE_X(LINQ_PREDICATE state)
#define LINQ_WHILE_O(r, state) LINQ_WHILE_X(LINQ_PROCESS state)
#define LINQ_WHILE_M(state) LINQ_WHILE_X(LINQ_OUT state)
#define LINQ_WHILE(var, col, tail) \
LINQ_WHILE_M(BOOST_PP_WHILE(LINQ_WHILE_P, LINQ_WHILE_O, (var, col, col, tail)))


#define LINQ_VAR(var, col) var
#define LINQ_COL(var, col) col
#define LINQ_FROM(from, tail) LINQ_WHILE(LINQ_VAR from, LINQ_COL from, tail)
#define LINQ_X(tail) LINQ_FROM(LINQ_HEAD(tail), LINQ_TAIL(tail))

#define LINQ(x) LINQ_X(LINQ_TAIL(LINQ_KEYWORD(x)))




#endif