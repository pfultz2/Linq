/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    last.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_LAST_H
#define LINQ_GUARD_EXTENSIONS_LAST_H

#include <linq/extensions/extension.h>
#include <linq/extensions/first.h>
#include <linq/extensions/reverse.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct last_t
{
    template<class Range, class Predicate, class Value>
    auto operator()(Range && r, Predicate p, Value && v) const LINQ_RETURNS
    (r | linq::reverse | linq::first(p, std::forward<Value>(v)));

    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS(*(--boost::end(r)));
};
}
namespace {
range_extension<detail::last_t, true> last = {};
}

}

#endif
