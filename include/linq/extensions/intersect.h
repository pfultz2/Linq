/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    intersect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_INTERSECT_H
#define LINQ_GUARD_EXTENSIONS_INTERSECT_H

#include <linq/extensions/extension.h>

namespace linq { 

//
// intersect
//
namespace detail {
struct intersect_t
{
    struct predicate
    {

        template<class T, class Set>
        bool operator()(const T& x, Set & s) const
        {
            auto it = s.find(x);
            if (it != s.end())
            {
                s.erase(it);
                return true;
            }
            else return false;
        }
    };
    // TODO: Add support for an equality selector
    template<class Range1, class Range2>
    auto operator()(Range1 && r1, Range2 && r2) const LINQ_RETURNS
    (make_set_filter_range(r2, r1, predicate()));
};
}
namespace {
range_extension<detail::intersect_t, true> intersect = {};
}

}

#endif
