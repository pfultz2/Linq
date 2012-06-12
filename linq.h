/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    linq.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_H
#define INCLUDE_GUARD_LINQ_H

#include <utility>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/range.hpp> 
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

//
//
// Some preprocessor utilites
//
//

//
// LINQ_RETURNS for auto return type deduction.
//
#define LINQ_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

//
// LINQ_IS_PAREN is used to detect if the first token is a parenthesis.
// It expands to 1 if it is, otherwise it expands to 0.
//
#define LINQ_IS_PAREN(x) LINQ_IS_PAREN_CHECK(LINQ_IS_PAREN_PROBE x)
#define LINQ_IS_PAREN_CHECK(...) LINQ_IS_PAREN_CHECK_N(__VA_ARGS__,0)
#define LINQ_IS_PAREN_PROBE(...) ~, 1,
#ifndef _MSC_VER
#define LINQ_IS_PAREN_CHECK_N(x, n, ...) n
#else
// MSVC workarounds
#define LINQ_IS_PAREN_CHECK_RES(x) x
#define LINQ_IS_PAREN_CHECK_II(x, n, ...) n
#define LINQ_IS_PAREN_CHECK_I(x) LINQ_IS_PAREN_CHECK_RES(LINQ_IS_PAREN_CHECK_II x)
#define LINQ_IS_PAREN_CHECK_N(...) LINQ_IS_PAREN_CHECK_I((__VA_ARGS__))
#endif

//
// LINQ_IS_EMPTY will expands to 1 if the parameter is empty, otherwise
// it expands to 0. This will work even if the parameter given is a set
// of parenthesis.
//
#define LINQ_IS_EMPTY(x) BOOST_PP_CAT(LINQ_IS_EMPTY_, LINQ_IS_PAREN(x))(x)
#define LINQ_IS_EMPTY_0(x) BOOST_PP_IS_EMPTY(x)
#define LINQ_IS_EMPTY_1(x) 0 

//
// LINQ_HEAD retrieves the first element of a sequence.
// Example:
//
//     LINQ_HEAD((1)(2)(3)) // Expands to (1)
//
#define LINQ_HEAD(x) LINQ_PICK_HEAD(LINQ_MARK x)
#define LINQ_MARK(...) (__VA_ARGS__), 
#define LINQ_PICK_HEAD(...) LINQ_PICK_HEAD_I(__VA_ARGS__,)
#ifndef _MSC_VER
#define LINQ_PICK_HEAD_I(x, ...) x
#else
// MSVC workarounds
#define LINQ_PICK_HEAD_III(x, ...) x
#define LINQ_PICK_HEAD_II(x) LINQ_PICK_HEAD_III x
#define LINQ_PICK_HEAD_I(...) LINQ_PICK_HEAD_II((__VA_ARGS__))
#endif


//
// LINQ_TAIL retrieves the tail of a sequence.
// Example:
//
//     LINQ_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define LINQ_TAIL(x) LINQ_EAT x
// Various utilities
#define LINQ_EAT(...)
#define LINQ_REM(...) __VA_ARGS__
#define LINQ_EXPAND(...) __VA_ARGS__

//
// LINQ_KEYWORD transforms the keyword. Keywords are generally defined
// like this:
//
//     //This defines a `my_keyword` macro
//     //NOTE: The space between the keyword and parenthesis is necessary
//     #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//
// Here is an example:
//
//     LINQ_KEYWORD(my_keyword foo) // Expands to (MY_KEYWORD_MACRO) foo
//
#define LINQ_KEYWORD(x) BOOST_PP_CAT(LINQ_KEYWORD_, x)

//
// LINQ_IS_KEYWORD will expand to 1 if the first token is a valid keyword
//
#define LINQ_IS_KEYWORD(x) LINQ_IS_PAREN(LINQ_KEYWORD(x))

//
// LINQ_PLACE retrieves whats placed in parenthesis. In essence it does
// this:
//
//     LINQ_PLACE((1) foo) //Expands to 1
//
// But when its used in the contexts of keywords, it will retrieve whats
// been defined inside the parenthesis of a keyword. Heres an example:
//
//     //This defines a `my_keyword` macro
//     //NOTE: The space between the keyword and parenthesis is necessary
//     #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//     LINQ_PLACE(LINQ_KEYWORD(my_keyword foo)) // Expands to MY_KEYWORD_MACRO
//
#define LINQ_PLACE(x) LINQ_EXPAND(LINQ_REM LINQ_PICK_HEAD(LINQ_MARK x))

//
// LINQ_TO_SEQ converts the keywords into a preprocessor sequence
// 
#define LINQ_TO_SEQ(x) LINQ_TO_SEQ_WHILE_M \
( \
    BOOST_PP_WHILE(LINQ_TO_SEQ_WHILE_P, LINQ_TO_SEQ_WHILE_O, (,x)) \
)

#define LINQ_TO_SEQ_WHILE_P(r, state) LINQ_TO_SEQ_P state
#define LINQ_TO_SEQ_WHILE_O(r, state) LINQ_TO_SEQ_O state
#define LINQ_TO_SEQ_WHILE_M(state) LINQ_TO_SEQ_M state

#define LINQ_TO_SEQ_P(prev, tail) BOOST_PP_NOT(LINQ_IS_EMPTY(tail))
#define LINQ_TO_SEQ_O(prev, tail) \
    BOOST_PP_IF(LINQ_IS_PAREN(tail), \
        LINQ_TO_SEQ_PAREN, \
        LINQ_TO_SEQ_KEYWORD \
        )(prev, tail)
#define LINQ_TO_SEQ_PAREN(prev, tail) \
    (prev (LINQ_HEAD(tail)), LINQ_TAIL(tail))

#define LINQ_TO_SEQ_KEYWORD(prev, tail) \
    LINQ_TO_SEQ_REPLACE(prev, LINQ_KEYWORD(tail))

#define LINQ_TO_SEQ_REPLACE(prev, tail) \
    (prev LINQ_HEAD(tail), LINQ_TAIL(tail))

#define LINQ_TO_SEQ_M(prev, tail) prev


//
// LINQ_SEQ_OUT outputs the sequence
//
#define LINQ_SEQ_OUT(seq) BOOST_PP_SEQ_FOR_EACH(LINQ_SEQ_OUT_EACH, ~, seq) 
#define LINQ_SEQ_OUT_EACH(r, data, x) x

// TODO: LINQ_SEQ_TRANSFORM_W


namespace linq { 

//MSVC 2010 doesn't provide declval
template <typename T>
typename std::add_rvalue_reference<T>::type declval(); // no definition required

// bind_iterator
template<class OuterIterator, class Selector, class SelectorRange = typename boost::result_of<Selector(typename boost::iterator_reference<OuterIterator>::type)>::type>
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
    SelectorRange r;

    bind_iterator(Selector selector, OuterIterator iterator) : selector(selector), iterator(iterator)
    {
    }

    void increment()
    {
        if (inner_last == inner_first)
        {
            r = selector(*iterator);
            inner_first = boost::begin(r);
            inner_last = boost::end(r);
            iterator++;
        }
        else
        {
            inner_first++;
        }
    }

    bool equal(const bind_iterator& other) const
    {
        return this->iterator == other.iterator;
    }

    typename boost::range_reference<SelectorRange >::type dereference() const
    {
        return *inner_first;
    }

};

template<class Iterator, class Selector>
bind_iterator<Iterator, Selector> make_bind_iterator(Selector selector, Iterator iterator)
{
    return bind_iterator<Iterator, Selector>(selector, iterator);
}

// Bound range adaptor
namespace detail {
template<class Selector>
struct bound_t
{
    Selector s;
    bound_t(Selector s) : s(s)
    {
    }

    template<class Range>
    friend auto operator|(Range && r, bound_t self) LINQ_RETURNS
    (
        boost::make_iterator_range
        (
            make_bind_iterator(self.s, boost::begin(r)),
            make_bind_iterator(self.s, boost::end(r))
        )
    )
};
}

template<class Selector>
detail::bound_t<Selector> bound(Selector s)
{
    return detail::bound_t<Selector>(s);
}

namespace detail {

//This is used to workaround a bug in boost 1.49
template<class Fun>
struct transformer
{
    Fun f;

    transformer(Fun f): f(f)
    {}

    template<class F>
    struct result
    {};

    template<class F, class T>
    struct result<F(T)>
    {
        typedef decltype(linq::declval<Fun>()(linq::declval<T>())) type;
    };

    template<class T>
    auto operator()(T && x) const LINQ_RETURNS(f(std::forward<T>(x)))

    template<class T>
    auto operator()(T && x) LINQ_RETURNS(f(std::forward<T>(x)))
};

template<class F>
transformer<F> make_transformer(F f)
{
    return transformer<F>(f);
}

// These are used to avoid using parenthessis with lambdas, since it
// could perhaps result in a pathological case for the preprocessor.
struct where_t
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(boost::adaptors::filtered(p))
};

struct select_t
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(boost::adaptors::transformed(make_transformer(t)))
};

struct select_many_t
{
    template<class Sel>
    auto operator+(Sel s) LINQ_RETURNS(linq::bound(s))
};
}

detail::select_many_t select_many = {};
detail::select_t select = {};
detail::where_t where = {};

}

// Helps in defining lambdas in two steps. It also dedcues the type held
// by the container.
#define LINQ_LAMBDA_BLOCK(...) { return (__VA_ARGS__); }
#define LINQ_LAMBDA_HEADER(var, col) [&](decltype( *(boost::begin(col)) ) var)

// These macros help in defining the clauses. So instead of writing this:
//
//     auto r = numbers | boost::adaptors::filtered([](int i) { return i > 2});
//
// This can be written:
//
//     auto r = numbers | LINQ_WHERE(i, numbers)(i > 2);
//
#define LINQ_WHERE(var, col) linq::where + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT(var, col) linq::select + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK
#define LINQ_SELECT_MANY(var, col) linq::select_many + LINQ_LAMBDA_HEADER(var, col) LINQ_LAMBDA_BLOCK

//
// Keywords used by linq
//
#define LINQ_KEYWORD_from ()
#define LINQ_KEYWORD_select (LINQ_SELECT)
#define LINQ_KEYWORD_where (LINQ_WHERE)

//
// Process the sequence
//

// Expands the parameter, for another preprocessor scan
#define LINQ_X(...) __VA_ARGS__

#define LINQ_PROCESS_PAREN(data, x) x
#define LINQ_PROCESS_KEYWORD(data, x) | x data

// Process the select, where clauses
#define LINQ_COL(var, col) col
#define LINQ_SELECT_WHERE(data, seq) LINQ_COL data LINQ_SEQ_OUT(BOOST_PP_SEQ_TRANSFORM(LINQ_SELECT_WHERE_O, data, seq))
#define LINQ_SELECT_WHERE_O(s, data, x) BOOST_PP_IF(LINQ_IS_PAREN(x), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(data, x)
// Transforms the sequence
#define LINQ_TRANSFORM(seq) LINQ_X(LINQ_SELECT_WHERE(BOOST_PP_SEQ_ELEM(1, seq) ,BOOST_PP_SEQ_REST_N(2, seq)))
// And finally the LINQ macro
#define LINQ(x) LINQ_TRANSFORM(LINQ_TO_SEQ(x))

#endif