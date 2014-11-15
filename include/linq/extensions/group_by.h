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

template<class KeySelector, class ElementSelector>
struct group_by_map_selector
{
    KeySelector key_selector;
    ElementSelector element_selector;

    group_by_map_selector(KeySelector key_selector, ElementSelector element_selector)
    : key_selector(key_selector), element_selector(element_selector)
    {}

    template<class>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef std::pair<typename linq::result_of<KeySelector(T)>::type, typename linq::result_of<ElementSelector(T)>::type > type;
    };

    template<class F, class T>
    struct result<F(T&&)>
    : result<F(T)>
    {};

    template<class T>
    typename result<group_by_map_selector(T&&)>::type operator()(T && x) const
    {
        return typename result<group_by_map_selector(T&&)>::type(key_selector(x), element_selector(x));
    }
};

template<class KeySelector, class ElementSelector>
group_by_map_selector < KeySelector, ElementSelector >
make_group_by_map_selector (KeySelector && key_selector, ElementSelector && element_selector)
{
    return group_by_map_selector < KeySelector, ElementSelector >
    (std::forward<KeySelector>(key_selector), std::forward<ElementSelector>(element_selector));
}

struct group_by_t
{
    template<class>
    struct result;

    template<class F, class Range, class KeySelector, class ElementSelector>
    struct result<F(Range, KeySelector, ElementSelector)>
    {
        typedef typename as_unordered_map<typename result_of<detail::select_t(Range, group_by_map_selector<KeySelector, ElementSelector>)>::type >::type type;
    };

    template<class F, class Range, class KeySelector, class ElementSelector>
    struct result<F(Range&&, KeySelector, ElementSelector)>
    : result<F(Range, KeySelector, ElementSelector)>
    {};

    template<class F, class Range, class KeySelector>
    struct result<F(Range, KeySelector)>
    : result<F(Range, KeySelector, identity_selector)>
    {};

    template<class Range, class KeySelector>
    typename result<group_by_t(Range&&, KeySelector)>::type operator()(Range && r, KeySelector ks) const
    {
        return make_map( r | linq::select(make_group_by_map_selector(ks, identity_selector())) );
    };

    // TODO: Custom comparer overloads can't be supported right now, 
    // because we can't detect the difference between a comparer and 
    // a selector in msvc

    template<class Range, class KeySelector, class ElementSelector>
    typename result<group_by_t(Range&&, KeySelector, ElementSelector)>::type operator()(Range && r, KeySelector ks, ElementSelector es) const
    {
        return make_map( r | linq::select(make_group_by_map_selector(ks, es)) );
    };

};
}
namespace {
range_extension<detail::group_by_t> group_by = {};
}

}

#endif
