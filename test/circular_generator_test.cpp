#include "catch.hpp"

#include "mat/circular_generator.h"
using namespace mat;

#include <array>
#include <vector>
#include <algorithm>
#include <iostream>


TEST_CASE("circular generator constructor exception", "[mat]")
{
    std::vector<int> v;  // empty sequence
    REQUIRE_THROWS_AS(circular_generator{ v }, std::range_error);
}

TEST_CASE("circular generator operator () ()", "[mat]")
{
    std::vector v{ 1, 2, 3 };
    circular_generator cg(v);
    CHECK(cg() == 1);
    CHECK(cg() == 2);
    CHECK(cg() == 3);
    CHECK(cg() == 1);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);

    circular_generator cg1{ std::vector{ 3, 2, 1 } };
    CHECK(cg1() == 3);
    CHECK(cg1() == 2);
    CHECK(cg1() == 1);
    CHECK(cg1() == 3);

    std::array<int, 3> arr{ 1, 2, 3 };
    circular_generator cg2(arr);
    CHECK(cg2() == 1);
    CHECK(cg2() == 2);
    CHECK(cg2() == 3);
    CHECK(cg2() == 1);
    CHECK(arr[0] == 1);
    CHECK(arr[1] == 2);
    CHECK(arr[2] == 3);

    circular_generator cg3{ std::array<int, 3>{ 3, 2, 1 } };
    CHECK(cg3() == 3);
    CHECK(cg3() == 2);
    CHECK(cg3() == 1);
    CHECK(cg3() == 3);
}

TEST_CASE("circular generator with std::generate", "[mat]")
{
    std::vector<int> v{ 7 };
    std::generate(v.begin(), v.end(), circular_generator{ std::vector{ 1, 2, 3 } });
    std::vector r{ 1, 2, 3, 1, 2, 3, 1 };
    CHECK(std::equal(v.begin(), v.end(), r.begin()));
}
