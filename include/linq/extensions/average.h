/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    average.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_AVERAGE_H
#define LINQ_GUARD_EXTENSIONS_AVERAGE_H

#include <linq/extensions/extension.h>
#include <linq/extensions/aggregate.h>
#include <linq/extensions/sum.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct average_t
{

    template<class Range>
    double operator()(Range&& r) const
    {
    	return (1.0 * (r | linq::sum) / boost::distance(r));
	}
};
}
namespace {
range_extension<detail::average_t, true> average = {};
}

}

#endif
