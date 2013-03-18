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

template<class Lookup>
struct join_value_selector
{
    Lookup lookup;

    join_value_selector(Lookup lookup)
    : lookup(lookup)
    {}

    template<class T>
    auto operator()(T && x) const LINQ_RETURNS
    (x.second);
};

template<class Lookup>
join_value_selector < Lookup >
make_join_value_selector (Lookup lookup)
{
    return join_value_selector < Lookup >
    (lookup);
}


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

    template<class>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : linq::result_of<ResultKeySelector
    (
        T, 
        typename linq::result_of<select_t
        (
            std::pair<typename Lookup::element_type::iterator, typename Lookup::element_type::iterator>, 
            join_value_selector<Lookup>
        )>::type
    )>
    {};

    template<class T>
    typename result<join_outer_selector(T&&)>::type
    operator()(T && x) const 
    // -> decltype(declval<const ResultKeySelector>()(std::forward<T>(x), declval<const Lookup>()->equal_range(declval<const OuterKeySelector>()(std::forward<T>(x))) | linq::select(make_join_value_selector(declval<const Lookup>()))))
    {
        return rs(std::forward<T>(x), inner_lookup->equal_range(os(std::forward<T>(x))) | linq::select(make_join_value_selector(inner_lookup)));
    };
};

template<class Lookup, class OuterKeySelector, class ResultKeySelector>
join_outer_selector < Lookup, OuterKeySelector, ResultKeySelector >
make_join_outer_selector (Lookup inner_lookup, OuterKeySelector os, ResultKeySelector rs)
{
    return join_outer_selector < Lookup, OuterKeySelector, ResultKeySelector >
    (inner_lookup, os, rs);
}

// TODO: Add a way to statically assert that all function objects can be called
// with the correct type and display an error
struct group_join_t
{
    template<class>
    struct result;

    template<class X, class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    struct result<X(Outer, Inner, OuterKeySelector, InnerKeySelector, ResultSelector) >
    : linq::result_of<select_t
    (
        Outer,
        join_outer_selector
        <
            typename as_shared_map<typename result_of<select_t(Inner, join_inner_selector<typename boost::decay<InnerKeySelector>::type>)>::type>::type,
            typename boost::decay<OuterKeySelector>::type,
            typename boost::decay<ResultSelector>::type
        >
    )>
    {};
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    typename result<group_join_t(Outer&&, Inner&&, OuterKeySelector, InnerKeySelector, ResultSelector)>::type 
    operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector result_selector) const
    {
        return outer | linq::select
        (
            make_join_outer_selector
            (
                make_shared_map(inner | linq::select(make_join_inner_selector(inner_key_selector))), 
                outer_key_selector, 
                result_selector
            )
        );
    };
};
}
namespace {
range_extension<detail::group_join_t> group_join = {};
}

}

#endif
