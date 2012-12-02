/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    single.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SINGLE_H
#define LINQ_GUARD_EXTENSIONS_SINGLE_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/is_single.h>
#include <exception>

namespace linq { 
namespace detail {
struct single_t
{

    static std::out_of_range single_throw()
    {
        return std::out_of_range("Is not a single range");
    }

    template<class Range>
    auto operator()(Range && r) const LINQ_RETURNS
    (
        is_single(r) ? *boost::begin(r) : throw single_throw()
    );
};
}
namespace {
range_extension<detail::single_t> single = {};
}

}

#endif
