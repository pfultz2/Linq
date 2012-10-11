/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    aggregate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_AGGREGATE_H
#define LINQ_GUARD_EXTENSIONS_AGGREGATE_H

#include <linq/extensions/extension.h>

namespace linq { 

//
// aggregate
//
namespace detail {
struct aggregate_t
{
    //TODO: make it work for empty and single ranges
    template<class Range, class Reducer>
    auto operator()(Range && r, Reducer reducer) LINQ_RETURNS(std::accumulate(++boost::begin(r), boost::end(r), *boost::begin(r)));

    template<class Range, class Seed, class Reducer>
    auto operator()(Range && r, Seed && s, Reducer reducer) LINQ_RETURNS(std::accumulate(boost::begin(r), boost::end(r), s, reducer));

    template<class Range, class Seed, class Reducer>
    auto operator()(Range && r, Seed && s, Reducer reducer, Selector sel) LINQ_RETURNS(sel(std::accumulate(boost::begin(r), boost::end(r), s, reducer)));
};
}
namespace {
range_extension<detail::aggregate_t> aggregate = {};
}

}

#endif
