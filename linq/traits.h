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

template<class T1 = void, class T2 = void, class T3 = void, class T4 = void, class T5 = void>
struct holder
{
    typedef void type;
};

}

template<class T, class X = void>
struct is_iterator 
: boost::mpl::bool_<false>
{};

template<class T>
struct is_iterator<T*>
: boost::mpl::bool_<true>
{};

template<class Iterator>
struct is_iterator<Iterator, typename linq::detail::holder
<
    typename Iterator::iterator_category, 
    typename Iterator::reference, 
    typename Iterator::pointer, 
    typename Iterator::value_type, 
    typename Iterator::difference_type 
>::type >
: boost::mpl::bool_<true>
{};

//
// is_range type trait
//
template<class T>
struct is_range : boost::mpl::if_
< 
    boost::is_const<T>,
    boost::has_range_const_iterator<typename boost::remove_const<T>::type>,
    boost::mpl::and_<boost::has_range_iterator<T>, boost::has_range_const_iterator<T> > 
>::type
{};
template<class T, class U>
struct is_range<std::pair<T, U> > : boost::mpl::and_<is_iterator<T>, is_iterator<U>, boost::is_same<T, U> >::type
{};

template<class T>
struct is_range<T&>
: is_range<T>
{};

template<class T>
struct is_range<T&&>
: is_range<T>
{};

}

#endif