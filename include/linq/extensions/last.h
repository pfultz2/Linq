/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    last.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_LAST_H
#define LINQ_GUARD_EXTENSIONS_LAST_H

#include <linq/extensions/extension.h>
#include <linq/extensions/first.h>
#include <linq/extensions/reverse.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct last_t
{
	template<class>
    struct result;

    template<class F, class Range>
    struct result<F(Range)>
    : boost::range_reference<typename std::decay<Range>::type>
    {};

    template<class F, class Range, class Predicate>
    struct result<F(Range, Predicate)>
    : boost::range_reference<typename std::decay<Range>::type>
    {};

    template<class Range, class Predicate>
    typename result<last_t(Range&&, Predicate)>::type operator()(Range && r, Predicate p) const
    {
    	return r | linq::reverse | linq::first(p);
    };

    template<class Range>
    typename result<last_t(Range&&)>::type operator()(Range && r) const
    {
        if (boost::empty(r)) throw std::out_of_range("linq::last failed");
    	return *(--boost::end(r));
    };
};
}
namespace {
range_extension<detail::last_t, true> last = {};
}

}

#endif
