/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    skip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SKIP_H
#define LINQ_GUARD_EXTENSIONS_SKIP_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct skip_t
{
    template<class Range>
    auto operator()(Range && r, long count) const LINQ_RETURNS(boost::make_iterator_range(boost::next(boost::begin(r), count), boost::end(r)));
};
}
namespace {
range_extension<detail::skip_t> skip = {};
}

}

#endif
