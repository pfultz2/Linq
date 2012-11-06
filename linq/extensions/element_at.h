/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    element_at.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_ELEMENT_AT_H
#define LINQ_GUARD_EXTENSIONS_ELEMENT_AT_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>


namespace linq { 

//
// element_at
//
namespace detail {
struct element_at_t
{
    template<class Iterator>
    static Iterator advance_it(Iterator it, std::size_t n)
    {
        std::advance(it, n);
        return it;
    }
    // TODO: Throw when its out of range
    // TODO: Add overload to provide a fallback value when its out of range
    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS(*(advance_it(boost::begin(r))));
};
}
namespace {
range_extension<detail::element_at_t> element_at = {};
}

}

#endif
