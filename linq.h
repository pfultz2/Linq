/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    linq.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_H
#define INCLUDE_GUARD_LINQ_H

#include <utility>
#include <boost/optional.hpp>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <numeric>

//
//
// Some preprocessor utilites
//
//

//
// LINQ_RETURNS for auto return type deduction.
//
#define LINQ_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }


#define LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) decltype(__VA_ARGS__)>::type { return __VA_ARGS__; }
#define LINQ_RETURN_REQUIRES(...) -> typename boost::enable_if<__VA_ARGS__, LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

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
// LINQ_HEAD((1)(2)(3)) // Expands to (1)
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
// LINQ_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define LINQ_TAIL(x) LINQ_EAT x
// Various utilities
#define LINQ_EMPTY(...)
#define LINQ_EAT(...)
#define LINQ_REM(...) __VA_ARGS__
#define LINQ_EXPAND(...) __VA_ARGS__
#define LINQ_DEFER(...) __VA_ARGS__ LINQ_EMPTY()
#define LINQ_OBSTRUCT(...) __VA_ARGS__ LINQ_DEFER(LINQ_EMPTY)()

#define LINQ_EVAL(...) LINQ_EVAL_A(LINQ_EVAL_A(LINQ_EVAL_A(__VA_ARGS__)))
#define LINQ_EVAL_A(...) LINQ_EVAL_B(LINQ_EVAL_B(LINQ_EVAL_B(__VA_ARGS__)))
#define LINQ_EVAL_B(...) LINQ_EVAL_C(LINQ_EVAL_C(LINQ_EVAL_C(__VA_ARGS__)))
#define LINQ_EVAL_C(...) LINQ_EVAL_D(LINQ_EVAL_D(LINQ_EVAL_D(__VA_ARGS__)))
#define LINQ_EVAL_D(...) LINQ_EVAL_E(LINQ_EVAL_E(LINQ_EVAL_E(__VA_ARGS__)))
#define LINQ_EVAL_E(...) __VA_ARGS__

//
// LINQ_BACK gets the last element of a sequence
//
#define LINQ_BACK(seq) BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), seq)

//
// LINQ_KEYWORD transforms the keyword. Keywords are generally defined
// like this:
//
// //This defines a `my_keyword` macro
// //NOTE: The space between the keyword and parenthesis is necessary
// #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
//
// Here is an example:
//
// LINQ_KEYWORD(my_keyword foo) // Expands to (MY_KEYWORD_MACRO) foo
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
// LINQ_PLACE((1) foo) //Expands to 1
//
// But when its used in the contexts of keywords, it will retrieve whats
// been defined inside the parenthesis of a keyword. Heres an example:
//
// //This defines a `my_keyword` macro
// //NOTE: The space between the keyword and parenthesis is necessary
// #define LINQ_KEYWORD_my_keyword (MY_KEYWORD_MACRO)
// LINQ_PLACE(LINQ_KEYWORD(my_keyword foo)) // Expands to MY_KEYWORD_MACRO
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
// LINQ_SEQ_TO_STRING convert a sequence back to a string of tokens
//
#define LINQ_SEQ_TO_STRING(seq) BOOST_PP_SEQ_FOR_EACH(LINQ_SEQ_TO_STRING_EACH, ~, seq)
#define LINQ_SEQ_TO_STRING_EACH(r, data, x) x

//
// LINQ_SEQ_SPLIT
//
#define LINQ_SEQ_SPLIT(seq, pred, data) LINQ_SEQ_SPLIT_FOLD_LEFT_M(BOOST_PP_SEQ_FOLD_LEFT(LINQ_SEQ_SPLIT_FOLD_LEFT_O, (pred, data,,), seq))
#define LINQ_SEQ_SPLIT_FOLD_LEFT_O(s, state, x) LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE((s, x, LINQ_REM state))
#ifndef _MSC_VER
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE(x) LINQ_SEQ_SPLIT_OP x
#else
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE(x) LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE_I x
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE_I(...) LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE_II((__VA_ARGS__))
#define LINQ_SEQ_SPLIT_FOLD_LEFT_INVOKE_II(x) LINQ_SEQ_SPLIT_OP x
#endif
#define LINQ_SEQ_SPLIT_OP(s, x, pred, data, seq, elem) BOOST_PP_IF(pred(s, data, x), LINQ_SEQ_SPLIT_OP_TRUE, LINQ_SEQ_SPLIT_OP_FALSE)(x, pred, data, seq, elem)
#define LINQ_SEQ_SPLIT_OP_TRUE(x, pred, data, seq, elem) BOOST_PP_IIF(LINQ_IS_PAREN(elem), \
                                                                    (pred, data, seq(elem),),\
                                                                    (pred, data, seq,) )
#define LINQ_SEQ_SPLIT_OP_FALSE(x, pred, data, seq, elem) (pred, data, seq, elem (x))
#ifndef _MSC_VER
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M(x) LINQ_SEQ_SPLIT_M x
#else
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M_X(x) x
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M_N(x) LINQ_SEQ_SPLIT_FOLD_LEFT_M_X(LINQ_SEQ_SPLIT_FOLD_LEFT_M_X(LINQ_SEQ_SPLIT_FOLD_LEFT_M_X(LINQ_SEQ_SPLIT_FOLD_LEFT_M_X(x))))
#define LINQ_SEQ_SPLIT_FOLD_LEFT_M(x) LINQ_SEQ_SPLIT_FOLD_LEFT_M_N(LINQ_SEQ_SPLIT_M x)
#endif
#define LINQ_SEQ_SPLIT_M(pred, data, seq, elem) seq BOOST_PP_IIF(LINQ_IS_PAREN(elem), (elem),)

//
// LINQ_SEQ_NEST
//
#define LINQ_SEQ_NEST(seq) BOOST_PP_SEQ_FOLD_LEFT(LINQ_SEQ_NEST_OP, LINQ_BACK(seq) , BOOST_PP_SEQ_POP_BACK(seq))
#define LINQ_SEQ_NEST_OP(s, state, x) x(state)

//
// LINQ_SEQ_NEST_REVERSE
//
#define LINQ_SEQ_NEST_REVERSE(seq) BOOST_PP_SEQ_FOLD_RIGHT(LINQ_SEQ_NEST_OP, LINQ_BACK(seq) , BOOST_PP_SEQ_POP_BACK(seq))

//
// LINQ_VARN_CAT
//
#define LINQ_VARN_CAT(n, tuple) LINQ_EVAL(LINQ_VARN_CAT_D(n, tuple))
#define LINQ_VARN_CAT_D(n, tuple) LINQ_VARN_INVOKE((n, LINQ_REM tuple, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT))
#define LINQ_VARN_CAT_ID() LINQ_VARN_CAT_D

#ifndef _MSC_VER
#define LINQ_VARN_INVOKE(data)  LINQ_VARN_PRIMITIVE_CAT data
#else
// MSVC Workarounds
#define LINQ_VARN_INVOKE(data)  LINQ_VARN_INVOKE_I(data)
#define LINQ_VARN_INVOKE_I(data)  LINQ_VARN_PRIMITIVE_CAT data
#endif
#define LINQ_VARN_PRIMITIVE_CAT(n, a, b, c, d, e, f, g, h, ...) LINQ_VARN_CAT_E(a, n) LINQ_VARN_CAT_E(b, n) LINQ_VARN_CAT_E(c, n) LINQ_VARN_CAT_E(d, n) LINQ_VARN_CAT_E(e, n) LINQ_VARN_CAT_E(f, n) LINQ_VARN_CAT_E(g, n) LINQ_VARN_CAT_E(h, n) 

#define LINQ_VARN_CAT_E(a, n) BOOST_PP_IF(LINQ_IS_PAREN(a), LINQ_VARN_CAT_EACH_PAREN, LINQ_VARN_CAT_EACH_TOKEN)(a, n)
#define LINQ_VARN_CAT_EACH_PAREN(a, n) (LINQ_OBSTRUCT(LINQ_VARN_CAT_ID)()(n, a))
#define LINQ_VARN_CAT_EACH_TOKEN(a, n)  a ## n

//
// LINQ_PARAMS
//
#define LINQ_PARAMS(n, ...) BOOST_PP_ENUM(n, LINQ_PARAMS_EACH, (__VA_ARGS__))
#define LINQ_PARAMS_Z(z, n, ...) BOOST_PP_ENUM_ ## z(n, LINQ_PARAMS_EACH, (__VA_ARGS__)) 
#define LINQ_PARAMS_EACH(z, n, data) LINQ_VARN_CAT(n, data)

//
// LINQ_GEN
//
#define LINQ_GEN(n, ...) BOOST_PP_REPEAT(n, LINQ_GEN_EACH, (__VA_ARGS__)) 
#define LINQ_GEN_Z(z, n, ...) BOOST_PP_REPEAT_ ## z(n, LINQ_GEN_EACH, (__VA_ARGS__)) 
#define LINQ_GEN_EACH(z, n, data) LINQ_VARN_CAT(n, data)



//
//
// Extensions
//
//

namespace linq {

// MSVC 2010 doesn't provide declval
// We also return T&& instead std::add_rvalue_reference<T>
// because MSVC has a buggy implementation of it.
// So, this function will work in all cases except when T
// is void(which should rarely happen).
template <typename T>
T&& declval(); // no definition required

//
// is_iterator type trait
//
namespace detail {
BOOST_MPL_HAS_XXX_TRAIT_DEF(iterator_category)
}

template<class T, class Enabler = void>
struct is_iterator
: boost::mpl::false_
{};
template<class T>
struct is_iterator<T, BOOST_DEDUCED_TYPENAME boost::enable_if<detail::has_iterator_category<T> >::type >
: boost::mpl::true_
{};
template<class T>
struct is_iterator<T, BOOST_DEDUCED_TYPENAME boost::enable_if<boost::is_pointer<T> >::type >
: boost::mpl::true_
{};

//
// is_range type trait
//
template<class T>
struct is_range : boost::mpl::eval_if< boost::is_const<T>,
boost::has_range_const_iterator<BOOST_DEDUCED_TYPENAME boost::remove_const<T>::type>,
boost::mpl::and_<boost::has_range_iterator<T>, boost::has_range_const_iterator<T> > 
>::type
{};
template<class T, class U>
struct is_range<std::pair<T, U> > : boost::mpl::and_<is_iterator<T>, is_iterator<U>, boost::is_same<T, U> >::type
{};

// 
// Range extension
//
namespace detail {
struct na {};

template<class F, BOOST_PP_ENUM_BINARY_PARAMS_Z(1, 4, class T, = na BOOST_PP_INTERCEPT) >
struct pipe_closure {};

#define LINQ_PIPE_CLOSURE(z, n, data) \
template<class F, LINQ_PARAMS(n, T), LINQ_PARAMS(BOOST_PP_SUB(4,n), na BOOST_PP_INTERCEPT)> \
struct pipe_closure \
{ \
    LINQ_GEN(n, T, x, ; BOOST_PP_INTERCEPT) \
    template<LINQ_PARAMS(n, class X)>\
    pipe_closure(LINQ_PARAMS(n, X, && BOOST_PP_INTERCEPT, x)) \
    : LINQ_PARAMS(n, x, (std::forward<X, > BOOST_PP_INTERCEPT, (x))) {} \
    \
    template<class Range> \
    friend auto operator|(Range && r, pipe_closure p) LINQ_RETURN_REQUIRES(is_range<Range>) \
    (F()(std::forward<Range>(r), LINQ_PARAMS(n, std::forward<T, > BOOST_PP_INTERCEPT, (p.x)) )) \
\
};
BOOST_PP_REPEAT_FROM_TO(1, 4, LINQ_PIPE_CLOSURE, ~)
}
#define LINQ_RANGE_EXTENSION_OP(z, n, data) \
    template<class Range, BOOST_PP_COMMA_IF(n) LINQ_PARAMS(n, class T)> \
    auto operator()(Range && r BOOST_PP_COMMA_IF(n) LINQ_PARAMS(n, T, && BOOST_PP_INTERCEPT, x)) LINQ_RETURN_REQUIRES(is_range<Range>) \
    (F()(std::forward<Range>(r) BOOST_PP_COMMA_IF(n) LINQ_PARAMS(n, std::forward<T, > BOOST_PP_INTERCEPT, (x)) )) \
    \
    template<LINQ_PARAMS(n, class T)> \
    auto operator()(LINQ_PARAMS(n, T, && BOOST_PP_INTERCEPT, x) ) LINQ_RETURN_REQUIRES(boost::mpl::not_<is_range<T0> >) \
    ( \
        detail::pipe_closure<F, LINQ_PARAMS(n, T, && BOOST_PP_INTERCEPT)> \
        (LINQ_PARAMS(n, std::forward<T, > BOOST_PP_INTERCEPT, (x)) ) \
    ) 

template<class F, int N=-1>
struct range_extension
{
    BOOST_PP_REPEAT(4, LINQ_RANGE_EXTENSION_OP, ~)
    template<class Range>
    friend auto operator|(Range && r, range_extension) LINQ_RETURN_REQUIRES(is_range<Range>)
    (F()(std::forward<Range>(r)))
};

#define LINQ_RANGE_EXTENSION(z, n, data) \
template<class F> \
struct range_extension<F, n> \
{ \
    LINQ_RANGE_EXTENSION_OP(z, n, data) \
    template<class Range> \
    friend auto operator|(Range && r, range_extension) LINQ_RETURN_REQUIRES(is_range<Range>) \
    (F()(std::forward<Range>(r))) \
};
BOOST_PP_REPEAT_FROM_TO(1, 4, LINQ_RANGE_EXTENSION, ~)

//
// find
//
namespace detail {

// TODO: Add overload for string
template<class Range, class T>
auto find(Range && r, T && x) LINQ_RETURNS(std::find(boost::begin(r), boost::end(r), std::forward<T>(x)))

struct find_t
{
    template<class Range, class T>
    auto operator()(Range && r, T && x) 
    LINQ_RETURNS(find(std::forward<Range>(r), std::forward<T>(x)))
};
}
namespace {
range_extension<detail::find_t, 1> find = {};
}

//
// select
//
namespace detail {
struct select_t
{
    //TODO: make it work for empty and single ranges

    template<class F, class It>
    static auto make_transform_iterator(F f, It it) LINQ_RETURNS(boost::transform_iterator<F, It>(it, f))

    template<class Range, class Selector>
    auto operator()(Range && r, Selector selector) 
    LINQ_RETURNS(boost::make_iterator_range(make_transform_iterator(selector, boost::begin(r)), make_transform_iterator(selector, boost::end(r))) )

};
}
namespace {
range_extension<detail::select_t> select = {};
}


//
// aggregate
//
namespace detail {
struct aggregate_t
{
    //TODO: make it work for empty and single ranges
    template<class Range, class Reducer>
    auto operator()(Range && r, Reducer reducer) LINQ_RETURNS(std::accumulate(++boost::begin(r), boost::end(r), *boost::begin(r)))

    template<class Range, class Seed, class Reducer>
    auto operator()(Range && r, Seed && s, Reducer reducer) LINQ_RETURNS(std::accumulate(boost::begin(r), boost::end(r), s, reducer))

    template<class Range, class Seed, class Reducer>
    auto operator()(Range && r, Seed && s, Reducer reducer, Selector sel) LINQ_RETURNS(sel(std::accumulate(boost::begin(r), boost::end(r), s, reducer)))
};
}
namespace {
range_extension<detail::aggregate_t> aggregate = {};
}

//
// all
//
namespace detail {
struct all_t
{
    template<class Range, class Pred>
    auto operator()(Range && r, Pred p) LINQ_RETURNS(std::all_of(boost::begin(r), boost::end(r), pred))
};
}
namespace {
range_extension<detail::all_t> all = {};
}

//
// any
//
namespace detail {
struct any_t
{
    template<class Range, class Pred>
    auto operator()(Range && r) LINQ_RETURNS(!boost::empty(r))

    template<class Range, class Pred>
    auto operator()(Range && r, Pred p) LINQ_RETURNS(std::any_of(boost::begin(r), boost::end(r), pred))
};
}
namespace {
range_extension<detail::any_t> any = {};
}

//
// average
//
// TODO

//
// concat
//
namespace detail {
struct concat_t
{
    template<class Range1, class Range2>
    auto operator()(Range1 && r1, Range2 && r2) LINQ_RETURNS(boost::join(r1, r2))
};
}
namespace {
range_extension<detail::concat_t, 2> concat = {};
}

//
// contains
//


//
// count
//


//
// default_if_empty
//

//
// distinct
//

//
// element_at
//

//
// except
//



//
// first
//

//
// first_or_default
//

//
// group_by
//

//
// intersect
//

//
// join
//

//
// last
//

//
// last_or_default
//

//
// max
//

//
// min
//

//
// order_by
//

//
// reverse
//

//
// select_many
//

//
// sequence_equal
//

//
// single
//

//
// single_or_default
//

//
// skip
//

//
// skip_while
//

//
// sum
//

//
// take
//

//
// take_while
//

//
// to_container
//

//
// to_string
//

//
// union
//

//
// where
//

//
// zip
//

// Lambdas aren't very nice, so we use this wrapper to make them play nicer. This
// will make the function_object default constructible, even if it doesn't have a
// default constructor. This helpful since these function objects are being used
// inside of iterators.
template<class Fun>
struct function_object
{
    boost::optional<Fun> f;

    function_object()
    {}
    function_object(Fun f): f(f)
    {}

    function_object(const function_object & rhs) : f(rhs.f)
    {}

    // Assignment operator is just a copy construction, which does not provide
    // the strong exception guarentee.
    function_object& operator=(const function_object& rhs)
    {
        if (this != &rhs)
        {
            this->~function_object();
            new (this) function_object(rhs);
        }
        return *this;
    }

    template<class F>
    struct result
    {};

    template<class F, class T>
    struct result<F(T)>
    {
        typedef decltype(linq::declval<Fun>()(linq::declval<T>())) type;
    };

    template<class T>
    auto operator()(T && x) const LINQ_RETURNS((*f)(std::forward<T>(x)))

    template<class T>
    auto operator()(T && x) LINQ_RETURNS((*f)(std::forward<T>(x)))
};

template<class F>
function_object<F> make_function_object(F f)
{
    return function_object<F>(f);
}


// bind_iterator
template<class OuterIterator, class Selector, class SelectorRange = typename std::result_of<Selector(typename boost::iterator_reference<OuterIterator>::type)>::type>
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
        this->select();
    }

    void select()
    {
        for(;iterator!=last;iterator++)
        {
            if (inner_first==inner_last)
            {
                auto&& r = selector(*iterator);
                inner_first = boost::begin(r);
                inner_last = boost::end(r);
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
        return *inner_first;
    }

};

template<class Iterator, class Selector>
bind_iterator<Iterator, Selector> make_bind_iterator(Selector selector, Iterator iterator, Iterator last)
{
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
)

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
    friend auto operator|(Range && r, const bound_t self) -> decltype(linq::bind_range(std::forward<Range>(r), linq::declval<Selector>()))
    {
        return linq::bind_range(std::forward<Range>(r), self.s);
    }

};
}

template<class Selector>
detail::bound_t<Selector> bound(Selector s)
{
    return detail::bound_t<Selector>(s);
}



namespace detail {

// These are used to avoid using parenthessis with lambdas, since it
// could perhaps result in a pathological case for the preprocessor.
struct where_t
{
    template<class Pred>
    auto operator+(Pred p) LINQ_RETURNS(boost::adaptors::filtered(linq::make_function_object(p)))
};

struct select_t
{
    template<class Tran>
    auto operator+(Tran t) LINQ_RETURNS(boost::adaptors::transformed(linq::make_function_object(t)))
};

struct select_many_t
{
    template<class Sel>
    auto operator+(Sel s) LINQ_RETURNS(linq::bound(make_function_object(s)))
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
// auto r = numbers | boost::adaptors::filtered([](int i) { return i > 2});
//
// This can be written:
//
// auto r = numbers | LINQ_WHERE(i, numbers)(i > 2);
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
#ifndef _MSC_VER
#define LINQ_PROCESS_KEYWORD(data, x) | x data
#else
// MSVC Workarounds
#define LINQ_PROCESS_KEYWORD_RES(x) x
#define LINQ_PROCESS_KEYWORD(data, x) | LINQ_PROCESS_KEYWORD_RES(x data)
#endif

#define LINQ_COL(var, col) col

// Process the select, where clauses
#define LINQ_SELECT_WHERE(seq) LINQ_SELECT_WHERE_TRANSFORM(BOOST_PP_SEQ_ELEM(0, seq) ,BOOST_PP_SEQ_REST_N(1, seq))
#define LINQ_SELECT_WHERE_TRANSFORM(data, seq) LINQ_COL data LINQ_SEQ_TO_STRING(BOOST_PP_SEQ_TRANSFORM(LINQ_SELECT_WHERE_O, data, seq))
#define LINQ_SELECT_WHERE_O(s, data, x) BOOST_PP_IF(LINQ_IS_PAREN(x), LINQ_PROCESS_PAREN, LINQ_PROCESS_KEYWORD)(data, x)

// Process from clauses
// ()((x, col))()((y, x.col))(LINQ_SELECT)((x))
// SPLIT
// ( ((x, col)) ) ( ((y, x.col))(LINQ_SELECT)((x)) )
// TRANSFORM
// (SELECT_MANY(x, col))((y, x.col)(LINQ_SELECT)(x))
// NEST
#define LINQ_FROM(seq) LINQ_FROM_CHECK(LINQ_SEQ_SPLIT(seq, LINQ_FROM_P, data))
#define LINQ_FROM_CHECK(seq) BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), LINQ_FROM_MULTI, LINQ_FROM_SINGLE)(seq)
#define LINQ_FROM_MULTI(seq) LINQ_SEQ_NEST(LINQ_FROM_TRANSFORM(seq))
#define LINQ_FROM_SINGLE(seq) LINQ_SELECT_WHERE(BOOST_PP_SEQ_HEAD(seq))


#define LINQ_FROM_TRANSFORM(seq) BOOST_PP_SEQ_TRANSFORM(LINQ_FROM_OP, data, BOOST_PP_SEQ_POP_BACK(seq)) (LINQ_SELECT_WHERE(LINQ_BACK(seq)))

#define LINQ_FROM_P(s, data, x) LINQ_IS_EMPTY(x)
#ifndef _MSC_VER
#define LINQ_FROM_OP(s, data, x) LINQ_PROCESS_FROM LINQ_REM x
#else
// MSVC Workarounds
#define LINQ_FROM_OP(s, data, x) LINQ_FROM_OP_INVOKE(LINQ_REM x)
#define LINQ_FROM_OP_INVOKE_X(x) x
#define LINQ_FROM_OP_INVOKE(x) LINQ_FROM_OP_INVOKE_X(LINQ_PROCESS_FROM x)
#endif
#define LINQ_PROCESS_FROM(var, col) col | LINQ_SELECT_MANY(var, col)

// Transforms the sequence
#define LINQ_TRANSFORM(seq) LINQ_X(LINQ_FROM(seq))
// And finally the LINQ macro
#define LINQ(x) LINQ_TRANSFORM(LINQ_TO_SEQ(x))

#endif