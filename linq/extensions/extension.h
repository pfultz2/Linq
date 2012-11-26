/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    extension.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_EXTENSIONS_EXTENSION_H
#define LINQ_GUARD_EXTENSIONS_EXTENSION_H

#include <linq/pp.h>
#include <linq/utility.h>
#include <linq/traits.h>
#include <linq/extensions/detail/placeholders.h>
#include <utility>
#include <functional>

namespace linq { 

// 
// Range extension
//
#ifndef LINQ_LIMIT_EXTENSION
#define LINQ_LIMIT_EXTENSION 6
#endif
namespace detail {
struct na {};


template<class F>
struct pipe_closure
{
    F f;
    pipe_closure(F f) : f(f)
    {};

    template<class Range>
    friend typename std::result_of<F(Range&&)>::type 
    operator|(Range&& r, const pipe_closure& p)
    {
        return p.f(r);
    }
};

template<class F>
pipe_closure<F> make_pipe_closure(F f)
{
    return pipe_closure<F>(f);
}

template<class F>
struct bind_wrapper : F
{
    template<class X>
    bind_wrapper(X x) : F(x)
    {}
};

template<class T, class Enable = void>
struct auto_ref_type
{
    typedef T type;
};

template<class T>
struct auto_ref_type<T, typename std::enable_if<std::is_bind_expression<T>::value>::type>
{
    typedef bind_wrapper<typename std::decay<T>::type> type;
};

template<class T>
struct auto_ref_type<T&&>
: auto_ref_type<T>
{};

template<class T>
struct auto_ref_type<T&, typename std::enable_if<!std::is_bind_expression<T>::value>::type>
{
    typedef std::reference_wrapper<T> type;
};

template<class T>
struct auto_ref_type<const T&, typename std::enable_if<!std::is_bind_expression<T>::value>::type>
{
    typedef std::reference_wrapper<const T> type;
};


template<class T>
typename detail::auto_ref_type<T>::type auto_ref(T&& x)
{
    return typename detail::auto_ref_type<T>::type(std::forward<T>(x));
}

// template<class T>
// struct is_bw
// : boost::mpl::bool_<false>
// {};

// template<class T>
// struct is_bw<bind_wrapper<T> >
// : boost::mpl::bool_<true>
// {};

// struct foo
// {
//     template<class T, class U>
//     void operator()(T x, U y) const
//     {
//     }
// };

// static_assert(is_bw<decltype(auto_ref(std::bind(foo(), 1, linq::_1)))>::value, "Error");
// static_assert(!std::is_bind_expression<decltype(auto_ref(std::bind(foo(), 1, linq::_1)))>::value, "Error");
// static_assert(!is_bw<decltype(auto_ref(1))>::value, "Error");

}

#define LINQ_RANGE_EXTENSION_AUTO_REF(z, n, data) detail::auto_ref(std::forward<T ## n>(x ## n))

#define LINQ_RANGE_EXTENSION_OP(z, n, data) \
    template<BOOST_PP_ENUM_PARAMS_Z(z, n, class T)> \
    auto operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, n, T, && x) ) const LINQ_RETURNS \
    ( \
        detail::make_pipe_closure \
        (std::bind(F(), linq::_1, BOOST_PP_ENUM_ ## z(n, LINQ_RANGE_EXTENSION_AUTO_REF, ~) ) ) \
    ); 

template<class F>
struct range_extension
{
    BOOST_PP_REPEAT_FROM_TO_1(1, LINQ_LIMIT_EXTENSION, LINQ_RANGE_EXTENSION_OP, ~)
    template<class Range>
    friend auto operator|(Range && r, const range_extension) LINQ_RETURN_REQUIRES(is_range<Range>)
    (F()(std::forward<Range>(r)));

    range_extension<F>& operator()()
    {
        return *this;
    }

    const range_extension<F>& operator()() const
    {
        return *this;
    }
};

}

#endif
