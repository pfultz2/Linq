
#include <linq/extensions.h>

#include <boost/assign.hpp>
#include <string>
#include <vector>
#include <map>

#define BOOST_TEST_MODULE LinqExtensions
#include <boost/test/included/unit_test.hpp>

using boost::assign::list_of;
using boost::assign::map_list_of;

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

BOOST_AUTO_TEST_CASE( count_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    BOOST_CHECK_EQUAL(2, v | linq::count(odd()));
}

BOOST_AUTO_TEST_CASE( default_if_empty_test )
{
    std::vector<int> non_empty = list_of(1)(2);
    std::vector<int> empty;
    std::vector<int> default_empty = list_of(5);
    BOOST_CHECK(non_empty | linq::default_if_empty(5) | linq::sequence_equal(non_empty));
    BOOST_CHECK(empty | linq::default_if_empty(5) | linq::sequence_equal(default_empty));
}

BOOST_AUTO_TEST_CASE( distinct_test )
{
    std::vector<int> v = list_of(1)(2)(2)(3)(2)(4)(5)(5);
    std::vector<int> d = list_of(1)(2)(3)(4)(5);
    BOOST_CHECK(v | linq::distinct | linq::sequence_equal(d));
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

// BOOST_AUTO_TEST_CASE( find_map_test )
// {
//     std::map<int, int> m = map_list_of(0, 1)(1, 2)(2, 3)(3, 4)(4, 5)(5, 6);
//     BOOST_CHECK_EQUAL(5, (m | linq::find(4))->second);
// }

BOOST_AUTO_TEST_CASE( first_test )
{
    std::vector<int> v = list_of(2)(3)(4)(5);
    BOOST_CHECK_EQUAL(2, v | linq::first);
    BOOST_CHECK_EQUAL(3, v | linq::first(odd(), 0));
    BOOST_CHECK_EQUAL(0, v | linq::first([](int x) { return x > 5; } , 0));
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

    // auto q = linq::detail::group_join_t()(people, pets, 
    //     [](person p) { return p.name; },
    //     [](pet p) { return p.owner; },
    //     linq::detail::identity_selector());

    // auto q = people | linq::group_join(pets, 
    //     [](person p) { return p.name; },
    //     [](pet p) { return p.owner; },
    //     linq::detail::identity_selector());


}

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
    BOOST_CHECK_EQUAL(5, v | linq::last(odd(), 0));
    BOOST_CHECK_EQUAL(0, v | linq::last([](int x) { return x > 6; } , 0));
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

BOOST_AUTO_TEST_CASE( order_by_test )
{
    std::vector<person> people = list_of
    (person("Tom", 25))
    (person("Bob", 22))
    (person("Terry", 37))
    (person("Jerry", 22));

    std::vector<int> people_age = list_of(22)(22)(25)(37);

    auto age_select = [](person p) { return p.age; };

    // BOOST_CHECK(people | linq::order_by(age_select) | linq::select(age_select) | linq::sequence_equal(people_age));
}

BOOST_AUTO_TEST_CASE( reverse_test )
{
    std::vector<int> v1 = list_of(3)(2)(1);
    std::vector<int> v2 = list_of(1)(2)(3);
    BOOST_CHECK_EQUAL(1, v1 | linq::reverse | linq::sequence_equal(v2));
}

BOOST_AUTO_TEST_CASE( select_many_test )
{
    std::vector<student> students = list_of
    (student("Bob", list_of(90)(100)(75)))
    (student("Tom", list_of(92)(81)(70)))
    (student("Terry", list_of(105)(98)(94)));
    std::vector<int> r = list_of(90)(100)(75)
    (92)(81)(70)
    (105)(98)(94);
    BOOST_CHECK(students | linq::select_many([](student s) { return s.grades; }) | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( select_test )
{
    std::vector<int> v = list_of(1)(2)(3)(4);
    std::vector<int> r = list_of(3)(6)(9)(12);
    BOOST_CHECK(v | linq::select([](int i) { return i * 3; }) | linq::sequence_equal(r));
}

BOOST_AUTO_TEST_CASE( values_test )
{
    std::map<int, int> m = map_list_of(1, 10)(2, 20)(3, 30);
    std::vector<int> v = list_of(10)(20)(30);
    BOOST_CHECK(m | linq::keys | linq::sequence_equal(v));
}


