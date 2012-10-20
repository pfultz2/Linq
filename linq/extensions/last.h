/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    last.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_LAST_H
#define LINQ_GUARD_EXTENSIONS_LAST_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct last_t
{
    template<class Iterator, class Predicate, class Value>
    static typename boost::iterator_value<Iterator>::type last_it(Iterator first, Iterator last, Predicate p, Value && v)
    {
        typedef std::reverse_iterator<Iterator> iterator;
        auto rlast = iterator(last);
        auto it = std::find_if(iterator(first), rlast, p);
        if (it == rlast) return v;
        else return *it;
    }

    template<class Range, class Predicate, class Value>
    auto operator()(Range && r, Predicate p, Value && v) LINQ_RETURNS
    (last_it(boost::begin(r), boost::end(r), p, std::forward<Value>(v)));

    template<class Range>
    auto operator()(Range && r) LINQ_RETURNS(*(--boost::end(r)));
};
}
namespace {
range_extension<detail::last_t> last = {};
}

}

#endif
