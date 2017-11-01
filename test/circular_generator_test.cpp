#include "catch.hpp"

#include "mat/circular_generator.h"
using namespace mat;

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
}
