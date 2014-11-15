/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    simple_zip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_SIMPLE_ZIP_H
#define LINQ_GUARD_DETAIL_SIMPLE_ZIP_H

#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 

template<class Range1, class Range2>
auto simple_zip(Range1 && r1, Range2 && r2) LINQ_RETURNS
(
    boost::make_iterator_range
    (
        boost::make_zip_iterator(boost::make_tuple(boost::begin(r1), boost::begin(r2))),
        boost::make_zip_iterator(boost::make_tuple(boost::end(r1), boost::end(r2)))
    )
);

}

#endif
