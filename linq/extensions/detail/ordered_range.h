/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    ordered_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_ORDERED_RANGE_H
#define LINQ_GUARD_DETAIL_ORDERED_RANGE_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/range.hpp>
#include <linq/utility.h>
#include <linq/traits.h>
#include <set>

namespace linq { 

template<class Iterator, class Compare>
struct ordered_range
{
    typedef std::multiset<typename boost::iterator_value<Iterator>::type, Compare> sorted_set;
    Compare c;
    Iterator first, last;
    sorted_set sorted;

    typedef typename sorted_set::iterator iterator;
    typedef typename sorted_set::const_iterator const_iterator;

    ordered_range(Iterator first, Iterator last)
    : first(first), last(last)
    {}

    ordered_range(Iterator first, Iterator last, Compare c)
    : first(first), last(last), c(c)
    {}

    void sort()
    {
        if (sorted.empty()) sorted = sorted_set(first, last, c);
    }

    iterator begin()
    {
        this->sort(); 
        return sorted.begin();
    }

    iterator end()
    {
        this->sort(); 
        return sorted.end();
    }

};

template<class Iterator, class Compare>
struct is_bindable_range<ordered_range<Iterator, Compare> >
: boost::mpl::bool_<true>
{};

template<class Iterator, class Compare>
ordered_range<Iterator, function_object<Compare> > make_ordered_range(Iterator first, Iterator last, Compare c)
{
    return ordered_range<Iterator, function_object<Compare> >(first, last, function_object<Compare>(c));
}

}

#endif
