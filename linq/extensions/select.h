/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    select.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SELECT_H
#define LINQ_GUARD_EXTENSIONS_SELECT_H

namespace linq { 

//
// select
//
namespace detail {
struct select_t
{
    //TODO: make it work for empty and single ranges

    template<class F, class It>
    static auto make_transform_iterator(F f, It it) LINQ_RETURNS(boost::transform_iterator<F, It>(it, f));

    template<class Range, class Selector>
    auto operator()(Range && r, Selector selector) 
    LINQ_RETURNS(boost::make_iterator_range(make_transform_iterator(selector, boost::begin(r)), make_transform_iterator(selector, boost::end(r))) );

};
}
namespace {
range_extension<detail::select_t> select = {};
}

}

#endif
