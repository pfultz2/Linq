/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pp.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_PP_H
#define INCLUDE_GUARD_LINQ_PP_H

#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

//
//
// Some preprocessor utilites
//
//

#define LINQ_NARGS(...) \
         LINQ_DETAIL_NARG((__VA_ARGS__,LINQ_DETAIL_RSEQ_N()))
#define LINQ_DETAIL_NARG(args) \
         LINQ_DETAIL_ARG_N args
#define LINQ_DETAIL_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define LINQ_DETAIL_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 

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
// LINQ_MSVC_INVOKE invokes a macro on msvc
//
#define LINQ_MSVC_INVOKE BOOST_PP_CAT(LINQ_MSVC_INVOKE_, BOOST_PP_AUTO_REC(LINQ_DETAIL_MSVC_INVOKE_P, 8))

#define LINQ_DETAIL_MSVC_INVOKE_P(n) LINQ_IS_PAREN( LINQ_MSVC_INVOKE_ ## n((),) )

#define LINQ_MSVC_INVOKE_1(macro, args) LINQ_MSVC_INVOKE_I_1(macro, args)
#define LINQ_MSVC_INVOKE_2(macro, args) LINQ_MSVC_INVOKE_I_2(macro, args)
#define LINQ_MSVC_INVOKE_3(macro, args) LINQ_MSVC_INVOKE_I_3(macro, args)
#define LINQ_MSVC_INVOKE_4(macro, args) LINQ_MSVC_INVOKE_I_4(macro, args)
#define LINQ_MSVC_INVOKE_5(macro, args) LINQ_MSVC_INVOKE_I_5(macro, args)
#define LINQ_MSVC_INVOKE_6(macro, args) LINQ_MSVC_INVOKE_I_6(macro, args)
#define LINQ_MSVC_INVOKE_7(macro, args) LINQ_MSVC_INVOKE_I_7(macro, args)
#define LINQ_MSVC_INVOKE_8(macro, args) LINQ_MSVC_INVOKE_I_8(macro, args)

#define LINQ_MSVC_INVOKE_I_1(macro, args) LINQ_MSVC_INVOKE_X_1(macro args)
#define LINQ_MSVC_INVOKE_I_2(macro, args) LINQ_MSVC_INVOKE_X_2(macro args)
#define LINQ_MSVC_INVOKE_I_3(macro, args) LINQ_MSVC_INVOKE_X_3(macro args)
#define LINQ_MSVC_INVOKE_I_4(macro, args) LINQ_MSVC_INVOKE_X_4(macro args)
#define LINQ_MSVC_INVOKE_I_5(macro, args) LINQ_MSVC_INVOKE_X_5(macro args)
#define LINQ_MSVC_INVOKE_I_6(macro, args) LINQ_MSVC_INVOKE_X_6(macro args)
#define LINQ_MSVC_INVOKE_I_7(macro, args) LINQ_MSVC_INVOKE_X_7(macro args)
#define LINQ_MSVC_INVOKE_I_8(macro, args) LINQ_MSVC_INVOKE_X_8(macro args)

#define LINQ_MSVC_INVOKE_X_1(x) x
#define LINQ_MSVC_INVOKE_X_2(x) x
#define LINQ_MSVC_INVOKE_X_3(x) x
#define LINQ_MSVC_INVOKE_X_4(x) x
#define LINQ_MSVC_INVOKE_X_5(x) x
#define LINQ_MSVC_INVOKE_X_6(x) x
#define LINQ_MSVC_INVOKE_X_7(x) x
#define LINQ_MSVC_INVOKE_X_8(x) x


#define LINQ_CONTAINS(set, x) BOOST_PP_IIF(LINQ_IS_PAREN(x), LINQ_CONTAINS_PAREN, LINQ_CONTAINS_TOKEN)(set, x)
#define LINQ_CONTAINS_PAREN(set, x) 0
#define LINQ_CONTAINS_TOKEN(set, x) LINQ_IS_PAREN(BOOST_PP_CAT(set, x))

#define LINQ_FIND(set, x, default_) BOOST_PP_IIF(LINQ_IS_PAREN(x), LINQ_FIND_PAREN, LINQ_FIND_TOKEN)(set, x, default_)
#define LINQ_FIND_PAREN(set, x, default_) (default_, x)
#define LINQ_FIND_TOKEN(set, x, default_) LINQ_FIND_TOKEN_I(x, BOOST_PP_CAT(set, x), default_)
#define LINQ_FIND_TOKEN_I(x, transformed, default_) BOOST_PP_IIF(LINQ_IS_PAREN(transformed), LINQ_FIND_FOUND, LINQ_FIND_NOT_FOUND)(x, transformed, default_)
#define LINQ_FIND_FOUND(x, transformed, default_) (LINQ_PLACE(transformed), LINQ_EAT transformed)
#define LINQ_FIND_NOT_FOUND(x, transformed, default_) (default_, x)

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
// LINQ_HEAD((1)(2)(3)) // Expands to (1)
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
// LINQ_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define LINQ_TAIL(x) LINQ_EAT x
// Various utilities
#define LINQ_EMPTY(...)
#define LINQ_EAT(...)
#define LINQ_REM(...) __VA_ARGS__
#define LINQ_OUT(x) LINQ_REM x 
#define LINQ_POP(x) LINQ_EAT x 
#define LINQ_EXPAND(...) __VA_ARGS__
#define LINQ_DEFER(...) __VA_ARGS__ LINQ_EMPTY()
#define LINQ_OBSTRUCT(...) __VA_ARGS__ LINQ_DEFER(LINQ_EMPTY)()

#define LINQ_EVAL(...) LINQ_EVAL_A(LINQ_EVAL_A(LINQ_EVAL_A(__VA_ARGS__)))
#define LINQ_EVAL_A(...) LINQ_EVAL_B(LINQ_EVAL_B(LINQ_EVAL_B(__VA_ARGS__)))
#define LINQ_EVAL_B(...) LINQ_EVAL_C(LINQ_EVAL_C(LINQ_EVAL_C(__VA_ARGS__)))
#define LINQ_EVAL_C(...) LINQ_EVAL_D(LINQ_EVAL_D(LINQ_EVAL_D(__VA_ARGS__)))
#define LINQ_EVAL_D(...) LINQ_EVAL_E(LINQ_EVAL_E(LINQ_EVAL_E(__VA_ARGS__)))
#define LINQ_EVAL_E(...) __VA_ARGS__

//
// LINQ_BACK gets the last element of a sequence
//
#define LINQ_BACK(seq) BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), seq)

//
// LINQ_KEYWORD transforms the keyword. Keywords are generally defined
// like this:
//
// //This defines a `my_keyword` macro
// //NOTE: The space between the keyword and parenthesis is necessary
// #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//
// Here is an example:
//
// LINQ_KEYWORD(my_keyword foo) // Expands to (MY_KEYWORD_MACRO) foo
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
// LINQ_PLACE((1) foo) //Expands to 1
//
// But when its used in the contexts of keywords, it will retrieve whats
// been defined inside the parenthesis of a keyword. Heres an example:
//
// //This defines a `my_keyword` macro
// //NOTE: The space between the keyword and parenthesis is necessary
// #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
// LINQ_PLACE(LINQ_KEYWORD(my_keyword foo)) // Expands to MY_KEYWORD_MACRO
//
#define LINQ_PLACE(x) LINQ_EXPAND(LINQ_REM LINQ_PICK_HEAD(LINQ_MARK x))

//
// LINQ_TO_SEQ converts the keywords into a preprocessor sequence
//
#define LINQ_TO_SEQ(x) LINQ_TO_SEQ_WHILE_M \
( \
BOOST_PP_WHILE(LINQ_TO_SEQ_WHILE_P, LINQ_TO_SEQ_WHILE_O, (,x)) \
)

#define LINQ_TO_SEQ_WHILE_P(r, state) LINQ_TO_SEQ_P state
#define LINQ_TO_SEQ_WHILE_O(r, state) LINQ_TO_SEQ_O state
#define LINQ_TO_SEQ_WHILE_M(state) LINQ_TO_SEQ_M state

#define LINQ_TO_SEQ_P(prev, tail) BOOST_PP_NOT(LINQ_IS_EMPTY(tail))
#define LINQ_TO_SEQ_O(prev, tail) \
BOOST_PP_IF(LINQ_IS_PAREN(tail), \
    LINQ_TO_SEQ_PAREN, \
    LINQ_TO_SEQ_KEYWORD \
    )(prev, tail)
#define LINQ_TO_SEQ_PAREN(prev, tail) \
(prev (LINQ_HEAD(tail)), LINQ_TAIL(tail))

#define LINQ_TO_SEQ_KEYWORD(prev, tail) \
LINQ_TO_SEQ_REPLACE(prev, LINQ_KEYWORD(tail))

#define LINQ_TO_SEQ_REPLACE(prev, tail) \
(prev LINQ_HEAD(tail), LINQ_TAIL(tail))

#define LINQ_TO_SEQ_M(prev, tail) prev


//
// LINQ_SEQ_TO_STRING convert a sequence back to a string of tokens
//
#define LINQ_SEQ_TO_STRING(seq) BOOST_PP_SEQ_FOR_EACH(LINQ_SEQ_TO_STRING_EACH, ~, seq)
#define LINQ_SEQ_TO_STRING_EACH(r, data, x) x

//
// LINQ_SEQ_SPLIT
//
#define LINQ_SEQ_SPLIT(seq, pred, data) LINQ_SEQ_SPLIT_FOLD_LEFT_M(BOOST_PP_SEQ_FOLD_LEFT(LINQ_SEQ_SPLIT_FOLD_LEFT_O, (pred, data,,), seq))
#define LINQ_SEQ_SPLIT_FOLD_LEFT_O(s, state, x) LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE((s, x, LINQ_REM state))
#ifndef _MSC_VER
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE(x) LINQ_SEQ_SPLIT_OP x
#else
// MSVC workarounds
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE(x) LINQ_MSVC_INVOKE(LINQ_SEQ_SPLIT_OP, x)
#endif
#define LINQ_SEQ_SPLIT_OP(s, x, pred, data, seq, elem) BOOST_PP_IF(pred(s, data, x), LINQ_SEQ_SPLIT_OP_TRUE, LINQ_SEQ_SPLIT_OP_FALSE)(x, pred, data, seq, elem)
#define LINQ_SEQ_SPLIT_OP_TRUE(x, pred, data, seq, elem) BOOST_PP_IIF(LINQ_IS_PAREN(elem), \
                                                                    (pred, data, seq(elem),),\
                                                                    (pred, data, seq,) )
#define LINQ_SEQ_SPLIT_OP_FALSE(x, pred, data, seq, elem) (pred, data, seq, elem (x))
#ifndef _MSC_VER
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M(x) LINQ_SEQ_SPLIT_M x
#else
// MSVC workarounds
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M(x) LINQ_MSVC_INVOKE(LINQ_SEQ_SPLIT_M, x)
#endif
#define LINQ_SEQ_SPLIT_M(pred, data, seq, elem) seq BOOST_PP_IIF(LINQ_IS_PAREN(elem), (elem),)

//
// LINQ_SEQ_NEST
//
#define LINQ_SEQ_NEST(seq) BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), LINQ_SEQ_NEST_FOLD, LINQ_OUT)(seq)
#define LINQ_SEQ_NEST_FOLD(seq) BOOST_PP_SEQ_FOLD_RIGHT(LINQ_SEQ_NEST_OP, LINQ_BACK(seq) , BOOST_PP_SEQ_POP_BACK(seq))
#define LINQ_SEQ_NEST_OP(s, state, x) x(state)

//
// LINQ_SEQ_REMOVE_EMPTY
//
#define LINQ_SEQ_REMOVE_EMPTY(seq) LINQ_SEQ_REMOVE_EMPTY_I(LINQ_SEQ_REMOVE_EMPTY_1 seq)
#define LINQ_SEQ_REMOVE_EMPTY_I(x) BOOST_PP_CAT(x, _END) 
#define LINQ_SEQ_REMOVE_EMPTY_1(x) BOOST_PP_IIF(LINQ_IS_EMPTY(x),,(x)) LINQ_SEQ_REMOVE_EMPTY_2
#define LINQ_SEQ_REMOVE_EMPTY_2(x) BOOST_PP_IIF(LINQ_IS_EMPTY(x),,(x)) LINQ_SEQ_REMOVE_EMPTY_1
#define LINQ_SEQ_REMOVE_EMPTY_1_END
#define LINQ_SEQ_REMOVE_EMPTY_2_END

//
// LINQ_SEQ_NEST_REVERSE
//
#define LINQ_SEQ_NEST_REVERSE(seq) BOOST_PP_SEQ_FOLD_RIGHT(LINQ_SEQ_NEST_OP, LINQ_BACK(seq) , BOOST_PP_SEQ_POP_BACK(seq))

//
// LINQ_VARN_CAT
//
#define LINQ_VARN_CAT(n, tuple) LINQ_EVAL(LINQ_VARN_CAT_D(n, tuple))
#define LINQ_VARN_CAT_D(n, tuple) LINQ_VARN_INVOKE((n, LINQ_REM tuple, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT))
#define LINQ_VARN_CAT_ID() LINQ_VARN_CAT_D

#ifndef _MSC_VER
#define LINQ_VARN_INVOKE(data)  LINQ_VARN_PRIMITIVE_CAT data
#else
// MSVC Workarounds
#define LINQ_VARN_INVOKE(data)  LINQ_VARN_INVOKE_I(data)
#define LINQ_VARN_INVOKE_I(data)  LINQ_VARN_PRIMITIVE_CAT data
#endif
#define LINQ_VARN_PRIMITIVE_CAT(n, a, b, c, d, e, f, g, h, ...) LINQ_VARN_CAT_E(a, n) LINQ_VARN_CAT_E(b, n) LINQ_VARN_CAT_E(c, n) LINQ_VARN_CAT_E(d, n) LINQ_VARN_CAT_E(e, n) LINQ_VARN_CAT_E(f, n) LINQ_VARN_CAT_E(g, n) LINQ_VARN_CAT_E(h, n) 

#define LINQ_VARN_CAT_E(a, n) BOOST_PP_IF(LINQ_IS_PAREN(a), LINQ_VARN_CAT_EACH_PAREN, LINQ_VARN_CAT_EACH_TOKEN)(a, n)
#define LINQ_VARN_CAT_EACH_PAREN(a, n) (LINQ_OBSTRUCT(LINQ_VARN_CAT_ID)()(n, a))
#define LINQ_VARN_CAT_EACH_TOKEN(a, n)  a ## n

//
// LINQ_PARAMS
//
#define LINQ_PARAMS(n, ...) BOOST_PP_ENUM(n, LINQ_PARAMS_EACH, (__VA_ARGS__))
#define LINQ_PARAMS_Z(z, n, ...) BOOST_PP_ENUM_ ## z(n, LINQ_PARAMS_EACH, (__VA_ARGS__)) 
#define LINQ_PARAMS_EACH(z, n, data) LINQ_VARN_CAT(n, data)

//
// LINQ_GEN
//
#define LINQ_GEN(n, ...) BOOST_PP_REPEAT(n, LINQ_GEN_EACH, (__VA_ARGS__)) 
#define LINQ_GEN_Z(z, n, ...) BOOST_PP_REPEAT_ ## z(n, LINQ_GEN_EACH, (__VA_ARGS__)) 
#define LINQ_GEN_EACH(z, n, data) LINQ_VARN_CAT(n, data)


//
// LINQ_FORWARD_PARAMS
//
#define LINQ_FORWARD_PARAMS(n, type, var) BOOST_PP_ENUM(n, LINQ_FORWARD_PARAMS_EACH, (type, var))
#define LINQ_FORWARD_PARAMS_Z(z, n, type, var) BOOST_PP_ENUM_ ## z(n, LINQ_FORWARD_PARAMS_EACH, (type, var))
#define LINQ_FORWARD_PARAMS_EACH(z, n, data) std::forward<BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 0, data),n)>(BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, data),n))


#endif
