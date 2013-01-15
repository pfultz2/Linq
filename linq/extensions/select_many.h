/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    select_many.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SELECT_MANY_H
#define LINQ_GUARD_EXTENSIONS_SELECT_MANY_H

#include <linq/extensions/extension.h>
#include <linq/extensions/detail/function_object.h>
#include <boost/range.hpp>
#include <boost/mpl/bool.hpp>

namespace linq { 


// bind_iterator
template<
class OuterIterator, 
class Selector, 
class SelectorRangeRef=typename std::result_of<Selector(typename boost::iterator_reference<OuterIterator>::type)>::type, 
class SelectorRange = typename std::decay<SelectorRangeRef>::type>
struct bind_iterator
: boost::iterator_facade
<
    bind_iterator<OuterIterator, Selector, SelectorRange>,
    typename boost::range_value<SelectorRange >::type,
    boost::forward_traversal_tag,
    typename boost::range_reference<SelectorRange >::type
>
{
    typedef typename boost::range_iterator<SelectorRange >::type InnerIteraror;

    Selector selector;
    OuterIterator iterator;
    InnerIteraror inner_first;
    InnerIteraror inner_last;
    OuterIterator last;

    bind_iterator(Selector selector, OuterIterator iterator, OuterIterator last) : selector(selector), iterator(iterator), last(last)
    {
        this->select_first();
    }

    void select_first()
    {
        if (iterator!=last)
        {
            this->inner_select();
            if (inner_first==inner_last) this->select();
        }
    }

    void inner_select()
    {
        auto&& r = selector(*iterator);
        static_assert(is_bindable_range<decltype(r)>::value, "Ranges returned from select_many selector must be bindable");
        inner_first = boost::begin(r);
        inner_last = boost::end(r);
    }
    
    void select()
    {
        for(;iterator!=last;iterator++)
        {
            if (inner_first==inner_last)
            {
                this->inner_select();
            }
            else inner_first++;
            for(;inner_first!=inner_last;inner_first++)
                return;
        }
    }

    void increment()
    {
        this->select();
    }

    bool equal(const bind_iterator& other) const
    {
        return this->iterator == other.iterator;
    }

    typename boost::range_reference<SelectorRange >::type dereference() const
    {
        assert(iterator!=last);
        assert(inner_first != inner_last);
        return *inner_first;
    }

};

template<class Iterator, class Selector>
bind_iterator<Iterator, Selector> make_bind_iterator(Selector selector, Iterator iterator, Iterator last)
{
    static_assert(is_bindable_range<typename std::result_of<Selector(typename boost::iterator_reference<Iterator>::type)>::type>::value, "Ranges returned from select_many selector must be bindable");
    return bind_iterator<Iterator, Selector>(selector, iterator, last);
}

template<class Range, class Selector>
auto bind_range(Range && r, Selector s) LINQ_RETURNS
(
    boost::make_iterator_range
    (
        make_bind_iterator(s, boost::begin(r), boost::end(r)),
        make_bind_iterator(s, boost::end(r), boost::end(r))
    )
);

namespace detail {
struct select_many_t
{
    template<class>
    struct result;

    template<class F, class Range, class Selector>
    struct result<F(Range, Selector)>
    {
        typedef typename boost::range_iterator<typename std::decay<Range>::type>::type iterator;
        typedef function_object<typename std::decay<Selector>::type> fun;
        typedef boost::iterator_range<bind_iterator<iterator,  fun> > type;
    };
    template<class Range, class Selector>
    typename result<select_many_t(Range&&, Selector)>::type operator()(Range && r, Selector s) const
    {
        return linq::bind_range(std::forward<Range>(r), make_function_object(s));
    };
};
}
namespace {
range_extension<detail::select_many_t> select_many = {};
}

}

#endif
