/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    order_by.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_ORDER_BY_H
#define LINQ_GUARD_EXTENSIONS_ORDER_BY_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/defer.h>
#include <linq/extensions/detail/ordered_range.h>
#include <linq/extensions/detail/placeholders.h>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 
namespace detail {
struct order_selector
{
    template<class Selector, class T>
    bool operator()(Selector&& s, const T& x, const T& y) const
    {
        return s(x) < s(y);
    }
};

struct order_by_t
{
    template<class Range, class Selector>
    auto operator()(Range&& r, Selector s) const LINQ_RETURNS
    (make_ordered_range(boost::begin(r), boost::end(r), std::bind(defer<order_selector>(), s, linq::_1, linq::_2)));
};
}
namespace {
range_extension<detail::order_by_t> order_by = {};
}

}

#endif
