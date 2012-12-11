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
#include <linq/extensions/detail/placeholders.h>
#include <linq/extensions/detail/defer.h>
#include <linq/extensions/detail/result_of.h>
#include <utility>
#include <functional>
#include <boost/static_assert.hpp>

namespace linq { 

// 
// Range extension
//
#ifndef LINQ_LIMIT_EXTENSION
#define LINQ_LIMIT_EXTENSION 6
#endif

#define LINQ_RANGE_EXTENSION_FORWARD(z, n, data) std::forward<T ## n>(x ## n)
#define LINQ_RANGE_EXTENSION_FORWARD_P(z, n, data) std::forward<T ## n>(p.x ## n)
#define LINQ_RANGE_EXTENSION_CONSTRUCT(z, n, data) x ## n(x ## n)
#define LINQ_RANGE_EXTENSION_MEMBER(z, n, data) T ## n x ## n;
namespace detail {
struct na {};

template<class F>
struct pipe_closure;

#define LINQ_RANGE_EXTENSION_PIPE_CLOSURE(z, n, data) \
template<class F, BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
struct pipe_closure<F(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
{ \
    BOOST_PP_REPEAT_ ## z(n, LINQ_RANGE_EXTENSION_MEMBER, ~) \
    pipe_closure(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, x)) \
    : BOOST_PP_ENUM_ ## z(n, LINQ_RANGE_EXTENSION_CONSTRUCT, ~) \
    {} \
    \
    template<class Range> \
    friend typename boost::lazy_enable_if<is_bindable_range<Range>, linq::result_of<F(Range&&, BOOST_PP_ENUM_PARAMS_Z(z, n, T))> >::type \
    operator|(Range&& r, pipe_closure && p) \
    { \
        return F()(std::forward<Range>(r), BOOST_PP_ENUM_ ## z(n, LINQ_RANGE_EXTENSION_FORWARD_P, ~)); \
    } \
};
BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_RANGE_EXTENSION_PIPE_CLOSURE, ~)

#define LINQ_RANGE_EXTENSION_MAKE_PIPE_CLOSURE(z, n, data) \
template<class F, BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
pipe_closure<F(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
make_pipe_closure(F, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, && x)) \
{ \
    return pipe_closure<F(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
    (BOOST_PP_ENUM_ ## z(n, LINQ_RANGE_EXTENSION_FORWARD, ~)); \
}
BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_RANGE_EXTENSION_MAKE_PIPE_CLOSURE, ~)

}

#define LINQ_RANGE_EXTENSION_OP(z, n, data) \
    template<BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
    auto operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, && x) ) const LINQ_RETURNS \
    ( \
        detail::make_pipe_closure \
        (F(), BOOST_PP_ENUM_ ## z(n, LINQ_RANGE_EXTENSION_FORWARD, ~) ) \
    ); 

template<class F, bool Unary=false>
struct range_extension
{
    BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_RANGE_EXTENSION_OP, ~)

    range_extension<F>& operator()()
    {
        return *this;
    }

    const range_extension<F>& operator()() const
    {
        return *this;
    }
};

template<class Range, class F>
typename boost::lazy_enable_if
<
    is_bindable_range<Range>, 
    linq::result_of<const F(Range&&)> 
>::type
operator|(Range && r, const range_extension<F, true>)
{
    return F()(std::forward<Range>(r));
};

}

#endif
