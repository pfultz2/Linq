/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    take_while.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_TAKE_WHILE_H
#define LINQ_GUARD_EXTENSIONS_TAKE_WHILE_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <linq/extensions/detail/placeholders.h>
#include <linq/extensions/detail/not_predicate.h>

namespace linq { 
namespace detail {
struct take_while_t
{
    template<class Range, class Predicate>
    auto operator()(Range && r, Predicate p) const
    LINQ_RETURNS(boost::make_iterator_range(boost::begin(r), boost::find_if(r, std::bind(not_predicate(), p, linq::_1))));
};
}
namespace {
range_extension<detail::take_while_t> take_while = {};
}

}

#endif
