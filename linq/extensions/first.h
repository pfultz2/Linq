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

    template<class Range, class Predicate, class Value>
    auto operator()(Range && r, Predicate p, Value && v) const LINQ_RETURNS
    (first_it(boost::begin(r), boost::end(r), p, std::forward<Value>(v)));

    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS(*(boost::begin(r)));

};
}
namespace {
range_extension<detail::first_t> first = {};
}

}

#endif
