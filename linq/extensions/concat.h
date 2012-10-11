/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    concat.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_CONCAT_H
#define LINQ_GUARD_EXTENSIONS_CONCAT_H

namespace linq { 

//
// concat
//
namespace detail {
struct concat_t
{
    template<class Range1, class Range2>
    auto operator()(Range1 && r1, Range2 && r2) LINQ_RETURNS(boost::join(r1, r2));
};
}
namespace {
range_extension<detail::concat_t> concat = {};
}

}

#endif
