#include "catch.hpp"

#include "mat/mat.h"
using namespace mat;


TEST_CASE("det", "[mat]")
{
    //==== C++ built-in array ====

    // compile-time calculation
    constexpr int mat[2][2] = { { 3, 0 },
                                { 0, 2 } };
    static_assert(det(mat) == 6);

    // runtime calculation
    int mat1[2][2] = { { 3, 0 },
                       { 0, 2 } };
    REQUIRE(det(mat1) == 6);

    //==== std::array ====
    using mat_t = std::array<std::array<int, 2>, 2>;

    // compile-time calculation
    constexpr mat_t mat2 = { { { 3, 0 },
                               { 0, 2 } } };
    static_assert(det(mat2) == 6);

    // runtime calculation
    mat_t mat3 = { { { 3, 0 },
                     { 0, 2 } } };
    REQUIRE(det(mat3) == 6);

    //==== std::vector ====
    std::vector<std::vector<int>> mat4{ { 3, 0 },
                                        { 0, 2 } };
    REQUIRE(mat4.size() == 2);
    REQUIRE(mat4[0][0] == 3);
    REQUIRE(mat4[0][1] == 0);
    REQUIRE(mat4[1][0] == 0);
    REQUIRE(mat4[1][1] == 2);

    // runtime calculation
    REQUIRE((det<2>(mat4) == 6));   
}
