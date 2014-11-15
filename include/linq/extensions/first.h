/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    first.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_FIRST_H
#define LINQ_GUARD_EXTENSIONS_FIRST_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>

namespace linq { 

//
// first
//
namespace detail {
struct first_t
{
    template<class>
    struct result;

    template<class F, class Range>
    struct result<F(Range)>
    : boost::range_reference<typename std::decay<Range>::type>
    {};

    template<class F, class Range, class Predicate>
    struct result<F(Range, Predicate)>
    : boost::range_reference<typename std::decay<Range>::type>
    {};

    template<class Range, class Predicate>
    typename result<first_t(Range&&, Predicate)>::type operator()(Range && r, Predicate p) const
    {
        auto it = std::find_if(boost::begin(r), boost::end(r), p);
        if (it == boost::end(r)) throw std::out_of_range("linq::first failed");
        return *it;
    };

    template<class Range>
    typename result<first_t(Range&&)>::type operator()(Range && r) const 
    {
        if (boost::empty(r)) throw std::out_of_range("linq::first failed");
        return *(boost::begin(r));
    };

};
}
namespace {
range_extension<detail::first_t, true> first = {};
}

}

#endif
