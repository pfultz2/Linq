/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    select.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SELECT_H
#define LINQ_GUARD_EXTENSIONS_SELECT_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>
#include <linq/utility.h>

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

    template<class>
    struct result;

    template<class F, class Range, class Selector>
    struct result<F(Range, Selector)>
    {
        typedef typename boost::range_iterator<typename std::remove_reference<Range>::type>::type iterator;
        typedef function_object<typename std::decay<Selector>::type> fun;
        typedef boost::iterator_range<boost::transform_iterator<fun, iterator> > type;
    };

    template<class Range, class Selector>
    typename result<select_t(Range&&, Selector)>::type operator()(Range && r, Selector selector) const
    {
        return boost::make_iterator_range
        (
            make_transform_iterator(make_function_object(selector), boost::begin(r)), 
            make_transform_iterator(make_function_object(selector), boost::end(r))
        ); 
    }

};
}
namespace {
range_extension<detail::select_t> select = {};
}

}

#endif
