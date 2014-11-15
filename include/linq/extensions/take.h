/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    take.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_TAKE_H
#define LINQ_GUARD_EXTENSIONS_TAKE_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct take_t
{
    template<class Range>
    auto operator()(Range && r, long count) const LINQ_RETURNS(boost::make_iterator_range(boost::begin(r), boost::next(boost::begin(r), count)));
};
}
namespace {
range_extension<detail::take_t> take = {};
}

}

#endif
