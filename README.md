Linq
====

Linq and list comprehension for C++:

```c#
std::vector<int> numbers = { 1, 2, 3, 4 };
auto r = LINQ(from(x, numbers) where(x > 2) select(x * x));
for (auto x : r) printf("%i\n", x);
```

The C++ code above will outputs this(yes that is C++ code above):

    9
    16


Motivation
----------

List comprehension can already be done using Boost.Range and lambda, like this:

```c#
auto r = numbers 
        | boost::adaptors::filtered([](int x) { return x > 2; }) 
        | boost::adaptors::transformed([](int x) { return x * x; });
```

However, this is can be very hard to read, and is filled with a lot of boilerplate lambda code. Also, if the type of the container is much longer or more complex it can get even harder to read. Linq provides syntatic surgar to make the statement shorter and more concise:

```c#
auto r = LINQ(from(x, numbers) where(x > 2) select(x * x));
```

This will deduce the type of the `numbers` range, and reuse that for all the lambdas. (This is why its important to put select at the end)

Usage
-----

To use Linq, just include the "linq.h" file. Then all your linq queries must be placed inside the `LINQ()` macro. The result of the linq query is a range of elements that match the query. All queries are lazily evaluated. Since there is no copy of the range, the query cannot outlive the range.


### from
All linq queries must start with a `from` statement. This specifies the variable name to be used for the lambdas and the container that the queries will be applied to. Also, multiple `from` statements can be used.

```c#
LINQ(from(variable, container) ... );
```

### where
The where clause returns the element that matches the predicate. It is optional but must come after a `from` clause and should be before a `select` clause if there is one.

 ```c#
LINQ(from(variable, container) where(predicate) ... );
```

### select
The select clause applies a transformation to the elements. It is optional also, but should be the very last clause.

 ```c#
LINQ( ... select(transformation) );
```

Requirements
------------

This will work on MSVC 2010 or later, clang, and gcc. The compiler needs to be set to C++11 mode, because it requires lambda support. It also requires boost. It was tested with boost 1.49, but may work on earlier versions. 

Limitations
-----------

Multiple `from` statements are implemented using nested lambdas. However, because of a [bug](http://connect.microsoft.com/VisualStudio/feedback/details/634688) in MSVC 2010, nested lambas don't work(This should be fixed in MSVC 2012, but I haven't tested it). If there is a way to implement transparent identifiers in C++, nested lambdas could be avoided.

Also, the `let`, `group` and `join` clauses aren't supported yet.

