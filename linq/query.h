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
#include <linq/extensions/join.h>
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

template<class Inner, class OuterSelector, class InnerSelector>
struct inner_join
{
    Inner inner;
    OuterSelector os;
    InnerSelector is;

    inner_join(Inner inner, OuterSelector os, InnerSelector is)
    : inner(inner), os(os), is(is)
    {}

    template<class Sel>
    auto operator+(Sel rs) LINQ_RETURNS
    (linq::join(std::forward<Inner>(inner), os, is, rs));
};

template<class Inner, class OuterSelector, class InnerSelector>
inner_join < Inner, OuterSelector, InnerSelector >
make_inner_join (Inner && inner, OuterSelector os, InnerSelector is)
{
    return inner_join < Inner, OuterSelector, InnerSelector >
    (std::forward<Inner>(inner), os, is);
}

template<class Inner>
struct join
{
    Inner inner;

    join(Inner inner)
    : inner(inner)
    {}

    template<class Pair>
    auto operator+(Pair p) LINQ_RETURNS
    (make_inner_join(std::forward<Inner>(inner), p.first, p.second));
};

template<class Inner>
join < Inner >
make_join (Inner && inner)
{
    return join < Inner >
    (std::forward<Inner>(inner));
}




// template<class Inner, class OuterSelector, class InnerSelector>
// struct join
// {
//     Inner inner;
//     OuterSelector os;
//     InnerSelector is;

//     join(Inner inner, OuterSelector os, InnerSelector is)
//     : inner(inner), os(os), is(is)
//     {}

//     template<class Sel>
//     auto operator+(Sel rs) LINQ_RETURNS
//     (linq::join(std::forward<Inner>(inner), std::forward<OuterSelector>(os), std::forward<InnerSelector>(is), rs));
// };

// template<class Inner, class OuterSelector, class InnerSelector>
// join < Inner, OuterSelector, InnerSelector >
// make_join (Inner && inner, OuterSelector && os, InnerSelector && is)
// {
//     return join < Inner, OuterSelector, InnerSelector >
//     (std::forward<Inner>(inner), std::forward<OuterSelector>(os), std::forward<InnerSelector>(is));
// }


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

#define LINQ_PIPE_I() |
#define LINQ_PIPE() LINQ_PIPE_I LINQ_EMPTY() ()

// Helps in defining lambdas in two steps. It also dedcues the type held
// by the container.
#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_TYPE(col) decltype( linq::query::make_const(*(boost::begin(col))) )
#define LINQ_LAMBDA_HEADER(var, col) [&](LINQ_LAMBDA_TYPE(col) var)
// The `LINQ_EMPTY()` is needed on the end for MSVC
#define LINQ_LAMBDA(var, col) LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK LINQ_EMPTY()

// These macros help in defining the clauses. So instead of writing this:
//
// auto r = numbers | linq::where([](int i) { return i > 2});
//
// This can be written:
//
// auto r = numbers | LINQ_WHERE(i, numbers, i > 2);
//
#define LINQ_WHERE(var, col, ...) LINQ_PIPE() linq::where(LINQ_LAMBDA(var, col)(__VA_ARGS__))
#define LINQ_SELECT(var, col, ...) LINQ_PIPE() linq::select(LINQ_LAMBDA(var, col)(__VA_ARGS__))
#define LINQ_GROUP(var, col, key, x) LINQ_PIPE() linq::group_by(LINQ_LAMBDA(var, col)(key), LINQ_LAMBDA(var, col)(x))


#define LINQ_ORDERBY(var, col, ...) LINQ_PIPE() LINQ_ORDERBY_I(var, col, BOOST_PP_TUPLE_TO_SEQ(LINQ_NARGS(__VA_ARGS__), (__VA_ARGS__)))
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
#define LINQ_THENBY_EACH_OP(d, data, x) LINQ_PIPE() LINQ_ORDERBY_METHOD(data, LINQ_FIND(LINQ_THEN_KEYWORD_, x, then_by))  \


#define LINQ_SELECT_MANY(var, col) linq::query::select_many() + LINQ_LAMBDA(var, col)


#define LINQ_JOIN(ovar, ocol, ivar, icol, os, is) ocol LINQ_PIPE() linq::query::make_inner_join(icol, LINQ_LAMBDA(ovar, ocol)(os), LINQ_LAMBDA(ivar, icol)(is)) + 


#define LINQ_LET(var, val) linq::query::let<decltype(val)>(val) + [&](decltype(val) var) LINQ_LAMBDA_BLOCK

//
// Keywords used by linq
//
#define LINQ_KEYWORD_from (from)
#define LINQ_KEYWORD_group (LINQ_GROUP)
#define LINQ_KEYWORD_into (into)
#define LINQ_KEYWORD_join (join)
#define LINQ_KEYWORD_on ()
#define LINQ_KEYWORD_orderby (LINQ_ORDERBY)
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)

#define LINQ_THEN_KEYWORD_ascending (then_by)
#define LINQ_THEN_KEYWORD_descending (then_by_descending)

#define LINQ_ORDER_KEYWORD_ascending (order_by)
#define LINQ_ORDER_KEYWORD_descending (order_by_descending)

//
// From keyword
//
#define LINQ_FROM_KEYWORD_from ()

//
// Join keyword
//
#define LINQ_JOIN_KEYWORD_join ()

//
// Into keyword
//
#define LINQ_INTO_KEYWORD_into ()

//
#define LINQ_FROM(var, col) col LINQ_PIPE() LINQ_SELECT_MANY(var, col)

//
// Process the sequence
//

// Expands the parameter, for another preprocessor scan
#define LINQ_X(...) __VA_ARGS__


#define LINQ_PROCESS_PAREN(data, x) (LINQ_REM data, LINQ_REM x)
#define LINQ_PROCESS_KEYWORD(data, x) x 

#define LINQ_COL(var, col) col

// Process the select, where clauses
#define LINQ_TRANSFORM_CLAUSES(seq) LINQ_TRANSFORM_CLAUSES_TRANSFORM(BOOST_PP_SEQ_ELEM(0, seq) ,BOOST_PP_SEQ_REST_N(1, seq))
#define LINQ_TRANSFORM_CLAUSES_TRANSFORM(data, seq) (LINQ_COL data) BOOST_PP_SEQ_TRANSFORM(LINQ_TRANSFORM_CLAUSES_O, data, seq)
#define LINQ_TRANSFORM_CLAUSES_O(s, data, x) BOOST_PP_IF(LINQ_IS_PAREN(x), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(data, x)

// Process from clauses
// ()((x, col))()((y, x.col))(LINQ_SELECT)((x))
// SPLIT
// ( ((x, col)) ) ( ((y, x.col))(LINQ_SELECT)((x)) )
// TRANSFORM
// (SELECT_MANY(x, col))((y, x.col)(LINQ_SELECT)(x))
// NEST
#define LINQ_TRANSFORM_FROM_CLAUSES(seq) LINQ_SEQ_NEST(LINQ_TRANSFORM_FROM_CLAUSES_EACH(LINQ_SEQ_SPLIT(seq, LINQ_TRANSFORM_FROM_P, data)))

#define LINQ_TRANSFORM_FROM_CLAUSES_EACH(seq) LINQ_TRANSFORM_FROM_CLAUSES_EACH_I(LINQ_TRANSFORM_FROM_CLAUSES_EACH_1 seq)
#define LINQ_TRANSFORM_FROM_CLAUSES_EACH_I(seq) BOOST_PP_CAT(seq, _END)
#define LINQ_TRANSFORM_FROM_CLAUSES_EACH_1(x) (LINQ_TRANSFORM_FROM_CLAUSES_OP(x)) LINQ_TRANSFORM_FROM_CLAUSES_EACH_2
#define LINQ_TRANSFORM_FROM_CLAUSES_EACH_2(x) (LINQ_TRANSFORM_FROM_CLAUSES_OP(x)) LINQ_TRANSFORM_FROM_CLAUSES_EACH_1
#define LINQ_TRANSFORM_FROM_CLAUSES_EACH_1_END
#define LINQ_TRANSFORM_FROM_CLAUSES_EACH_2_END


#define LINQ_TRANSFORM_FROM_CLAUSES_OP(x) LINQ_SEQ_TO_STRING \
( \
    BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(x)), LINQ_TRANSFORM_FROM_CLAUSES_OTHER, LINQ_TRANSFORM_FROM_CLAUSES_FROM)(x) \
)

#define LINQ_TRANSFORM_FROM_CLAUSES_FROM(x) (LINQ_FROM) x
#define LINQ_TRANSFORM_FROM_CLAUSES_OTHER(x) LINQ_TRANSFORM_CLAUSES(x)


#define LINQ_TRANSFORM_FROM_P(s, data, x) LINQ_CONTAINS(LINQ_FROM_KEYWORD_, x)


// Transforms the sequence
#define LINQ_TRANSFORM(seq) LINQ_X(LINQ_TRANSFORM_FROM_CLAUSES(seq))
// And finally the LINQ macro
#define LINQ(x) LINQ_TRANSFORM(LINQ_TO_SEQ(x))

#ifdef LINQ_TEST_QUERY

// LINQ_NEST
LINQ_SEQ_NEST((x))
// LINQ_NEST
LINQ_SEQ_NEST((f)(y))
// LINQ_NEST
LINQ_SEQ_NEST((f)(g)(y))

// LINQ_FOLD_JOIN
// LINQ_FOLD_JOIN( ((x, xx)) (LINQ_SELECT)((x, y, x * 3)) )
// LINQ_FOLD_JOIN( ((x, xx)) (join)((x, xx, y, yy))((x, xx, x, y)) )
// LINQ_FOLD_JOIN_MERGE((LINQ_JOIN), ((x, xx, y, yy))((x, xx, x, y)))
// LINQ_FOLD_JOIN( ((x, xx)) (join)((x, xx, y, yy))((x, xx, x, y)) (LINQ_SELECT)((x, xx, x * 3)) )

// LINQ_JOIN
// LINQ(from(category, categories)
//    join(prod, products) on(category.ID, prod.CategoryID)
//    select(prod.name))

// LINQ(from(i, v) select(i * 3))
LINQ(from(i, v) select(i * 3))

// LINQ(from(x, xx) from(y, yy) select(x * 3))
LINQ(from(x, xx) from(y, yy) select(x * 3))

// LINQ(from(i, v) select(i * 3) group(i))
LINQ(from(i, v) select(i * 3) group(i, i*3))

// LINQ(from(i, v) select(i * 3) orderby(i))
LINQ(from(i, v) select(i * 3) orderby(i))

// LINQ(from(i, v) select(i * 3) orderby(i, i*3))
LINQ(from(i, v) select(i * 3) orderby(i, i*3))

// LINQ(from(p, people) orderby(descending p.age) select(p.age))
LINQ(from(p, people) orderby(descending p.age) select(p.age))

#endif


#endif
