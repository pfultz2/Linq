/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    last_or_default.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_LAST_OR_DEFAULT_H
#define LINQ_GUARD_EXTENSIONS_LAST_OR_DEFAULT_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/always.h>
#include <linq/extensions/first_or_default.h>
#include <linq/extensions/reverse.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {
struct last_or_default_t
{
	template<class>
    struct result;

    template<class X, class Range>
    struct result<X(Range)>
    : boost::range_value<typename boost::decay<Range>::type>
    {};

    template<class X, class Range, class Predicate>
    struct result<X(Range, Predicate)>
    : boost::range_value<typename boost::decay<Range>::type>
    {};

    template<class Range>
    typename result<last_or_default_t(Range&&)>::type operator()(Range && r) const 
    {
        return r | linq::reverse | linq::first_or_default;
    };

    template<class Range, class Predicate>
    typename result<last_or_default_t(Range&&, Predicate)>::type operator()(Range && r, Predicate p) const
    {
        return r | linq::reverse | linq::first_or_default(p);
    };
};
}
namespace {
range_extension<detail::last_or_default_t, true> last_or_default = {};
}

}

#endif
