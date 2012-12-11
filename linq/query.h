/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    query.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_LINQ_QUERY_H
#define LINQ_GUARD_LINQ_QUERY_H

#include <linq/pp.h>
#include <linq/extensions/group_by.h>
#include <linq/extensions/select.h>
#include <linq/extensions/select_many.h>
#include <linq/extensions/where.h>

namespace linq { 

namespace query {

template<class T>
const T& make_const(const T&);


template<class T>
const T& make_const(T&);

// These are used to avoid using parenthessis with lambdas, since it
// could perhaps result in a pathological case for the preprocessor.
struct where
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(linq::where(p));
};

struct select
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(linq::select(t));
};

struct select_many
{
    template<class Sel>
    auto operator+(Sel s) LINQ_RETURNS(linq::select_many(s));
};

template<class T>
struct let
{
    T x;
    let(T x) : x(x)
    {}
    template<class F>
    auto operator+(F && f) LINQ_RETURNS(f(std::forward<T>(x)));
};
}

}

// Helps in defining lambdas in two steps. It also dedcues the type held
// by the container.
#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_HEADER(var, col) [&](decltype( linq::query::make_const(*(boost::begin(col))) ) var)
#define LINQ_LAMBDA(var, col) LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK

// These macros help in defining the clauses. So instead of writing this:
//
// auto r = numbers | linq::where([](int i) { return i > 2});
//
// This can be written:
//
// auto r = numbers | LINQ_WHERE(i, numbers, i > 2);
//
#define LINQ_WHERE(var, col, ...) linq::where(LINQ_LAMBDA(var, col)(__VA_ARGS__))
#define LINQ_SELECT(var, col, ...) linq::select(LINQ_LAMBDA(var, col)(__VA_ARGS__))
#define LINQ_GROUP(var, col, key, x) linq::group_by(LINQ_LAMBDA(var, col)(key), LINQ_LAMBDA(var, col)(x))


#define LINQ_ORDERBY(var, col, ...) LINQ_ORDERBY_I(var, col, BOOST_PP_TUPLE_TO_SEQ(LINQ_NARGS(__VA_ARGS__), (__VA_ARGS__)))
#define LINQ_ORDERBY_I(var, col, seq) LINQ_ORDERBY_EACH(var, col, BOOST_PP_SEQ_HEAD(seq)) LINQ_SEQ_TO_STRING(LINQ_THENBY_EACH(var, col, BOOST_PP_SEQ_TAIL(seq)))

#ifndef _MSC_VER
#define LINQ_ORDERBY_INVOKE(args) LINQ_ORDERBY_METHOD_I args
#else
// MSVC Workarounds
#define LINQ_ORDERBY_INVOKE(args) LINQ_MSVC_INVOKE(LINQ_ORDERBY_METHOD_I, args)
#endif
#define LINQ_ORDERBY_METHOD_I(var, col, method, x) linq::method (LINQ_LAMBDA(var, col)(x))
#define LINQ_ORDERBY_METHOD(data, f) LINQ_ORDERBY_INVOKE((LINQ_REM data, LINQ_REM f))

#define LINQ_ORDERBY_EACH(var, col, x)  LINQ_ORDERBY_METHOD((var, col), LINQ_FIND(LINQ_ORDER_KEYWORD_, x, order_by)) 

#define LINQ_THENBY_EACH(var, col, seq) BOOST_PP_IIF(LINQ_IS_PAREN(seq), BOOST_PP_SEQ_TRANSFORM, LINQ_EAT)(LINQ_THENBY_EACH_OP, (var, col), seq)
#define LINQ_THENBY_EACH_OP(d, data, x) | LINQ_ORDERBY_METHOD(data, LINQ_FIND(LINQ_THEN_KEYWORD_, x, then_by))  \


#define LINQ_SELECT_MANY(var, col) linq::query::select_many() + LINQ_LAMBDA(var, col)



#define LINQ_LET(var, val) linq::query::let<decltype(val)>(val) + [&](decltype(val) var) LINQ_LAMBDA_BLOCK

//
// Keywords used by linq
//
#define LINQ_KEYWORD_from (from)
#define LINQ_KEYWORD_group (LINQ_GROUP)
#define LINQ_KEYWORD_into (into)
#define LINQ_KEYWORD_join (join)
#define LINQ_KEYWORD_on (on)
#define LINQ_KEYWORD_orderby (LINQ_ORDERBY)
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)

#define LINQ_THEN_KEYWORD_ascending (then_by)
#define LINQ_THEN_KEYWORD_descending (then_by_descending)

#define LINQ_ORDER_KEYWORD_ascending (order_by)
#define LINQ_ORDER_KEYWORD_descending (order_by_descending)

//
// Clauses that create scope
//
#define LINQ_SCOPE_KEYWORD_from (LINQ_FROM)

#define LINQ_FROM(var, col) col | LINQ_SELECT_MANY(var, col)

//
// Process the sequence
//

// Expands the parameter, for another preprocessor scan
#define LINQ_X(...) __VA_ARGS__

#define LINQ_PROCESS_PAREN(data, x) (LINQ_REM data, LINQ_REM x)
#ifndef _MSC_VER
#define LINQ_PROCESS_KEYWORD(data, x) | x 
#else
// MSVC Workarounds
#define LINQ_PROCESS_KEYWORD_RES(x) x
#define LINQ_PROCESS_KEYWORD(data, x) | LINQ_PROCESS_KEYWORD_RES(x)
#endif

#define LINQ_COL(var, col) col

// Process the select, where clauses
#define LINQ_TRANSFORM_CLAUSES(seq) LINQ_TRANSFORM_CLAUSES_TRANSFORM(BOOST_PP_SEQ_ELEM(0, seq) ,BOOST_PP_SEQ_REST_N(1, seq))
#define LINQ_TRANSFORM_CLAUSES_TRANSFORM(data, seq) LINQ_COL data LINQ_SEQ_TO_STRING(BOOST_PP_SEQ_TRANSFORM(LINQ_TRANSFORM_CLAUSES_O, data, seq))
#define LINQ_TRANSFORM_CLAUSES_O(s, data, x) BOOST_PP_IF(LINQ_IS_PAREN(x), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(data, x)


#define LINQ_FOLD_SCOPE(seq) BOOST_PP_SEQ_FOLD_LEFT(op, state, seq) 

#define LINQ_FOLD_SCOPE_OP()

// Process from clauses
// ()((x, col))()((y, x.col))(LINQ_SELECT)((x))
// SPLIT
// ( ((x, col)) ) ( ((y, x.col))(LINQ_SELECT)((x)) )
// TRANSFORM
// (SELECT_MANY(x, col))((y, x.col)(LINQ_SELECT)(x))
// NEST
#define LINQ_TRANSFORM_SCOPE(seq) LINQ_TRANSFORM_SCOPE_CHECK(LINQ_SEQ_SPLIT(seq, LINQ_TRANSFORM_SCOPE_P, data))
#define LINQ_TRANSFORM_SCOPE_CHECK(seq) BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), LINQ_TRANSFORM_SCOPE_MULTI, LINQ_TRANSFORM_SCOPE_SINGLE)(seq)
#define LINQ_TRANSFORM_SCOPE_MULTI(seq) LINQ_SEQ_NEST(LINQ_TRANSFORM_SCOPE_TRANSFORM(seq))
#define LINQ_TRANSFORM_SCOPE_SINGLE(seq) LINQ_TRANSFORM_CLAUSES(BOOST_PP_SEQ_HEAD(seq))


#define LINQ_TRANSFORM_SCOPE_TRANSFORM(seq) BOOST_PP_SEQ_TRANSFORM(LINQ_TRANSFORM_SCOPE_OP, data, BOOST_PP_SEQ_POP_BACK(seq)) (LINQ_TRANSFORM_CLAUSES(LINQ_BACK(seq)))

#define LINQ_TRANSFORM_SCOPE_P(s, data, x) LINQ_CONTAINS(LINQ_SCOPE_KEYWORD_, x)
#ifndef _MSC_VER
#define LINQ_TRANSFORM_SCOPE_OP(s, data, x) LINQ_FROM LINQ_REM x
#else
// MSVC Workarounds
#define LINQ_TRANSFORM_SCOPE_OP(s, data, x) LINQ_TRANSFORM_SCOPE_OP_INVOKE(LINQ_REM x)
#define LINQ_TRANSFORM_SCOPE_OP_INVOKE_X(x) x
#define LINQ_TRANSFORM_SCOPE_OP_INVOKE(x) LINQ_TRANSFORM_SCOPE_OP_INVOKE_X(LINQ_FROM x)
#endif


// Transforms the sequence
#define LINQ_TRANSFORM(seq) LINQ_X(LINQ_TRANSFORM_SCOPE(seq))
// And finally the LINQ macro
#define LINQ(x) LINQ_TRANSFORM(LINQ_TO_SEQ(x))

#ifdef LINQ_TEST_QUERY

// LINQ(from(i, v) select(i * 3))
LINQ(from(i, v) select(i * 3))

// LINQ(from(x, xx) from(y, yy) select(x * 3))
LINQ(from(x, xx) from(y, yy) select(x * 3))

// LINQ(from(i, v) select(i * 3) groupby(i))
LINQ(from(i, v) select(i * 3) groupby(i, i*3))

// LINQ(from(i, v) select(i * 3) orderby(i))
LINQ(from(i, v) select(i * 3) orderby(i))

// LINQ(from(i, v) select(i * 3) orderby(i, i*3))
LINQ(from(i, v) select(i * 3) orderby(i, i*3))

// LINQ(from(p, people) orderby(descending p.age) select(p.age))
LINQ(from(p, people) orderby(descending p.age) select(p.age))

#endif


#endif
