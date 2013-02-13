Linq
====

Linq for list comprehension in C++, provides an implementation of linq for C++. Currently it only supports C++ ranges, but it does support both the extension and query methods for linq. Here's an example:

```c#
struct student_t
{
    std::string last_name;
    std::vector<int> scores;
};

std::vector<student_t> students = 
{
    {"Omelchenko", {97, 72, 81, 60}},
    {"O'Donnell", {75, 84, 91, 39}},
    {"Mortensen", {88, 94, 65, 85}},
    {"Garcia", {97, 89, 85, 82}},
    {"Beebe", {35, 72, 91, 70}} 
};

auto scores = LINQ(from(student, students) 
                   from(score, student.scores) 
                   where(score > 90) 
                   select(std::make_pair(student.last_name, score)));

for (auto x : scores)
{
    printf("%s score: %i\n", x.first.c_str(), x.second);
}
```

The C++ code above will outputs this(yes that is C++ code above):

    Omelchenko score: 97
    O'Donnell score: 91
    Mortensen score: 94
    Garcia score: 97
    Beebe score: 91


Extension
---------

Extensions are implemented using the `|` pipe operator. This allows them to be applied to any range, without requiring inherting from some special base class. So extensions can work like this:

```c++
vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8 };
auto r = numbers 
        | linq::where([](int x) { return x > 2; }) 
        | linq::select([](int x) { return x * x; });
```
The extension methods supported are:

*   aggregate(reducer)
*   aggregate(seed, reducer)
*   aggregate(seed, reducer, selector)
*   all(predicate)
*   any(predicate)
*   average()
*   concat(range)
*   containe(element)
*   count()
*   count(predicat)
*   default_if_empty()
*   default_if_empty(default_value)
*   distinct()
*   element_at(index)
*   except(range)
*   find(element)
*   first()
*   first(predicate, value)
*   first_or_default()
*   first_or_default(predicate)
*   group_by(key_selector)
*   group_by(key_selector, element_selector)
*   group_join(range, outer_key_selector, inner_key_selector, result_selector)
*   intersect(range)
*   join(range, outer_key_selector, inner_key_selector, result_selector)
*   keys()
*   last()
*   last(predicate, value)
*   last_or_default()
*   last_or_default(predicate)
*   max()
*   min()
*   order_by(selector)
*   order_by_descending(selector)
*   reverse()
*   select(selector)
*   select_many(selector)
*   sequence_equal(range)
*   single()
*   single_or_default()
*   skip(count)
*   skip_while(predicate)
*   sum()
*   take(count)
*   take_while(predicate)
*   then_by(selector)
*   then_by_descending(selector)
*   to_container()
*   union(range)
*   values()
*   where(predicate)
*   zip(range)
*   zip(range, selector)

The library also provides a `range_extension` class, that can be used to write your own extensions, as well. First just define the function as a function object class, like this:
```c++
struct contains_t
{
    template<class Range, class T>
    bool operator()(Range && r, T && x) const
    { return (r | linq::find(x)) != boost::end(r); };
};
```
Then initialize the extension using static initialization:
```c++
range_extension<contains_t> contains = {};
```
Then the extension can be used like this:
```c++
if (numbers | contains(5)) printf("We have a 5");
```

Query
-----

### from
All linq queries must start with a `from` statement. This specifies the variable name to be used for the lambdas and the container that the queries will be applied to. Also, multiple `from` statements can be used.
```c#
struct student
{
    std::string name;
    std::vector<int> grades;

    student() {}

    template<class Range>
    student(std::string name, Range&& r)
    : name(name), grades(boost::begin(r), boost::end(r))
    {}
};

std::vector<student> students =
{
    student("Bob", { 90, 100, 75 })
    student("Tom", { 92, 81, 70 })
    student("Terry", { 105, 98, 94 })
};

// { 90, 100, 75, 92, 81, 70, 105, 98, 94 }
auto q = LINQ(from(s, students) from(g, s.grades) select(g));
```
### where
The where clause returns the element that matches the predicate. It is optional but must come after a `from` clause and should be before a `select` clause if there is one.

```c#
vector<int> v = { 1, 3, 4, 5 };
// { 1, 3, 5 }
auto q = LINQ(from(i, v) where(i % 2));
```

### select
The select clause applies a transformation to the elements. It is optional also, but should be the very last clause.

```c#
std::vector<int> v = { 1, 2, 4 };
// { 3, 6, 24 }
auto q = LINQ(from(x, v) select(x * 3));
```

### orderby
The `orderby` clause lets you specify elements to order the range by. Also `ascending` or `descending` can be preceded by the selector in order to specify the direction of ordering. The default is ascending.
```c#
struct person
{
    std::string name;
    int age;

    person() {}

    person(std::string name, int age)
    : name(name), age(age)
    {}
};

std::vector<person> people =
{
    person("Tom", 25),
    person("Bob", 22),
    person("Terry", 37),
    person("Jerry", 22)
}
//  { "Jerry", "Bob", "Tom", "Terry" }
auto q = LINQ(from(p, people) orderby(p.age, descending p.name) select(p.name));
```

### group
The `group` clause groups the elements of a sequence. The first parameter is the key selector, and the second one is the value selector.
```c#
struct person
{
    std::string name;
    int age;

    person() {}

    person(std::string name, int age)
    : name(name), age(age)
    {}
};

std::vector<person> people =
{
    person("Tom", 25),
    person("Bob", 22),
    person("Terry", 37),
    person("Jerry", 22)
}
auto q = LINQ(from(p, people) group(p.age, p.name));
```

Installation
------------

Just copy the `linq.h` and `linq` folder to the include directory for your compiler. Then just include the `linq.h` header, to include all of linq.

The `linq/extensions.h` header will just include whats needed for the extension methods, without the query syntax, and the `linq/query.h` will include the query syntax as well.

Requirements
------------

For full support it requires clang or gcc, and boost. There is partial support for visual studio. Visual studio doesn't support the `default_if_empty`, `group_by`, `group_join`, `join`, `order_by`, `select_many`, and `then_by` extensions, and it doesn't support `orderby`, `group`, and nested from clauses. Perhaps some visual studio wizards could help find workarounds for msvc bugs.  


Limitations
-----------

Multiple `from` statements are implemented using nested lambdas. However, because of a [bug](http://connect.microsoft.com/VisualStudio/feedback/details/634688) in MSVC 2010, nested lambas don't work(This should be fixed in MSVC 2012, but I haven't tested it). If there is a way to implement transparent identifiers in C++, nested lambdas could be avoided.

Also, the `let`, and `join` clauses aren't supported yet. The `into` clause can't be supported without the help of polymorhpic lambdas.

