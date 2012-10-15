/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    traits.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_TRAITS_H
#define INCLUDE_GUARD_LINQ_TRAITS_H

#include <boost/type_traits.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/utility.hpp>
#include  <boost/range/has_range_iterator.hpp> 


namespace linq {

//
// is_iterator type trait
//
namespace detail {
BOOST_MPL_HAS_XXX_TRAIT_DEF(iterator_category)
}

template<class T, class Enabler = void>
struct is_iterator
: boost::mpl::false_
{};
template<class T>
struct is_iterator<T, BOOST_DEDUCED_TYPENAME boost::enable_if<detail::has_iterator_category<T> >::type >
: boost::mpl::true_
{};
template<class T>
struct is_iterator<T, BOOST_DEDUCED_TYPENAME boost::enable_if<boost::is_pointer<T> >::type >
: boost::mpl::true_
{};

//
// is_range type trait
//
template<class T>
struct is_range : boost::mpl::eval_if< boost::is_const<T>,
boost::has_range_const_iterator<BOOST_DEDUCED_TYPENAME boost::remove_const<T>::type>,
boost::mpl::and_<boost::has_range_iterator<T>, boost::has_range_const_iterator<T> > 
>::type
{};
template<class T, class U>
struct is_range<std::pair<T, U> > : boost::mpl::and_<is_iterator<T>, is_iterator<U>, boost::is_same<T, U> >::type
{};

}

#endif