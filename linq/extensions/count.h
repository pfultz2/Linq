/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    count.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_COUNT_H
#define LINQ_GUARD_EXTENSIONS_COUNT_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/always.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/range.hpp>

namespace linq { 

//
// count
//
namespace detail {
struct count_t
{
    // TODO: add overload for random access ranges
    template<class Range>
    long operator()(Range && r)
    {
        return std::count_if(boost::begin(r), boost::end(r), detail::always());
    }

    template<class Range, class Pred>
    long operator()(Range && r, Pred p)
    {
        return std::count_if(boost::begin(r), boost::end(r), linq::make_function_object(p));
    }
};
}
namespace {
range_extension<detail::count_t> count = {};
}

}

#endif
