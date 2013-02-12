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
    template<class Iterator, class Predicate, class Value>
    static typename boost::iterator_value<Iterator>::type first_it(Iterator first, Iterator last, Predicate p, Value && v)
    {
        auto it = std::find_if(first, last, p);
        if (it == last) return v;
        else return *it;
    }

    template<class>
    struct result;

    template<class F, class Range, class Predicate, class Value>
    struct result<F(Range, Predicate, Value)>
    : boost::range_value<typename std::decay<Range>::type>
    {};

    template<class F, class Range>
    struct result<F(Range)>
    : boost::range_reference<typename std::decay<Range>::type>
    {};

    template<class Range, class Predicate, class Value>
    typename result<first_t(Range&&, Predicate, Value&&)>::type operator()(Range && r, Predicate p, Value && v) const
    {
        return first_it(boost::begin(r), boost::end(r), p, std::forward<Value>(v));
    };

    template<class Range>
    typename result<first_t(Range&&)>::type operator()(Range && r) const 
    {
        return *(boost::begin(r));
    };

};
}
namespace {
range_extension<detail::first_t, true> first = {};
}

}

#endif
