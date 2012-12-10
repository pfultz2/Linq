/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_RESULT_OF_H
#define LINQ_GUARD_DETAIL_RESULT_OF_H

#include <linq/utility.h>
#include <utility>

namespace linq { namespace detail {

template<class T>
struct unwrap
{
    typedef T type;
};

template<class T>
struct unwrap<std::reference_wrapper<T> >
{
    typedef T& type;
};

}

template<class F>
struct result_of;

template<class F, class T>
struct result_of<F(T)>
{
    typedef typename detail::unwrap<decltype(linq::declval<F>()(linq::declval<T>()))>::type type;
};

template<class F, class T, class U>
struct result_of<F(T, U)>
{
    typedef typename detail::unwrap<decltype(linq::declval<F>()(linq::declval<T>(), linq::declval<U>()))>::type type;
};

}

#endif
