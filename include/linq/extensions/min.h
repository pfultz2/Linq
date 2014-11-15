/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    min.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_MIN_H
#define LINQ_GUARD_EXTENSIONS_MIN_H

#include <linq/extensions/extension.h>
#include <linq/extensions/aggregate.h>
#include <linq/extensions/detail/defer.h>

namespace linq { 
namespace detail {

struct min_reducer
{
    template<class T>
    T operator()(T x, T y) const
    {
        return (x < y) ? x : y;
    }
};

struct min_t
{
    template<class>
    struct result;

    template<class X, class Range>
    struct result<X(Range)>
    : linq::result_of<aggregate_t(Range, defer<min_reducer>)>
    {};

    template<class Range>
    typename result<min_t(Range&&)>::type operator()(Range && r) const
    {
        return r | linq::aggregate(defer<min_reducer>());
    };
};
}
namespace {
range_extension<detail::min_t, true> min = {};
}

}

#endif
