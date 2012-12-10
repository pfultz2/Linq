/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    default_if_empty.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_DEFAULT_IF_EMPTY_H
#define LINQ_GUARD_EXTENSIONS_DEFAULT_IF_EMPTY_H

#include <linq/extensions/extension.h>
#include <boost/range.hpp>
#include <boost/optional.hpp>
#include <boost/iterator/iterator_traits.hpp>


namespace linq { 

//
// default_if_empty
//
namespace detail {

template<class Iterator, class Value=typename boost::iterator_value<Iterator>::type>
struct default_if_empty_iterator
: boost::iterator_facade<default_if_empty_iterator<Iterator, Value>, Value, boost::forward_traversal_tag>
{
    const bool empty;
    Iterator it;
    Value* ref;
    //Value v;
    //boost::optional<Value> v;

    default_if_empty_iterator(bool empty, Iterator it, Value* x)
    : empty(empty), it(it), ref(x)
    {
    }

    // TODO: Assign operator

    void increment()
    {
        if (not empty) it++;
        else ref++;
    }

    bool equal(const default_if_empty_iterator<Iterator>& other) const
    {
        assert(this->empty == other.empty);
        if (this->empty) return ref == other.ref;
        else return it == other.it;
    }

    typename boost::iterator_reference<Iterator>::type dereference() const
    {
        if (not empty) return *it;
        else return *ref;
    }
};

template<class Iterator, class Value=typename boost::iterator_value<Iterator>::type>
struct default_if_empty_range : boost::iterator_range<default_if_empty_iterator<Iterator> >
{
    typedef boost::iterator_range<default_if_empty_iterator<Iterator> > base;
    typedef default_if_empty_iterator<Iterator> iterator;

    template<class T>
    static T* get_end(T* x)
    {
        return ++x; 
    }

    Value v;
    default_if_empty_range(bool empty, Iterator first, Iterator last, Value x)
    : v(x), base(iterator(empty, first, &v), iterator(empty, last, get_end(&v)))
    {}
};

template<class Iterator, class Value>
default_if_empty_range<Iterator>
make_default_if_empty_range(bool empty, Iterator first, Iterator last, Value && v)
{
    return default_if_empty_range<Iterator>(empty, first, last, v);
}

template<class Range, class Value>
auto make_default_if_empty_range(bool empty, Range && r, Value && v) LINQ_RETURNS
(make_default_if_empty_range(empty, boost::begin(r), boost::end(r), std::forward<Value>(v)));


struct default_if_empty_t
{
    template<class Range, class T>
    auto operator()(Range && r, T && x) const LINQ_RETURNS
    (make_default_if_empty_range(boost::empty(r), r, std::forward<T>(x)));

    template<class Range, class T>
    auto operator()(Range && r) const LINQ_RETURNS
    (make_default_if_empty_range(boost::empty(r), r, typename boost::range_value<Range>::type()));
};
}
namespace {
range_extension<detail::default_if_empty_t, true> default_if_empty = {};
}

}

#endif
