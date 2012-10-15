/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    keys.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_KEYS_H
#define LINQ_GUARD_EXTENSIONS_KEYS_H

#include <linq/extensions/extension.h>
#include <linq/extensions/select.h>
#include <linq/utility.h>

namespace linq { 
namespace detail {
struct keys_t
{
    struct key_selector
    {
        template<class T>
        auto operator()(T && x) LINQ_RETURNS(x.first);
    };

    template<class Range>
    auto operator()(Range && r) LINQ_RETURNS(r | linq::select(key_selector()));

};
}
namespace {
range_extension<detail::keys_t> keys = {};
}

}

#endif
