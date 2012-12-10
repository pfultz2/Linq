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
#include <linq/extensions/detail/defer.h>
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
struct group_by_t
{
    template<class KeySelector, class ElementSelector>
    struct map_selector
    {
        KeySelector key_selector;
        ElementSelector element_selector;

        map_selector(KeySelector key_selector, ElementSelector element_selector)
        : key_selector(key_selector), element_selector(element_selector)
        {}

        template<class T>
        auto operator()(T && x) const -> decltype(std::make_pair(declval<KeySelector>()(x), declval<ElementSelector>()(x)))
        {
            return std::make_pair(key_selector(x), element_selector(x));
        }
    };

    template<class KeySelector, class ElementSelector>
    static map_selector < KeySelector, ElementSelector >
    make_map_selector (KeySelector && key_selector, ElementSelector && element_selector)
    {
        return map_selector < KeySelector, ElementSelector >
        (std::forward<KeySelector>(key_selector), std::forward<ElementSelector>(element_selector));
    }


    template<class Range, class KeySelector>
    auto operator()(Range && r, KeySelector ks) const LINQ_RETURNS
    (
        make_map( r | linq::select(make_map_selector(ks, identity_selector())) )
    );

    // TODO: Custom comparer overloads can't be supported right now, 
    // because we can't detect the difference between a comparer and 
    // a selector in msvc

    template<class Range, class KeySelector, class ElementSelector>
    auto operator()(Range && r, KeySelector ks, ElementSelector es) const LINQ_RETURNS
    (
        make_map( r | linq::select(make_map_selector(ks, es)) )
    );

};
}
namespace {
range_extension<detail::group_by_t> group_by = {};
}

}

#endif
