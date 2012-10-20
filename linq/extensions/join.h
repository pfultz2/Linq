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

namespace linq { 
namespace detail {
struct join_t
{
    struct pair_selector
    {
        template<class Key, class Value>
        auto operator()(Key && k, Value && v) const LINQ_RETURNS
        (
            std::make_pair(std::forward<Key>(k), std::forward<Value>(v));
        );
    };
    struct join_selector
    {
        template<class Pair>
        auto operator()(Pair && p) const LINQ_RETURNS
        (
            p.second | linq::select(std::bind(pair_selector(), p.first, _1));
        );
    };
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector result_selector) LINQ_RETURNS
    (
        linq::group_join(std::forward<Outer>(outer), std::forward<Inner>(inner), outer_key_selector, inner_key_selector, result_selector)
        | linq::select_many(join_selector())
    );
};
}
namespace {
range_extension<detail::join_t> join = {};
}

}

#endif
