/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    utility.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_UTILITY_H
#define INCLUDE_GUARD_LINQ_UTILITY_H

#include <utility>

namespace linq {

// MSVC 2010 doesn't provide declval
// We also return T&& instead std::add_rvalue_reference<T>
// because MSVC has a buggy implementation of it.
// So, this function will work in all cases except when T
// is void(which should rarely happen).
template <typename T>
T&& declval(); // no definition required

}

#endif