Linq
====

Linq and list comprehension for C++:

```c#
std::vector<int> numbers = { 1, 2, 3, 4 };
auto r = LINQ(from(x, numbers) where(x > 2) select(x * x));
for (auto x : r) printf("%i\n", x);
```

Outputs:

    9
    16


Motivation
----------

List comprehension can already be done using Boost.Range and lambda, like this:

```c#
auto r = numbers | boost::adaptors::filtered([](int x) { return x > 2; }) | boost::adaptors::transformed([](int x) { return x * x; });
```

However, this is can be very hard to read, and is filled with a lot of boilerplate lambda code. Also, if the type of the container is much longer or more complex it can get even harder to read. Linq provides syntatic surger to make the statement shorter and more concise:

```c#
auto r = LINQ(from(x, numbers) where(x > 2) select(x * x));
```

This will dedcue the type of the `numbers` range, and reuse that for all the lambdas. (This is why its important to put select at the end)

Requirements
------------

This will work on MSVC 2010 or later, clang, and gcc. The compiler needs to be set to C++11 mode, because it requires lambda support. It also requires boost. It was tested with boost 1.49, but may work on earlier versions. 

Limitations
-----------

As of now, it doesn't support cartesian products. I hope to add that in the future(any help on developing a cartesian product iterator would be helpful). So you cannot write this:

```c#
//This is not supported yet
LINQ(from(x, list1) from(y, list2) select(x*y));
```

Also, the `sortedby` clause is not suported yet.

