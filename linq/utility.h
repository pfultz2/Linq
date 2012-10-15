/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    utility.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_UTILITY_H
#define INCLUDE_GUARD_LINQ_UTILITY_H

#include <utility>
#include <boost/utility/enable_if.hpp>

//
// LINQ_RETURNS for auto return type deduction.
//
#define LINQ_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } static_assert(true, "")


#define LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) decltype(__VA_ARGS__)>::type { return __VA_ARGS__; }
#define LINQ_RETURN_REQUIRES(...) -> typename boost::enable_if<__VA_ARGS__, LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

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