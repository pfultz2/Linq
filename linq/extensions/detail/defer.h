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
    struct result
    : linq::result_of<X>
    {};

};

}

#endif
