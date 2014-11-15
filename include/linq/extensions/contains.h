/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    contains.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_CONTAINS_H
#define LINQ_GUARD_EXTENSIONS_CONTAINS_H

#include <linq/extensions/extension.h>
#include <linq/extensions/find.h>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 

//
// contains
//
namespace detail {
struct contains_t
{
    template<class Range, class T>
    bool operator()(Range && r, T && x) const
    { return (r | linq::find(x)) != boost::end(r); };
};
}
namespace {
range_extension<detail::contains_t> contains = {};
}

}

#endif
