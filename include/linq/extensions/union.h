/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    union.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_UNION_H
#define LINQ_GUARD_EXTENSIONS_UNION_H

#include <linq/extensions/distinct.h>
#include <linq/extensions/concat.h>

namespace linq { 

namespace detail {
struct union_t
{
    template<class Range1, class Range2>
    auto operator()(Range1 && r1, Range2 && r2) const LINQ_RETURNS
    (r1 | linq::concat(r2) | linq::distinct);
};
}
namespace {
range_extension<detail::union_t> union_ = {};
}

}

#endif
