/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    to_container.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_TO_CONTAINER_H
#define LINQ_GUARD_EXTENSIONS_TO_CONTAINER_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>

namespace linq { 

//
// to_container
//
namespace detail {
struct to_container_t
{

    template<class Range>
    struct converter
    {
        Range r;

        template<class R>
        converter(R && x) : r(std::forward<R&&>(x))
        {}

        template<class C>
        operator C() const
        {
            return C(boost::begin(r), boost::end(r));
        }
    };

    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS
    (converter<Range&&>(std::forward<Range>(r)));
};
}
namespace {
range_extension<detail::to_container_t, true> to_container = {};
}

}

#endif
