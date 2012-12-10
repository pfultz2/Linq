/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_RESULT_OF_H
#define LINQ_GUARD_DETAIL_RESULT_OF_H

#include <linq/utility.h>
#include <linq/pp.h>
#include <utility>

#ifndef LINQ_RESULT_OF_LIMIT
#define LINQ_RESULT_OF_LIMIT 8
#endif

namespace linq { 

template<class F>
struct result_of;

#define LINQ_RESULT_OF_M(z, n, data) linq::declval<T ## n>()
#define LINQ_RESULT_OF_EACH(z, n, data) \
template<class X BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
struct result_of<X(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
{ \
    typedef decltype(linq::declval<X>()(BOOST_PP_ENUM_ ## z(n, LINQ_RESULT_OF_M, ~) )) type;\
}; 

BOOST_PP_REPEAT_1(LINQ_RESULT_OF_LIMIT, LINQ_RESULT_OF_EACH, ~)



}

#endif
