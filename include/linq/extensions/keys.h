/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    keys.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_KEYS_H
#define LINQ_GUARD_EXTENSIONS_KEYS_H

#include <linq/extensions/extension.h>
#include <linq/extensions/select.h>
#include <linq/utility.h>

namespace linq { 
namespace detail {
struct keys_t
{
    struct key_selector
    {

        template<class>
        struct result;

        template<class X, class T>
        struct result<X(T)>
        {
            typedef typename T::first_type type;
        };

        template<class X, class T>
        struct result<X(T&&)>
        {
            typedef typename T::first_type type;
        };

        template<class X, class T>
        struct result<X(T&)>
        {
            // typedef typename std::decay<typename T::first_type>::type& type;
            typedef typename std::add_const<typename std::decay<typename T::first_type>::type>::type& type;
        };

        template<class X, class T>
        struct result<X(const T&)>
        {
            typedef typename std::add_const<typename std::decay<typename T::first_type>::type>::type& type;
        };

        template<class T>
        typename result<key_selector(T&&)>::type operator()(T && x) const 
        {
            return x.first;
        }
    };

    template<class>
    struct result;

    template<class X, class Range>
    struct result<X(Range)>
    : linq::result_of<select_t(Range, key_selector)>
    {};

    template<class Range>
    typename result<keys_t(Range&&)>::type operator()(Range && r) const 
    {
        return r | linq::select(key_selector());
    };

};
}
namespace {
range_extension<detail::keys_t, true> keys = {};
}

}

#endif
