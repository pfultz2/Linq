/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    empty_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_EMPTY_RANGE_H
#define LINQ_GUARD_EXTENSIONS_EMPTY_RANGE_H

#include <linq/extensions/extension.h>

namespace linq { 

template<class T>
auto empty_range() LINQ_RETURNS(boost::iterator_range<T*>(0, 0));

}

#endif
