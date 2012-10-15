/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    values.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_VALUES_H
#define LINQ_GUARD_EXTENSIONS_VALUES_H

#include <linq/extensions/extension.h>

namespace linq { 

namespace detail {
struct values_t
{
    struct value_selector
    {
        template<class T>
        auto operator()(T && x) LINQ_RETURNS(x.first);
    };

    template<class Range>
    auto operator()(Range && r) LINQ_RETURNS(r | linq::select(value_selector()));
};
}
namespace {
range_extension<detail::values_t> values = {};
}

}

#endif
