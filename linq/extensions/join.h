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
        template<class ResultSelector, class Key, class Value>
        auto operator()(ResultSelector rs, Key && k, Value && v) const LINQ_RETURNS
        (
            v | linq::select(std::bind(rs, k, linq::_1))
        );

    };
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector rs) const LINQ_RETURNS
    (
        outer | linq::group_join(std::forward<Inner>(inner), outer_key_selector, inner_key_selector, std::bind(defer<result_selector>(), protect(rs), linq::_1, linq::_2))
        | linq::select_many(linq::detail::identity_selector())
    );
};
}
namespace {
range_extension<detail::join_t> join = {};
}

}

#endif
