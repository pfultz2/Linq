/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    single_or_default.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SINGLE_OR_DEFAULT_H
#define LINQ_GUARD_EXTENSIONS_SINGLE_OR_DEFAULT_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/is_single.h>
#include <exception>

namespace linq { 
namespace detail {
struct single_or_default_t
{
	template<class>
    struct result;

    template<class F, class Range, class T>
    struct result<F(Range, T)>
    : boost::range_value<typename std::decay<Range>::type>
    {};

    template<class Range, class T>
    typename result<single_or_default_t(Range&&, T&&)>::type operator()(Range && r, T && x) const
    {
        return is_single(r) ? *boost::begin(r) : x;
    };
};
}
namespace {
range_extension<detail::single_or_default_t> single_or_default = {};
}

}

#endif
