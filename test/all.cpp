
#include <linq/extensions.h>
#include <linq/query.h>

#include <boost/assign.hpp>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <boost/foreach.hpp>

static_assert(linq::is_range<std::vector<int>&>::value, "error");

#define BOOST_TEST_MODULE LinqExtensions
#include <boost/test/included/unit_test.hpp>

using boost::assign::list_of;
using boost::assign::map_list_of;

struct output_elem
{
    typedef void result_type;
    template<class Stream, class T>
    void operator()(Stream && s, const T& x)
    {
        s << x << ", ";
    }
};

template<class Stream, class Range>
void output_range(Stream && s, Range && r)
{
    s << "(";
    std::for_each(boost::begin(r), boost::end(r), boost::bind(output_elem(), boost::ref(s), boost::arg<1>()));
    s << ")";
}

template<class Range1, class Range2>
boost::test_tools::predicate_result
compare_seq( const Range1& r1, const Range2& r2 )
{
    if(!(r1 | linq::sequence_equal(r2)) ) 
    {
        boost::test_tools::predicate_result res( false );

        res.message() << "Different sequences\n    [";
        output_range(res.message(), r1);
        res.message() << "!=";
        output_range(res.message(), r2);
        res.message() << "]";

        return res;
    }

    return true;
}

#define CHECK_SEQ(x, y) BOOST_CHECK(compare_seq(x, y))

struct odd
{
    template<typename T>
    bool operator()(T t) const
    {
        return t % 2;
    }
};

struct person
{
    std::string name;
    int age;

    person() {}

    person(std::string name, int age)
    : name(name), age(age)
    {}
};

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

struct pet
{
    std::string name;
    std::string owner;

    pet() {}

    pet(std::string name, std::string owner)
    : name(name), owner(owner)
    {}
};

struct name_selector
{
    template<class T>
    auto operator()(T && x) const LINQ_RETURNS
    (x.name);
};
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( query_group_by_test )
{
    std::vector<person> v = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    auto q = LINQ(from(p, v) group(p.age, p.name));
    BOOST_CHECK_EQUAL(1, boost::distance(q.equal_range(25)));
    BOOST_CHECK_EQUAL(2, boost::distance(q.equal_range(22)));
    BOOST_CHECK_EQUAL(1, boost::distance(q.equal_range(37)));

    BOOST_CHECK_EQUAL("Tom", q.equal_range(25).first->second);
    BOOST_CHECK_EQUAL("Terry", q.equal_range(37).first->second);
    BOOST_CHECK(q.equal_range(22) | linq::values | linq::contains("Bob") );
    BOOST_CHECK(q.equal_range(22) | linq::values | linq::contains("Jerry") );
}

BOOST_AUTO_TEST_CASE( query_order_by_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<int> people_age = list_of(22)(22)(25)(37);

    CHECK_SEQ(people_age, LINQ(from(p, people) orderby(p.age) select(p.age)));
    CHECK_SEQ(people_age, LINQ(from(p, people) orderby(ascending p.age) select(p.age)));
    CHECK_SEQ(people_age | linq::reverse, LINQ(from(p, people) orderby(descending p.age) select(p.age)));
}

BOOST_AUTO_TEST_CASE( query_select_many_test )
{
    std::vector<student> students = list_of
    (student("Bob", list_of(90)(100)(75)))
    (student("Tom", list_of(92)(81)(70)))
    (student("Terry", list_of(105)(98)(94)));
    std::vector<int> r = list_of(90)(100)(75)
    (92)(81)(70)
    (105)(98)(94);

    auto q = LINQ(from(s, students) from(g, s.grades) select(g));
    CHECK_SEQ(r, q);
}
#endif
BOOST_AUTO_TEST_CASE( query_select_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    std::vector<int> r = list_of(3)(6)(9)(12);
    CHECK_SEQ(r, LINQ(from(i, v) select(i * 3)));
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( query_then_by_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<std::string> people_name = list_of("Bob")("Jerry")("Tom")("Terry");
    std::vector<std::string> people_name_d = list_of("Jerry")("Bob")("Tom")("Terry");

    CHECK_SEQ(people_name, LINQ(from(p, people) orderby(p.age, p.name) select(p.name)));
    CHECK_SEQ(people_name, LINQ(from(p, people) orderby(ascending p.age, ascending p.name) select(p.name)));
    CHECK_SEQ(people_name_d, LINQ(from(p, people) orderby(p.age, descending p.name) select(p.name)));
}
#endif
BOOST_AUTO_TEST_CASE( query_where_test )
{
    std::vector<int> v = list_of(1)(3)(4)(5);
    std::vector<int> r = list_of(1)(3)(5);
    CHECK_SEQ(r, LINQ(from(i, v) where(i % 2)));
}

BOOST_AUTO_TEST_CASE( aggregate_test )
{
    // the quick brown fox jumps over the lazy dog
    std::vector<std::string> words = list_of
    ("the")
    ("quick")
    ("brown")
    ("fox")
    ("jumps")
    ("over")
    ("the")
    ("lazy")
    ("dog");

    std::string reversed = words | linq::aggregate([](const std::string& workingSetence, const std::string& next)
    {
        return next + " " + workingSetence;
    });

    BOOST_CHECK_EQUAL( reversed, "dog lazy the over jumps fox brown quick the" );
}

BOOST_AUTO_TEST_CASE( all_test )
{
    std::vector<int> v1 = list_of(1)(3);
    std::vector<int> v2 = list_of(1)(2);
    BOOST_CHECK(v1 | linq::all(odd()));
    BOOST_CHECK(!(v2 | linq::all(odd())));
}

BOOST_AUTO_TEST_CASE( any_test )
{
    std::vector<int> v1 = list_of(1)(2);
    std::vector<int> v2 = list_of(2)(4);
    BOOST_CHECK(v1 | linq::any(odd()));
    BOOST_CHECK(!(v2 | linq::any(odd())));
}


BOOST_AUTO_TEST_CASE( average_test )
{
    // 78, 92, 100, 37, 81
    std::vector<int> grades = list_of(78)(92)(100)(37)(81);
    BOOST_CHECK_EQUAL(77.6, grades | linq::average);
}

BOOST_AUTO_TEST_CASE( concat_test )
{
    std::vector<int> v1 = list_of(1)(2);
    std::vector<int> v2 = list_of(3)(4);
    std::vector<int> v = list_of(1)(2)(3)(4);
    BOOST_CHECK(v | linq::sequence_equal(v1 | linq::concat(v2)));
}

BOOST_AUTO_TEST_CASE( contains_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    BOOST_CHECK(v | linq::contains(3));
}

BOOST_AUTO_TEST_CASE( contains_string_test )
{
    std::string s = "Hello World!";
    BOOST_CHECK(s | linq::contains("World"));
    BOOST_CHECK(!(s | linq::contains("foo")));
}

BOOST_AUTO_TEST_CASE( count_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    BOOST_CHECK_EQUAL(2, v | linq::count(odd()));
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( default_if_empty_test )
{
    std::vector<int> non_empty = list_of(1)(2);
    std::vector<int> empty;
    std::vector<int> default_empty = list_of(5);
    BOOST_CHECK(non_empty | linq::default_if_empty(5) | linq::sequence_equal(non_empty));
    BOOST_CHECK(empty | linq::default_if_empty(5) | linq::sequence_equal(default_empty));
}
#endif
BOOST_AUTO_TEST_CASE( distinct_test )
{
    std::vector<int> v = list_of(1)(2)(2)(3)(2)(4)(5)(5);
    std::vector<int> d = list_of(1)(2)(3)(4)(5);
    CHECK_SEQ(d, v | linq::distinct);
}

BOOST_AUTO_TEST_CASE( element_at_test )
{
    std::vector<int> v = list_of(0)(1)(2)(3)(4)(5);
    BOOST_CHECK_EQUAL(4, v | linq::element_at(4));
}

BOOST_AUTO_TEST_CASE( except_test )
{
    std::vector<int> v1 = list_of(1)(2)(3)(4)(5);
    std::vector<int> v2 = list_of(2)(4);
    std::vector<int> e = list_of(1)(3)(5);
    BOOST_CHECK(v1 | linq::except(v2) | linq::sequence_equal(e));
}

BOOST_AUTO_TEST_CASE( find_test )
{
    std::vector<int> v = list_of(0)(1)(2)(3)(4)(5);
    BOOST_CHECK_EQUAL(4, *(v | linq::find(4)));
}

BOOST_AUTO_TEST_CASE( find_map_test )
{
    std::map<int, int> m = map_list_of(0, 1)(1, 2)(2, 3)(3, 4)(4, 5)(5, 6);
    BOOST_CHECK_EQUAL(5, (m | linq::find(4))->second);
}

BOOST_AUTO_TEST_CASE( first_test )
{
    std::vector<int> v = list_of(2)(3)(4)(5);
    BOOST_CHECK_EQUAL(2, v | linq::first);
    BOOST_CHECK_EQUAL(3, v | linq::first(odd()));
    // BOOST_CHECK_EQUAL(0, v | linq::first([](int x) { return x > 5; }));
}

BOOST_AUTO_TEST_CASE( first_or_default_test )
{
    std::vector<int> v = list_of(2)(3)(4)(5);
    std::vector<int> empty_v;
    BOOST_CHECK_EQUAL(2, v | linq::first_or_default);
    BOOST_CHECK_EQUAL(0, empty_v | linq::first_or_default);
    BOOST_CHECK_EQUAL(3, v | linq::first_or_default(odd()));
    BOOST_CHECK_EQUAL(0, v | linq::first_or_default([](int x) { return x > 5; }));
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( group_by_test )
{
    std::vector<person> v = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    auto q = v | linq::group_by([](person p){ return p.age; }, [](person p) { return p.name; });
    BOOST_CHECK_EQUAL(1, boost::distance(q.equal_range(25)));
    BOOST_CHECK_EQUAL(2, boost::distance(q.equal_range(22)));
    BOOST_CHECK_EQUAL(1, boost::distance(q.equal_range(37)));

    BOOST_CHECK_EQUAL("Tom", q.equal_range(25).first->second);
    BOOST_CHECK_EQUAL("Terry", q.equal_range(37).first->second);
    BOOST_CHECK(q.equal_range(22) | linq::values | linq::contains("Bob") );
    BOOST_CHECK(q.equal_range(22) | linq::values | linq::contains("Jerry") );


}
#endif
struct group_join_select
{
    template<class Person, class Pets>
    auto operator()(Person && p, Pets && pets) const LINQ_RETURNS
    (
        std::make_pair(p.name, pets | linq::select(name_selector()))
    );

};
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( group_join_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<pet> pets = list_of
    (pet("Barry", "Terry"))
    (pet("Betty", "Terry"))
    (pet("Willie", "Bob"))
    (pet("Dan", "Jerry"));

    auto q = people | linq::group_join(pets, 
        [](person p) { return p.name; },
        [](pet p) { return p.owner; },
        group_join_select());

    BOOST_CHECK_EQUAL(people.size(), boost::distance(q));

    BOOST_FOREACH(auto x, q)
    {
        if (x.first == "Tom")
        {
            BOOST_CHECK_EQUAL(0, boost::distance(x.second));
        }

        if (x.first == "Bob")
        {
            BOOST_CHECK_EQUAL(1, boost::distance(x.second));
            BOOST_CHECK_EQUAL("Willie", x.second | linq::first);
        }

        if (x.first == "Terry")
        {
            BOOST_CHECK_EQUAL(2, boost::distance(x.second));
            BOOST_CHECK(x.second | linq::contains("Barry"));
            BOOST_CHECK(x.second | linq::contains("Betty"));
        }

        if (x.first == "Jerry")
        {
            BOOST_CHECK_EQUAL(1, boost::distance(x.second));
            BOOST_CHECK_EQUAL("Dan", x.second | linq::first);
        }
    }
}

BOOST_AUTO_TEST_CASE( join_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<pet> pets = list_of
    (pet("Barry", "Terry"))
    (pet("Betty", "Terry"))
    (pet("Willie", "Bob"))
    (pet("Dan", "Jerry"));

    auto q = people | linq::join(pets, 
        [](person p) { return p.name; },
        [](pet p) { return p.owner; },
        [](person, pet p) { return p.name; });

    std::vector<std::string> r1 = list_of
    ("Willie")
    ("Barry")
    ("Betty")
    ("Dan");

    std::vector<std::string> r2 = list_of
    ("Willie")
    ("Betty")
    ("Barry")
    ("Dan");

    // output_range(std::cout, q);

    BOOST_CHECK
    (
        (r1 | linq::sequence_equal(q)) ||
        (r2 | linq::sequence_equal(q))
    );
}
#endif
BOOST_AUTO_TEST_CASE( intersect_test )
{
    std::vector<int> v1 = list_of(1)(2)(3)(4)(5);
    std::vector<int> v2 = list_of(2)(4);
    std::vector<int> i = list_of(2)(4);
    BOOST_CHECK(v1 | linq::intersect(v2) | linq::sequence_equal(i));
}

BOOST_AUTO_TEST_CASE( keys_test )
{
    std::map<int, int> m = map_list_of(1, 10)(2, 20)(3, 30);
    std::vector<int> k = list_of(1)(2)(3);
    BOOST_CHECK(m | linq::keys | linq::sequence_equal(k));
}

BOOST_AUTO_TEST_CASE( last_test )
{
    std::vector<int> v = list_of(2)(3)(4)(5)(6);
    BOOST_CHECK_EQUAL(6, v | linq::last);
    BOOST_CHECK_EQUAL(5, v | linq::last(odd()));
    // BOOST_CHECK_EQUAL(0, v | linq::last([](int x) { return x > 6; }));
}

BOOST_AUTO_TEST_CASE( last_or_default_test )
{
    std::vector<int> v = list_of(2)(3)(4)(5)(6);
    std::vector<int> empty_v;
    BOOST_CHECK_EQUAL(6, v | linq::last_or_default);
    BOOST_CHECK_EQUAL(0, empty_v | linq::last_or_default);
    BOOST_CHECK_EQUAL(5, v | linq::last_or_default(odd()));
    BOOST_CHECK_EQUAL(0, v | linq::last_or_default([](int x) { return x > 6; }));
}

BOOST_AUTO_TEST_CASE( max_test )
{
    std::vector<int> v = list_of(2)(3)(8)(1)(4);
    BOOST_CHECK_EQUAL(8, v | linq::max);
}

BOOST_AUTO_TEST_CASE( min_test )
{
    std::vector<int> v = list_of(2)(3)(8)(1)(4);
    BOOST_CHECK_EQUAL(1, v | linq::min);
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( order_by_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<int> people_age = list_of(22)(22)(25)(37);

    auto age_select = [](person p) { return p.age; };

    CHECK_SEQ(people_age, people | linq::order_by(age_select) | linq::select(age_select));
    CHECK_SEQ(people_age | linq::reverse, people | linq::order_by_descending(age_select) | linq::select(age_select));
}
#endif
BOOST_AUTO_TEST_CASE( reverse_test )
{
    std::vector<int> v1 = list_of(3)(2)(1);
    std::vector<int> v2 = list_of(1)(2)(3);
    BOOST_CHECK(v1 | linq::reverse | linq::sequence_equal(v2));
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( select_many_test )
{
    std::vector<student> students = list_of
    (student("Bob", list_of(90)(100)(75)))
    (student("Tom", list_of(92)(81)(70)))
    (student("Terry", list_of(105)(98)(94)));
    std::vector<int> r = list_of(90)(100)(75)
    (92)(81)(70)
    (105)(98)(94);
    CHECK_SEQ(r, students | linq::select_many([](student& s) { return std::ref(s.grades); }));
    CHECK_SEQ(r, students | linq::select_many([](student& s) { return s.grades | linq::select([](int& g){ return std::ref(g); }); }));
}
#endif
BOOST_AUTO_TEST_CASE( select_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    std::vector<int> r = list_of(3)(6)(9)(12);
    BOOST_CHECK(v | linq::select([](int i) { return i * 3; }) | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( single_test )
{
    std::vector<int> v = list_of(1);
    BOOST_CHECK_EQUAL(1, v | linq::single);
}

BOOST_AUTO_TEST_CASE( single_or_default_test )
{
    std::vector<int> v = list_of(1);
    std::vector<int> empty_v;
    BOOST_CHECK_EQUAL(1, v | linq::single_or_default(0));
    BOOST_CHECK_EQUAL(0, empty_v | linq::single_or_default(0));
}

BOOST_AUTO_TEST_CASE( skip_test )
{
    std::vector<int> v = list_of(0)(1)(2)(3)(4)(5);
    std::vector<int> r = list_of(4)(5);
    CHECK_SEQ(r, v | linq::skip(4));
}

BOOST_AUTO_TEST_CASE( skip_while_test )
{
    std::vector<int> v = list_of(1)(3)(4)(5);
    std::vector<int> r = list_of(4)(5);
    CHECK_SEQ(r, v | linq::skip_while(odd()));
}

BOOST_AUTO_TEST_CASE( sum_test )
{
    std::vector<int> v = list_of(1)(2)(3);
    BOOST_CHECK_EQUAL(6, v | linq::sum);
}

BOOST_AUTO_TEST_CASE( take_test )
{
    std::vector<int> v = list_of(0)(1)(2)(3)(4)(5);
    std::vector<int> r = list_of(0)(1)(2)(3);
    BOOST_CHECK(v | linq::take(4) | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( take_while_test )
{
    std::vector<int> v = list_of(1)(3)(4)(5);
    std::vector<int> r = list_of(1)(3);
    BOOST_CHECK(v | linq::take_while(odd()) | linq::sequence_equal(r));
}
#ifndef _MSC_VER
BOOST_AUTO_TEST_CASE( then_by_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<std::string> people_name = list_of("Bob")("Jerry")("Tom")("Terry");
    std::vector<std::string> people_name_d = list_of("Jerry")("Bob")("Tom")("Terry");

    auto age_select = [](person p) { return p.age; };
    auto name_select = [](person p) { return p.name; };

    CHECK_SEQ(people_name, people | linq::order_by(age_select) | linq::then_by(name_select) | linq::select(name_select));
    CHECK_SEQ(people_name_d, people | linq::order_by(age_select) | linq::then_by_descending(name_select) | linq::select(name_select));
}
#endif
BOOST_AUTO_TEST_CASE( to_container_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    std::list<int> l = v | linq::select([](int i) { return i * 3; }) | linq::to_container;
    std::vector<int> r = list_of(3)(6)(9)(12);
    BOOST_CHECK(l | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( union_test )
{
    std::vector<int> v1 = list_of(1)(3)(5)(7)(9);
    std::vector<int> v2 = list_of(2)(3)(5)(7)(11);
    std::vector<int> r = list_of(1)(3)(5)(7)(9)(2)(11);

    std::vector<int> x = list_of(1)(3)(5)(7)(9)(2)(3)(5)(7)(11);
    CHECK_SEQ(x, v1 | linq::concat(v2));
    CHECK_SEQ(r, x | linq::distinct);

    CHECK_SEQ(r, v1 | linq::union_(v2));
}

// BOOST_AUTO_TEST_CASE( to_string_test )
// {
//     std::string lower = "test";
//     std::string upper = lower | linq::select([](char c) { return toupper(c); }) | linq::to_string;
//     BOOST_CHECK_EQUAL(upper, "TEST");
// }

BOOST_AUTO_TEST_CASE( values_test )
{
    std::map<int, int> m = map_list_of(1, 10)(2, 20)(3, 30);
    std::vector<int> v = list_of(10)(20)(30);
    CHECK_SEQ(v, m | linq::values);
}

BOOST_AUTO_TEST_CASE( where_test )
{
    std::vector<int> v = list_of(1)(3)(4)(5);
    std::vector<int> r = list_of(1)(3)(5);
    BOOST_CHECK(v | linq::where(odd()) | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( zip_test )
{
    std::vector<int> v1 = list_of(1)(2)(3);
    std::vector<int> v2 = list_of(10)(20)(30);
    std::vector<int> r = list_of(11)(22)(33);

    BOOST_CHECK(v1 | linq::zip(v2, [](int x, int y) { return x + y; }) | linq::sequence_equal(r));
}


