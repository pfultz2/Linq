/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_RESULT_OF_H
#define LINQ_GUARD_DETAIL_RESULT_OF_H

#include <linq/utility.h>
#include <linq/pp.h>
#include <utility>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/eval_if.hpp>

#ifndef LINQ_RESULT_OF_LIMIT
#define LINQ_RESULT_OF_LIMIT 8
#endif

#ifndef LINQ_NO_STD_RESULT_OF
#ifdef _MSC_VER
#define LINQ_NO_STD_RESULT_OF
#endif
#endif

namespace linq { 

#ifdef LINQ_NO_STD_RESULT_OF
namespace detail {

template<class F>
struct auto_result_of;

#define LINQ_AUTO_RESULT_OF_M(z, n, data) linq::declval<T ## n>()
#define LINQ_AUTO_RESULT_OF_EACH(z, n, data) \
template<class X BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
struct auto_result_of<X(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
{ \
    typedef decltype(linq::declval<X>()(BOOST_PP_ENUM_ ## z(n, LINQ_AUTO_RESULT_OF_M, ~) )) type;\
}; 

BOOST_PP_REPEAT_1(LINQ_RESULT_OF_LIMIT, LINQ_AUTO_RESULT_OF_EACH, ~)

BOOST_MPL_HAS_XXX_TEMPLATE_DEF(result)


template<bool HasResult, class F, class Sig>
struct result_of_impl
: F::template result<Sig>
{};

template<class F, class Sig>
struct result_of_impl<false, F, Sig>
: auto_result_of<Sig>
{};

}


template<class F>
struct result_of;

#define LINQ_RESULT_OF_EACH(z, n, data) \
template<class F BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
struct result_of<F(BOOST_PP_ENUM_PARAMS_Z(z, n, T))> \
: detail::result_of_impl<detail::has_result<F>::value, F, F(BOOST_PP_ENUM_PARAMS_Z(z, n, T)) > \
{};  

BOOST_PP_REPEAT_1(LINQ_RESULT_OF_LIMIT, LINQ_RESULT_OF_EACH, ~)

#else

template<class F>
struct result_of
: std::result_of<F>
{};

#endif

}

#endif
