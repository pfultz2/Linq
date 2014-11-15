/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    any.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_ANY_H
#define LINQ_GUARD_EXTENSIONS_ANY_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/range.hpp>

namespace linq { 

//
// any
//
namespace detail {
struct any_t
{
    template<class Range, class Pred>
    auto operator()(Range && r) const LINQ_RETURNS(!boost::empty(r));

    template<class Range, class Pred>
    auto operator()(Range && r, Pred p) const LINQ_RETURNS(std::any_of(boost::begin(r), boost::end(r), linq::make_function_object(p)));
};
}
namespace {
range_extension<detail::any_t, true> any = {};
}

}

#endif
