/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    is_single.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_IS_SINGLE_H
#define LINQ_GUARD_DETAIL_IS_SINGLE_H

#include <boost/range.hpp>

namespace linq { 
namespace detail {

template<class Range>
bool is_single(const Range & r)
{
    return !boost::empty(r) &&
    ++boost::begin(r) == boost::end(r);
}

}

}

#endif
