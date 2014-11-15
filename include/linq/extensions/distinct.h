/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    distinct.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_DISTINCT_H
#define LINQ_GUARD_EXTENSIONS_DISTINCT_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/set_filter_iterator.h>

namespace linq { 

//
// distinct
//
namespace detail {
struct distinct_t
{
    struct predicate
    {
        template<class T, class Set>
        bool operator()(const T& x, Set& s) const
        {
            if (s.find(x) == s.end())
            {
                s.insert(x);
                return true;
            }
            else
            {
                return false;
            }
        }
    };
    // TODO: Add support for an equality selector
    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS(make_set_filter_range(r, predicate()));
};
}
namespace {
range_extension<detail::distinct_t, true> distinct = {};
}

}

#endif
