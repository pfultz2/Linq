/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    query.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_LINQ_QUERY_H
#define LINQ_GUARD_LINQ_QUERY_H

namespace linq { 

namespace detail {

// These are used to avoid using parenthessis with lambdas, since it
// could perhaps result in a pathological case for the preprocessor.
struct where_t
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(boost::adaptors::filtered(linq::make_function_object(p)));
};

struct select_t
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(boost::adaptors::transformed(linq::make_function_object(t)));
};

struct select_many_t
{
    template<class Sel>
    auto operator+(Sel s) LINQ_RETURNS(linq::bound(make_function_object(s)));
};
}

detail::select_many_t select_many = {};
detail::select_t select = {};
detail::where_t where = {};

}

// Helps in defining lambdas in two steps. It also dedcues the type held
// by the container.
#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_HEADER(var, col) [&](decltype( *(boost::begin(col)) ) var)

// These macros help in defining the clauses. So instead of writing this:
//
// auto r = numbers | boost::adaptors::filtered([](int i) { return i > 2});
//
// This can be written:
//
// auto r = numbers | LINQ_WHERE(i, numbers)(i > 2);
//
#define LINQ_WHERE(var, col) linq::where + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT(var, col) linq::select + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT_MANY(var, col) linq::select_many + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK

//
// Keywords used by linq
//
#define LINQ_KEYWORD_from ()
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)

//
// Process the sequence
//

// Expands the parameter, for another preprocessor scan
#define LINQ_X(...) __VA_ARGS__

#define LINQ_PROCESS_PAREN(data, x) x
#ifndef _MSC_VER
#define LINQ_PROCESS_KEYWORD(data, x) | x data
#else
// MSVC Workarounds
#define LINQ_PROCESS_KEYWORD_RES(x) x
#define LINQ_PROCESS_KEYWORD(data, x) | LINQ_PROCESS_KEYWORD_RES(x data)
#endif

#define LINQ_COL(var, col) col

// Process the select, where clauses
#define LINQ_SELECT_WHERE(seq) LINQ_SELECT_WHERE_TRANSFORM(BOOST_PP_SEQ_ELEM(0, seq) ,BOOST_PP_SEQ_REST_N(1, seq))
#define LINQ_SELECT_WHERE_TRANSFORM(data, seq) LINQ_COL data LINQ_SEQ_TO_STRING(BOOST_PP_SEQ_TRANSFORM(LINQ_SELECT_WHERE_O, data, seq))
#define LINQ_SELECT_WHERE_O(s, data, x) BOOST_PP_IF(LINQ_IS_PAREN(x), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(data, x)

// Process from clauses
// ()((x, col))()((y, x.col))(LINQ_SELECT)((x))
// SPLIT
// ( ((x, col)) ) ( ((y, x.col))(LINQ_SELECT)((x)) )
// TRANSFORM
// (SELECT_MANY(x, col))((y, x.col)(LINQ_SELECT)(x))
// NEST
#define LINQ_FROM(seq) LINQ_FROM_CHECK(LINQ_SEQ_SPLIT(seq, LINQ_FROM_P, data))
#define LINQ_FROM_CHECK(seq) BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), LINQ_FROM_MULTI, LINQ_FROM_SINGLE)(seq)
#define LINQ_FROM_MULTI(seq) LINQ_SEQ_NEST(LINQ_FROM_TRANSFORM(seq))
#define LINQ_FROM_SINGLE(seq) LINQ_SELECT_WHERE(BOOST_PP_SEQ_HEAD(seq))


#define LINQ_FROM_TRANSFORM(seq) BOOST_PP_SEQ_TRANSFORM(LINQ_FROM_OP, data, BOOST_PP_SEQ_POP_BACK(seq)) (LINQ_SELECT_WHERE(LINQ_BACK(seq)))

#define LINQ_FROM_P(s, data, x) LINQ_IS_EMPTY(x)
#ifndef _MSC_VER
#define LINQ_FROM_OP(s, data, x) LINQ_PROCESS_FROM LINQ_REM x
#else
// MSVC Workarounds
#define LINQ_FROM_OP(s, data, x) LINQ_FROM_OP_INVOKE(LINQ_REM x)
#define LINQ_FROM_OP_INVOKE_X(x) x
#define LINQ_FROM_OP_INVOKE(x) LINQ_FROM_OP_INVOKE_X(LINQ_PROCESS_FROM x)
#endif
#define LINQ_PROCESS_FROM(var, col) col | LINQ_SELECT_MANY(var, col)

// Transforms the sequence
#define LINQ_TRANSFORM(seq) LINQ_X(LINQ_FROM(seq))
// And finally the LINQ macro
#define LINQ(x) LINQ_TRANSFORM(LINQ_TO_SEQ(x))


}

#endif
