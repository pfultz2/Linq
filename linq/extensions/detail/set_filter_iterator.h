/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    set_filter_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_SET_FILTER_ITERATOR_H
#define LINQ_GUARD_DETAIL_SET_FILTER_ITERATOR_H

#include <boost/iterator_adaptors.hpp>
#include <boost/unordered_set.hpp>
#include <boost/range.hpp>
#include <linq/utility.h>

namespace linq { 

//
// set_filter_iterator
//
namespace detail {
// TODO: Add support for an equality selector
template <class Predicate, class Iterator>
struct set_filter_iterator
: boost::iterator_adaptor<set_filter_iterator<Predicate, Iterator>, Iterator, boost::use_default, boost::forward_traversal_tag>
{

    // Probably should be the initial base class so it can be
    // optimized away via EBO if it is an empty class.
    Predicate p;
    Iterator last;
    typedef boost::unordered_set<typename boost::iterator_value<Iterator>::type> set_t;
    set_t set;

    typedef boost::iterator_adaptor<set_filter_iterator<Predicate, Iterator>, Iterator, boost::use_default, boost::forward_traversal_tag> super_t;

    set_filter_iterator() { }

    template<class Range>
    set_filter_iterator(Range && r, Predicate f, Iterator x, Iterator l = Iterator())
        : super_t(x), p(f), last(l), set(boost::begin(r), boost::end(r))
    {
        satisfy_predicate();
    }

    set_filter_iterator(Predicate f, Iterator x, Iterator l = Iterator())
        : super_t(x), p(f), last(l)
    {
        satisfy_predicate();
    }

    set_filter_iterator(Iterator x, Iterator l = Iterator())
      : super_t(x), p(), last(l)
    {
        satisfy_predicate();
    }

    // template<class OtherIterator>
    // set_filter_iterator(
    //     set_filter_iterator<Predicate, OtherIterator> const& t
    //     , typename enable_if_convertible<OtherIterator, Iterator>::type* = 0
    //     )
    //     : super_t(t.base()), predicate(t.predicate()), last(t.end()) {}

    Predicate predicate() const { return p; }

    Iterator end() const { return last; }

    void increment()
    {
        ++(this->base_reference());
        satisfy_predicate();
    }

    void satisfy_predicate()
    {
        while (this->base() != this->last && !this->predicate()(*this->base(), this->set))
        {
            ++(this->base_reference());
        }
    }
};

template<class Iterator, class Predicate>
auto make_set_filter_iterator(Iterator it, Predicate p) LINQ_RETURNS
(set_filter_iterator<Predicate, Iterator>(p, it));

template<class Iterator, class Predicate>
auto make_set_filter_iterator(Iterator it, Iterator last, Predicate p) LINQ_RETURNS
(set_filter_iterator<Predicate, Iterator>(p, it, last));

template<class Range, class Iterator, class Predicate>
auto make_set_filter_iterator(Range && r, Iterator it, Iterator last, Predicate p) LINQ_RETURNS
(set_filter_iterator<Predicate, Iterator>(r, p, it, last));



template<class Range, class Predicate>
auto make_set_filter_range(Range && r, Predicate p) LINQ_RETURNS
(boost::make_iterator_range
(
make_set_filter_iterator(boost::begin(r), boost::end(r), p),
make_set_filter_iterator(boost::end(r), boost::end(r), p)
));

template<class Set, class Range, class Predicate>
auto make_set_filter_range(Set && s, Range && r, Predicate p) LINQ_RETURNS
(boost::make_iterator_range
(
make_set_filter_iterator(s, boost::begin(r), boost::end(r), p),
make_set_filter_iterator(boost::end(r), boost::end(r), p)
));

}

}

#endif
