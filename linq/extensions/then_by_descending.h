/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    then_by_descending.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_THEN_BY_DESCENDING_H
#define LINQ_GUARD_EXTENSIONS_THEN_BY_DESCENDING_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/ordered_range.h>
#include <linq/extensions/detail/placeholders.h>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 
namespace detail {

struct then_selector
{
    template<class Selector, class Compare, class T>
    bool operator()(Selector&& s, Compare&& c, const T& x, const T& y) const
    {
        bool first = c(x, y);
        return (first || c(y, x)) ? first : s(x) > s(y);
    }
};

struct then_by_descending_t
{
    template<class Range, class Selector>
    auto operator()(Range&& r, Selector s) const LINQ_RETURNS
    (make_ordered_range(boost::begin(r), boost::end(r), std::bind(then_selector(), s, r.c, _1, _2)));
};
}
namespace {
range_extension<detail::then_by_descending_t> then_by_descending = {};
}

}

#endif
