/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_JOIN_H
#define LINQ_GUARD_EXTENSIONS_JOIN_H

#include <linq/extensions/extension.h>
#include <linq/extensions/group_join.h>
#include <linq/extensions/select_many.h>
#include <linq/extensions/select.h>


#include <linq/utility.h>

namespace linq { 
namespace detail {
struct join_t
{
    struct result_selector
    {
        template<class ResultSelector, class Pair>
        auto operator()(ResultSelector rs, Pair && p) const LINQ_RETURNS
        (
            rs(p.first, p.second)
        );
    };
    struct make_pair_selector
    {
        template<class Key, class Value>
        auto operator()(Key && k, Value && v) const LINQ_RETURNS
        (
            std::make_pair(std::forward<Key>(k), std::forward<Value>(v))
        );
    };
    struct join_selector
    {
        template<class Pair>
        auto operator()(Pair && p) const LINQ_RETURNS
        (
            p.second | linq::select(std::bind(make_pair_selector(), p.first, linq::_1))
        );
    };
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector rs) LINQ_RETURNS
    (
        outer | linq::group_join(std::forward<Inner>(inner), outer_key_selector, inner_key_selector, make_pair_selector())
        | linq::select_many(defer<join_selector>())
        | linq::select(std::bind(defer<result_selector>(), protect(rs), linq::_1))
    );
};
}
namespace {
range_extension<detail::join_t> join = {};
}

}

#endif
