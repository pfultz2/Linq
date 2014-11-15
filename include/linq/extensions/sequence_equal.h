/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    sequence_equal.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_SEQUENCE_EQUAL_H
#define LINQ_GUARD_EXTENSIONS_SEQUENCE_EQUAL_H

#include <linq/extensions/extension.h>
#include <linq/utility.h>
#include <boost/range.hpp>

namespace linq { 
namespace detail {

template <class InputIterator1, class InputIterator2>
bool equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2 )
{
  while ( first1!=last1 )
  {
    if (first2 == last2) return false;
    if (!(*first1 == *first2))   // or: if (!pred(*first1,*first2)), for pred version
      return false;
    ++first1; ++first2;
  }
  return first2 == last2;
}

struct sequence_equal_t
{
    template<class R1, class R2>
    bool operator()(R1 && r1, R2 && r2) const
    {
      return linq::detail::equal(boost::begin(r1), boost::end(r1), boost::begin(r2), boost::end(r2));
    }
};
}
namespace {
range_extension<detail::sequence_equal_t> sequence_equal = {};
}

}

#endif
