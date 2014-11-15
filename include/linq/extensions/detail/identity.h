/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_IDENTITY_H
#define LINQ_GUARD_DETAIL_IDENTITY_H

#include <linq/utility.h>
#include <utility>

namespace linq { 

namespace detail {

struct identity_selector
{
    template<class T>
    auto operator()(T&& x) const LINQ_RETURNS(std::forward<T>(x));
};
}
}

#endif
