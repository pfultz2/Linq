/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    group_by.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_GROUP_BY_H
#define LINQ_GUARD_EXTENSIONS_GROUP_BY_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/identity.h>
#include <linq/extensions/detail/make_map.h>
#include <linq/extensions/detail/placeholders.h>
#include <linq/extensions/select.h>
#include <linq/utility.h>
#include <boost/range.hpp>
#include <functional>
#include <map>

namespace linq { 

//
// group_by
//
namespace detail {

template<class T>
T&& assert_range(T && x)
{
    static_assert(is_range<T>::value, "Must be a range");
    return std::forward<T>(x);
}
struct group_by_t
{
    struct map_selector
    {
        template<class KeySelector, class ElementSelector, class T>
        auto operator()(KeySelector key_selector, ElementSelector element_selector, T && x) 
        LINQ_RETURNS(std::make_pair(key_selector(x), element_selector(x)));
    };

    template<class Range, class KeySelector>
    auto operator()(Range && r, KeySelector ks) const LINQ_RETURNS
    (
        make_map( assert_range( r | linq::select(std::bind(map_selector(), ks, identity_selector(), _1)) ) )
    );

    // TODO: Custom comparer overloads can't be supported right now, 
    // because we can't detect the difference between a comparer and 
    // a selector in msvc

    template<class Range, class KeySelector, class ElementSelector>
    auto operator()(Range && r, KeySelector ks, ElementSelector es) const LINQ_RETURNS
    (
        make_map( assert_range( r | linq::select(std::bind(map_selector(), ks, es, _1)) ) )
    );

};
}
namespace {
range_extension<detail::group_by_t> group_by = {};
}

}

#endif
