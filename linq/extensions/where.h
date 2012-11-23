/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    where.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_WHERE_H
#define LINQ_GUARD_EXTENSIONS_WHERE_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 

namespace detail {
struct where_t
{
    template<class Range, class Predicate>
    auto operator()(Range && r, Predicate p) const LINQ_RETURNS
    (
        boost::make_iterator_range
        (
            boost::make_filter_iterator(make_function_object(p), boost::begin(r), boost::end(r)), 
            boost::make_filter_iterator(make_function_object(p), boost::end(r), boost::end(r))
        )
    );
};
}
namespace {
range_extension<detail::where_t> where = {};
}

}

#endif
