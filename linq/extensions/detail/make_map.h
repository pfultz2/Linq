/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    make_map.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_MAKE_MAP_H
#define LINQ_GUARD_DETAIL_MAKE_MAP_H

#include <linq/utility.h>
#include <linq/traits.h>
#include <boost/range.hpp>
#include <boost/unordered_map.hpp>

namespace linq { 

namespace detail {

template<class Range>
auto make_map(Range && r) LINQ_RETURN_REQUIRES(is_range<Range>)
(boost::unordered_multimap<decltype(boost::begin(r)->first), decltype(boost::begin(r)->second)>(boost::begin(r), boost::end(r)));

template<class Range, class Compare>
auto make_map(Range && r, Compare c) LINQ_RETURN_REQUIRES(is_range<Range>)
(std::multimap<decltype(boost::begin(r)->first), decltype(boost::begin(r)->second), Compare>(boost::begin(r), boost::end(r), c));

}

}

#endif
