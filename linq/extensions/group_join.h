/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    group_join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_GROUP_JOIN_H
#define LINQ_GUARD_EXTENSIONS_GROUP_JOIN_H

namespace linq { 

//
// group_join
//
namespace detail {

template<class InnerKeySelector>
struct join_inner_selector
{
    InnerKeySelector is;

    join_inner_selector(InnerKeySelector is) : is(is)
    {}

    template<class T>
    auto operator()(T && x) LINQ_RETURNS(std::make_pair(is(std::forward<T>(x), std::forward<T>(x)));
};

template<class InnerKeySelector>
auto make_join_inner_selector(InnerKeySelector is) LINQ_RETURNS(join_inner_selector<InnerKeySelector>(is));

template<class Value, class OuterKeySelector, class ResultKeySelector>
struct join_selector
{
    typedef decltype(linq::declval<OuterKeySelector>()(linq::declval<Value>())) key_t;
    boost::unordered_map<key_t, Value> inner_lookup;
    ResultKeySelector rs;
    OuterKeySelector os;

    template<class Range>
    join_selector(Range && r, OuterKeySelector os, ResultKeySelector rs)
    : inner_lookup(boost::begin(r), boost::end(r)), rs(rs), os(os)
    {}

    template<class Key>
    auto create_pair(Key && x) LINQ_RETURNS
    (
        std::make_pair(std::forward<T>(x), inner_lookup.equal_range(std::forward<T>(x)) | boost::adaptors::map_values)
    );

    template<class T>
    auto operator()(T && x) LINQ_RETURNS
    (
        rs
        ( 
            create_pair(os(std::forward<T>(x))) 
        )
    );
};

template<class Range, class OuterKeySelector, class ResultKeySelector>
static auto make_join_selector(Range && r, OuterKeySelector os, ResultKeySelector rs) LINQ_RETURNS
(join_selector<typename boost::range_value<R>, OuterKeySelector, ResultKeySelector >
(
    r, os, rs
));

struct group_join_t
{
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector result_selector) LINQ_RETURNS
    (
        outer | linq::select
        (
            make_join_selector(inner | linq::select(make_join_inner_selector(is)), outer_key_selector, result_selector)
        )
    );
};
}
namespace {
range_extension<detail::group_join_t> group_join = {};
}

}

#endif
