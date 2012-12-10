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
#include <linq/extensions/detail/defer.h>
#include <linq/utility.h>
#include <boost/range.hpp>
#include <functional>

namespace linq { 

//
// group_join
//
namespace detail {

// template<class T>
// struct no_volatile
// : boost::mpl::bool_<true>
// {};

// template<class T>
// struct no_volatile<volatile T>
// : boost::mpl::bool_<false>
// {};

// template<class T>
// struct no_volatile<T&>
// : no_volatile<T>
// {};

// template<class T>
// struct no_volatile<T&&>
// : no_volatile<T>
// {};

// struct join_inner_selector
// {

//     template<class InnerKeySelector, class T>
//     auto operator()(InnerKeySelector && is, T && x) const LINQ_RETURNS
//     (
//         std::make_pair( is(std::forward<T>(x)), std::forward<T>(x) )
//     );
// };

template<class InnerKeySelector>
struct join_inner_selector
{
    InnerKeySelector is;

    join_inner_selector(InnerKeySelector is)
    : is(is)
    {}

    template<class T>
    auto operator()(T && x) const -> decltype(std::make_pair( declval<const InnerKeySelector>()(std::forward<T>(x)), std::forward<T>(x) ))
    {
       return std::make_pair( is(std::forward<T>(x)), std::forward<T>(x) );
    };
};

template<class InnerKeySelector>
join_inner_selector < InnerKeySelector >
make_join_inner_selector (InnerKeySelector && is)
{
    return join_inner_selector < InnerKeySelector >
    (std::forward<InnerKeySelector>(is));
}

template<class Lookup, class OuterKeySelector, class ResultKeySelector>
struct join_outer_selector
{
    Lookup inner_lookup;
    OuterKeySelector os;
    ResultKeySelector rs;

    join_outer_selector(Lookup inner_lookup, OuterKeySelector os, ResultKeySelector rs)
    : inner_lookup(inner_lookup), os(os), rs(rs)
    {}

    template<class T>
    auto operator()(T && x) const 
    -> decltype(declval<const ResultKeySelector>()(std::forward<T>(x), declval<const Lookup>().equal_range(declval<const OuterKeySelector>()(std::forward<T>(x))) | linq::values))
    {
        return rs(std::forward<T>(x), inner_lookup.equal_range(os(std::forward<T>(x))) | linq::values);
    };
};

template<class Lookup, class OuterKeySelector, class ResultKeySelector>
join_outer_selector < Lookup, OuterKeySelector, ResultKeySelector >
make_join_outer_selector (Lookup inner_lookup, OuterKeySelector os, ResultKeySelector rs)
{
    return join_outer_selector < Lookup, OuterKeySelector, ResultKeySelector >
    (inner_lookup, os, rs);
}


// struct join_outer_selector
// {

//     template<class Lookup, class OuterKeySelector, class ResultKeySelector, class T>
//     auto operator()(const Lookup & inner_lookup, OuterKeySelector os, ResultKeySelector rs, T && x) const LINQ_RETURNS
//     (
//         rs(std::forward<T>(x), inner_lookup.equal_range(os(std::forward<T>(x))) | linq::values)
//     );
// };

// TODO: Add a way to statically assert that all function objects can be called
// with the correct type and display an error
struct group_join_t
{
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    auto operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector result_selector) const LINQ_RETURNS
    (
        outer | linq::select
        (
            make_join_outer_selector
            (
                make_map(inner | linq::select(make_join_inner_selector(inner_key_selector))), 
                outer_key_selector, 
                result_selector
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
