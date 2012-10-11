/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    group_by.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_GROUP_BY_H
#define LINQ_GUARD_EXTENSIONS_GROUP_BY_H

namespace linq { 

//
// group_by
//
namespace detail {
struct group_by_t
{

    template<class Range>
    static auto make_map(Range && r) LINQ_RETURNS
    (std::multimap<decltype(boost::begin(r)->first), boost::begin(r)->second>(boost::begin(r), boost::end(r)));

    template<class Range, class Compare>
    static auto make_map(Range && r, Compare c) LINQ_RETURNS
    (std::multimap<decltype(boost::begin(r)->first), boost::begin(r)->second, Compare>(boost::begin(r), boost::end(r), c));

    struct identity_selector
    {
        template<class T>
        auto operator()(T&& x) LINQ_RETURNS(std::forward<T>(x));
    };

    template<class KeySelector, class ElementSelector = identity_selector>
    struct map_selector
    {
        KeySelector key_selector;
        ElementSelector element_selector;

        map_selector(KeySelector ks, ElementSelector es = ElementSelector()) : key_selector(ks), element_selector(es)
        {}

        template<class T>
        auto operator()(T && x) LINQ_RETURNS(std::make_pair(key_selector(x), element_selector(x)));
    };

    template<class KeySelector>
    static auto make_map_selector(KeySelector ks) LINQ_RETURNS(map_selector<KeySelector>(ks));

    template<class KeySelector, class ElementSelector>
    static auto make_map_selector(KeySelector ks, ElementSelector es) LINQ_RETURNS(map_selector<KeySelector>(ks, es));

    template<class Range, class KeySelector>
    auto operator()(Range && r, KeySelector ks) LINQ_RETURNS
    (make_map(r | linq::select(make_map_selector(ks))));

    // TODO: Custom comparer overloads can't be supported right now, 
    // because we can't detect the difference between a comparer and 
    // a selector in msvc

    // template<class Range, class KeySelector, class KeyCompare>
    // auto operator()(Range && r, KeySelector ks, KeyCompare kc) LINQ_RETURNS
    // (make_map(r | linq::select(make_map_selector(ks)), kc));

    template<class Range, class KeySelector, class ElementSelector>
    auto operator()(Range && r, KeySelector ks, ElementSelector es) LINQ_RETURNS
    (make_map(r | linq::select(make_map_selector(ks, es))));

    // template<class Range, class KeySelector, class ElementSelector, class KeyCompare>
    // auto operator()(Range && r, KeySelector ks,ElementSelector es,  KeyCompare kc) LINQ_RETURNS
    // (make_map(r | linq::select(make_map_selector(ks, es)), kc));
};
}
namespace {
range_extension<detail::group_by_t> group_by = {};
}

}

#endif
