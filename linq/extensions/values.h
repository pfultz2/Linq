/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    values.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_VALUES_H
#define LINQ_GUARD_EXTENSIONS_VALUES_H

#include <linq/extensions/extension.h>
#include <linq/extensions/select.h>
#include <linq/extensions/detail/defer.h>

namespace linq { 

namespace detail {
struct values_t
{
    struct value_selector
    {
        template<class>
        struct result;

        template<class X, class T>
        struct result<X(T)>
        {
            typedef typename T::second_type type;
        };

        template<class X, class T>
        struct result<X(T&&)>
        {
            typedef typename T::second_type type;
        };

        template<class X, class T>
        struct result<X(T&)>
        {
            typedef typename std::add_lvalue_reference<typename T::second_type>::type type;
        };

        template<class X, class T>
        struct result<X(const T&)>
        {
            typedef typename std::add_const<typename std::add_lvalue_reference<typename T::second_type>::type>::type type;
        };

        template<class T>
        typename result<value_selector(T&&)>::type operator()(T && x) const 
        {
            return x.second;
        }
    };

    template<class>
    struct result;

    template<class X, class Range>
    struct result<X(Range)>
    : linq::result_of<select_t(Range, value_selector)>
    {};

    template<class Range>
    typename result<values_t(Range&&)>::type operator()(Range && r) const
    {
        return r | linq::select(value_selector());
    };
};
}
namespace {
range_extension<detail::values_t, true> values = {};
}

}

#endif
