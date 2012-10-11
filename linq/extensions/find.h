/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    find.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_FIND_H
#define LINQ_GUARD_EXTENSIONS_FIND_H

#include <linq/extensions/extension.h>

namespace linq { 

//
// find
//
namespace detail {

// TODO: Add overload for string
template<class Range, class T>
auto find(Range && r, T && x) LINQ_RETURNS(std::find(boost::begin(r), boost::end(r), std::forward<T>(x)));

struct find_t
{
    template<class Range, class T>
    auto operator()(Range && r, T && x) 
    LINQ_RETURNS(find(std::forward<Range>(r), std::forward<T>(x)));
};
}
namespace {
range_extension<detail::find_t> find = {};
}

}

#endif
