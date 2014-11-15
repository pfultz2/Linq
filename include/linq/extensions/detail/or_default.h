/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    or_default.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_OR_DEFAULT_H
#define LINQ_GUARD_DETAIL_OR_DEFAULT_H

namespace linq { 

//
// or_default
//
namespace detail {

template<class Iterator, class Value>
auto or_default(Iterator it, Iterator last, Value && v) LINQ_RETURNS
((it != last) ? *it : std::forward<Value>(v)); 
}

}

#endif
