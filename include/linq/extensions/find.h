/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    find.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_FIND_H
#define LINQ_GUARD_EXTENSIONS_FIND_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>


namespace linq { 

template<class R, class Enable = void>
struct finder
{
    template<class Range, class T>
    static auto call(Range && r, T && x) LINQ_RETURNS(std::find(boost::begin(r), boost::end(r), std::forward<T>(x)));
};

template<class R>
struct finder<R, typename boost::enable_if<is_keyed_range<R> >::type>
{
    template<class Range, class T>
    static auto call(Range && r, T && x) LINQ_RETURNS(r.find(std::forward<T>(x)));
};

template<class Char, class Traits, class Allocator>
struct finder<std::basic_string<Char, Traits, Allocator> >
{
    typedef typename std::basic_string<Char, Traits, Allocator>::const_iterator const_iterator;
    typedef typename std::basic_string<Char, Traits, Allocator>::iterator iterator;
    static const typename std::basic_string<Char, Traits, Allocator>::size_type npos = std::basic_string<Char, Traits, Allocator>::npos;

    template<class T>
    static const_iterator call(const std::basic_string<Char, Traits, Allocator>& s, T && x)
    {
        auto i = s.find(std::forward<T>(x));
        if (i == npos) return s.end();
        else return s.begin() + i;
    }

    template<class T>
    static iterator call(std::basic_string<Char, Traits, Allocator>& s, T && x)
    {
        auto i = s.find(std::forward<T>(x));
        if (i == npos) return s.end();
        else return s.begin() + i;
    }
};


//
// find
//
namespace detail {

struct find_t
{
    template<class Range, class T>
    auto operator()(Range && r, T && x) const
    LINQ_RETURNS(finder<typename std::decay<Range>::type>::call(std::forward<Range>(r), std::forward<T>(x)));
};
}
namespace {
range_extension<detail::find_t> find = {};
}

}

#endif
