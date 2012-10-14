/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    default_if_empty.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_DEFAULT_IF_EMPTY_H
#define LINQ_GUARD_EXTENSIONS_DEFAULT_IF_EMPTY_H

#include <linq/extensions/extension.h>


namespace linq { 

//
// default_if_empty
//
namespace detail {

template<class Iterator, class Value=typename boost::iterator_value<Iterator>::type>
struct default_if_empty_iterator
: boost::iterator_facade<default_if_empty_iterator, Value, boost::forward_traversal_tag>
{
    const bool empty;
    Iterator it;
    Value* ref;
    boost::optional<Value> v;

    default_if_empty_iterator(bool empty, Iterator it, Value x)
    : empty(empty), it(it), v(not empty, x)
    {}

    // TODO: Assign operator

    void increment()
    {
        if (not empty) it++;
        else v = boost::optional<Value>();
    }

    bool equal(const default_if_empty_iterator<Iterator>& other) const
    {
        return 
        (
            it == other.it and
            (not (v xor other.v))
        );
    }

    typename boost::iteraror_reference<Iterator>::type deref() const
    {
        if (not empty) return *it;
        else return *v;
    }
};

template<class Iterator, class Value>
auto make_default_if_empty_iterator(bool empty, Iterator && it, Value && v) LINQ_RETURNS
(default_if_empty_iterator<Iterator>(empty, it, v));

template<class Range, class Value>
auto make_default_if_empty_range(bool empty, Range && r, Value && v) LINQ_RETURNS
(boost::make_iterator_range
(
    make_default_if_empty_iterator(empty, boost::begin(r), v),
    make_default_if_empty_iterator(empty, boost::end(r), v)
));

struct default_if_empty_t
{
    template<class Range, class T>
    auto operator()(Range && r, T && x) LINQ_RETURNS
    (make_default_if_empty_range(boost::empty(r), r, x));

    template<class Range, class T>
    auto operator()(Range && r) LINQ_RETURNS
    (make_default_if_empty_range(boost::empty(r), r, typename boost::range_value<Range>::type()));
};
}
namespace {
range_extension<detail::default_if_empty_t> default_if_empty = {};
}

}

#endif
