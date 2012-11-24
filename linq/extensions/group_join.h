/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    group_join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_GROUP_JOIN_H
#define LINQ_GUARD_EXTENSIONS_GROUP_JOIN_H

#include <linq/extensions/extension.h>
#include <linq/extensions/select.h>
#include <linq/extensions/values.h>
#include <linq/extensions/detail/identity.h>
#include <linq/extensions/detail/make_map.h>
#include <linq/extensions/detail/placeholders.h>
#include <linq/utility.h>
#include <boost/range.hpp>
#include <functional>

namespace linq { 

//
// group_join
//
namespace detail {

struct join_inner_selector
{

    template<class InnerKeySelector, class T>
    auto operator()(InnerKeySelector && is, T && x) const LINQ_RETURNS
    (
        std::make_pair( is(std::forward<T>(x)), std::forward<T>(x) )
    );
};


struct join_selector
{

    template<class Lookup, class Key>
    static auto create_pair(Lookup && inner_lookup, Key && x) LINQ_RETURNS
    (
        std::make_pair(std::forward<Key>(x), inner_lookup.equal_range(std::forward<Key>(x)) | linq::values)
    );

    template<class Lookup, class OuterKeySelector, class ResultKeySelector, class T>
    auto operator()(Lookup && inner_lookup, OuterKeySelector os, ResultKeySelector rs, T && x) const LINQ_RETURNS
    (
        rs
        ( 
            create_pair(std::forward<Lookup>(inner_lookup), os(std::forward<T>(x))) 
        )
    );
};

struct group_join_t
{
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector result_selector) const LINQ_RETURNS
    (
        outer | linq::select
        (
            std::bind
            (
                join_selector(), 
                make_map(inner | linq::select(std::bind(join_inner_selector(), inner_key_selector, _1))), 
                outer_key_selector, 
                result_selector, 
                _1
            )
        )
    );
};
}
namespace {
range_extension<detail::group_join_t> group_join = {};
}

}

#endif
