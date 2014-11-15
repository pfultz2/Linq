/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    defer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_DEFER_H
#define LINQ_GUARD_DETAIL_DEFER_H

#include <linq/pp.h>
#include <linq/utility.h>
#include <linq/extensions/detail/result_of.h>

#ifndef LINQ_DEFER_LIMIT
#define LINQ_DEFER_LIMIT 8
#endif

namespace linq {

template<class F>
struct defer : F
{
    defer()
    {}

    template<class X>
    struct result;

#define LINQ_DEFER_EACH(z, n, data) \
    template<class X BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
    struct result<X(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
    : linq::result_of<F(BOOST_PP_ENUM_PARAMS_Z(z, n, T)) > \
    {};  
BOOST_PP_REPEAT_1(LINQ_DEFER_LIMIT, LINQ_DEFER_EACH, ~)

};

}

#endif
