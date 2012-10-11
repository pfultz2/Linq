/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    always.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_ALWAYS_H
#define LINQ_GUARD_DETAIL_ALWAYS_H

namespace linq { 

//
// always predicate
//
namespace detail {

struct always
{
    template<class T>
    bool operator()(T) { return true; }
};
}

}

#endif
