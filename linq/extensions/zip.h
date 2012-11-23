/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    zip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_ZIP_H
#define LINQ_GUARD_EXTENSIONS_ZIP_H

#include <linq/extensions/extension.h>
#include <linq/extensions/select.h>
#include <linq/extensions/detail/simple_zip.h>
#include <boost/fusion/include/make_fused.hpp>

namespace linq { 
namespace detail {
struct zip_t
{
    template<class Range1, class Range2>
    auto operator()(Range1 && r1, Range2 && r2) const LINQ_RETURNS
    (simple_zip(r1, r2));

    template<class Range1, class Range2, class Selector>
    auto operator()(Range1 && r1, Range2 && r2, Selector s) const LINQ_RETURNS
    (simple_zip(r1, r2) | linq::select(boost::make_fused(s)));
};
}
namespace {
range_extension<detail::zip_t> zip = {};
}

}

#endif
