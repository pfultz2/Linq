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
    template<class ResultSelector, class Key>
    struct apply_result_selector
    {
        ResultSelector rs;
        Key k;

        apply_result_selector(ResultSelector rs, Key k)
        : rs(rs), k(k)
        {}

        template<class T>
        auto operator()(T && x) const -> decltype(declval<const ResultSelector>()(declval<const Key>(), std::forward<T>(x)))
        {
            return rs(k, std::forward<T>(x));
        }
    };

    template<class ResultSelector, class Key>
    static apply_result_selector < ResultSelector, Key >
    make_apply_result_selector (ResultSelector rs, Key k)
    {
        return apply_result_selector < ResultSelector, Key >
        (rs, k);
    }


    template<class ResultSelector>
    struct result_selector
    {
        ResultSelector rs;

        result_selector(ResultSelector rs)
        : rs(rs)
        {}

        template<class>
        struct result;

        template<class X, class Key, class Value>
        struct result<X(Key, Value)>
        {
            typedef decltype(declval<Value>() | linq::select(make_apply_result_selector(declval<const ResultSelector>(), declval<Key>()))) type;
        };

        template<class Key, class Value>
        typename result<result_selector(Key&&, Value&&)>::type
        operator()(Key && k, Value && v) const // -> decltype(v | linq::select(make_apply_result_selector(declval<const ResultSelector>(), k)))
        {
            return v | linq::select(make_apply_result_selector(rs, k));
        };
    };

    template<class ResultSelector>
    static result_selector< function_object<ResultSelector> >
    make_result_selector (ResultSelector rs)
    {
        return result_selector < function_object<ResultSelector> >
        (make_function_object(rs));
    }

    template<class>
    struct result;

    template<class X, class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    struct result<X(Outer, Inner, OuterKeySelector, InnerKeySelector, ResultSelector)>
    {
        static Outer && outer;
        static Inner && inner;
        static OuterKeySelector&& outer_key_selector;
        static InnerKeySelector&& inner_key_selector;
        static ResultSelector&& rs;

        typedef decltype
        (
            outer | linq::group_join(std::forward<Inner>(inner), outer_key_selector, inner_key_selector, make_result_selector(rs))
            | linq::select_many(linq::detail::identity_selector())
        ) type;
    };
    
    template<class Outer, class Inner, class OuterKeySelector, class InnerKeySelector, class ResultSelector>
    typename result<join_t(Outer&&, Inner&&, OuterKeySelector, InnerKeySelector, ResultSelector)>::type
    operator()(Outer && outer, Inner && inner, OuterKeySelector outer_key_selector, InnerKeySelector inner_key_selector, ResultSelector rs) const
    {
        return outer | linq::group_join(std::forward<Inner>(inner), outer_key_selector, inner_key_selector, make_result_selector(rs))
        | linq::select_many(linq::detail::identity_selector());
    }
};
}
namespace {
range_extension<detail::join_t> join = {};
}

}

#endif
