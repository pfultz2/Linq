/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    utility.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef INCLUDE_GUARD_LINQ_UTILITY_H
#define INCLUDE_GUARD_LINQ_UTILITY_H

#include <utility>
#include <functional>
#include <boost/utility/enable_if.hpp>

//
// LINQ_RETURNS for auto return type deduction.
//
#define LINQ_RETURNS(...) -> typename linq::detail::returns_<decltype(__VA_ARGS__)>::type { return (__VA_ARGS__); } static_assert(true, "")


#define LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) decltype(__VA_ARGS__)>::type { return __VA_ARGS__; }
#define LINQ_RETURN_REQUIRES(...) -> typename boost::enable_if<__VA_ARGS__, LINQ_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

namespace linq {

namespace detail{

template<class T>
struct returns_
{
    typedef T type;
};

template<class T>
struct returns_<T&&>
: returns_<T>
{};


}

// MSVC 2010 doesn't provide declval
// We also return T&& instead std::add_rvalue_reference<T>
// because MSVC has a buggy implementation of it.
// So, this function will work in all cases except when T
// is void(which should rarely happen).
template <typename T>
T&& declval(); // no definition required

template<class F>
struct protector : F
{
    template<class X>
    protector(X x) : F(x)
    {}
};

template<class F>
typename std::enable_if<std::is_bind_expression<typename std::decay<F>::type>::value,
protector<typename std::decay<F>::type> >::type protect(F&& f)
{
    return protector<typename std::decay<F>::type>(f);
}

template<class F>
typename std::enable_if<!std::is_bind_expression<typename std::decay<F>::type>::value,
F&& >::type protect(F&& f)
{
    return std::forward<F>(f);
}



template<class F>
struct is_callable {};

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes; 
    typedef long no; 
    template<class T> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<decltype( declval<U>()(declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef std::integral_constant<bool, value> type; 
};

template<class F>
struct sfinae_error
{
    F f;

    template<class X>
    sfinae_error(X x) : F(x)
    {}

    template<class X, class Enable = void>
    struct result
    {
        typedef void type;
    };

    template<class X, class... T>
    struct result<X(T...), typename std::enable_if<is_callable<X(T...)>::value>::type>
    {
        typedef decltype(declval<F>()(declval<T>()...)) type;
    };

    template<class... T>
    typename result<F(T&&...)>::type operator()(T &&... x)
    {
        return f(std::forward<T>(x)...);
    }
};

}

#endif