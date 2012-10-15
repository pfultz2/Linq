/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    extension.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_EXTENSION_H
#define LINQ_GUARD_EXTENSIONS_EXTENSION_H

#include <linq/pp.h>
#include <linq/utility.h>
#include <linq/traits.h>
#include <utility>
#include <functional>

namespace linq { 

// 
// Range extension
//
#ifndef LINQ_LIMIT_EXTENSION
#define LINQ_LIMIT_EXTENSION 4
#endif
namespace detail {
struct na {};

template<class F, BOOST_PP_ENUM_BINARY_PARAMS_Z(1, LINQ_LIMIT_EXTENSION, class T, = na BOOST_PP_INTERCEPT) >
struct pipe_closure {};

#define LINQ_PIPE_CLOSURE_MEMBERS_OP(z, n, data) T ## n x ## n;
#define LINQ_PIPE_CLOSURE_CONSTRUCTOR_OP(z, n, data) x ## n(std::forward<X ## n>(x ## n))
#define LINQ_PIPE_CLOSURE(z, n, data) \
template<class F, BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
struct pipe_closure<F, BOOST_PP_ENUM_PARAMS_Z(z, n, T), BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_SUB(LINQ_LIMIT_EXTENSION,n), na BOOST_PP_INTERCEPT)> \
{ \
    BOOST_PP_REPEAT_ ## z(n, LINQ_PIPE_CLOSURE_MEMBERS_OP, ~) \
    template<LINQ_PARAMS(n, class X)>\
    pipe_closure(LINQ_PARAMS(n, X, && BOOST_PP_INTERCEPT, x)) \
    : BOOST_PP_ENUM_ ## z(n, LINQ_PIPE_CLOSURE_CONSTRUCTOR_OP, ~) \
    {} \
    \
    template<class Range> \
    friend auto operator|(Range && r, pipe_closure p) LINQ_RETURN_REQUIRES(is_range<Range>) \
    (F()(std::forward<Range>(r), LINQ_FORWARD_PARAMS(n, T, p.x) )) \
\
};
BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_PIPE_CLOSURE, ~)
}
#define LINQ_RANGE_EXTENSION_OP(z, n, data) \
    template<LINQ_PARAMS(n, class T)> \
    auto operator()(LINQ_PARAMS(n, T, && x) ) const LINQ_RETURNS \
    ( \
        detail::pipe_closure<F, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, && BOOST_PP_INTERCEPT)> \
        (LINQ_FORWARD_PARAMS_Z(z, n, T, x) ) \
    ); 

template<class F>
struct range_extension
{
    BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_RANGE_EXTENSION_OP, ~)
    template<class Range>
    friend auto operator|(Range && r, range_extension) LINQ_RETURN_REQUIRES(is_range<Range>)
    (F()(std::forward<Range>(r)))

    range_extension<F>& operator()()
    {
        return *this;
    }

    const range_extension<F>& operator()() const
    {
        return *this;
    }
};

}

#endif
