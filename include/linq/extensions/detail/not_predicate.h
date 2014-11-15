/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    not_predicate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_NOT_PREDICATE_H
#define LINQ_GUARD_DETAIL_NOT_PREDICATE_H

#include <linq/utility.h>

namespace linq { namespace detail {

struct not_predicate
{
    template<class P, class T>
    auto operator()(P p, T && x) const LINQ_RETURNS(!p(x));
};

}}

#endif
