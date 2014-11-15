/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    reverse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_REVERSE_H
#define LINQ_GUARD_EXTENSIONS_REVERSE_H

#include <linq/extensions/extension.h>
#include <linq/utility.h>
#include <boost/range.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace linq { 

namespace detail {
struct reverse_t
{
    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS(boost::adaptors::reverse(r));
};
}
namespace {
range_extension<detail::reverse_t, true> reverse = {};
}

}

#endif
