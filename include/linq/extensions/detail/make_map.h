/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    make_map.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_MAKE_MAP_H
#define LINQ_GUARD_DETAIL_MAKE_MAP_H

#include <linq/utility.h>
#include <linq/traits.h>
#include <boost/range.hpp>
#include <boost/unordered_map.hpp>
#include <memory>

namespace linq { 

namespace detail {

template<class R>
struct as_unordered_map
{
    typedef typename boost::range_value<typename std::decay<R>::type>::type value_type;
    typedef boost::unordered_multimap<typename value_type::first_type, typename value_type::second_type> type;
};

template<class R>
struct as_shared_map
{
    typedef std::shared_ptr<typename as_unordered_map<R>::type> type;
};

template<class R, class Compare>
struct as_map
{
    typedef typename boost::range_value<typename std::decay<R>::type>::type value_type;
    typedef std::multimap<typename value_type::first_type, typename value_type::second_type, Compare, std::allocator<std::pair<typename value_type::first_type, typename value_type::second_type> > > type;
};


template<class Range>
typename as_unordered_map<Range>::type make_map(Range && r)
{
    return typename as_unordered_map<Range>::type(boost::begin(r), boost::end(r));
}
// (boost::unordered_multimap<decltype(boost::begin(r)->first), decltype(boost::begin(r)->second)>(boost::begin(r), boost::end(r)));

template<class Range>
std::shared_ptr<typename as_unordered_map<Range>::type> make_shared_map(Range && r)
{
    return std::make_shared<typename as_unordered_map<Range>::type>(boost::begin(r), boost::end(r));
}

// template<class Range>
// auto make_shared_map(Range && r) LINQ_RETURN_REQUIRES(is_range<Range>)
// (std::make_shared<boost::unordered_multimap<decltype(boost::begin(r)->first), decltype(boost::begin(r)->second)> >(boost::begin(r), boost::end(r)));

template<class Range, class Compare>
typename as_map<Range, Compare>::type make_map(Range && r, Compare c)
{
    return typename as_map<Range, Compare>::type(boost::begin(r), boost::end(r), c);
}

// template<class Range, class Compare>
// auto make_map(Range && r, Compare c) LINQ_RETURN_REQUIRES(is_range<Range>)
// (std::multimap<decltype(boost::begin(r)->first), decltype(boost::begin(r)->second), Compare>(boost::begin(r), boost::end(r), c));

}

}

#endif
