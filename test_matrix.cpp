#include <gtest/gtest.h>
#include <sstream>
#include <list>
#include <tuple>
#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif
#include "matrix.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace std::literals::string_literals;

TEST(test_matrix, test_create)
{
    using namespace otus_hw6;
    Matrix<int, -1> matrix;
    EXPECT_EQ( matrix.size(), 0 );
    EXPECT_EQ( matrix.dimension_count, 2 );

    Matrix<int, -1, 1> vec;
    EXPECT_EQ( vec.size(), 0 );
    EXPECT_EQ( vec.dimension_count, 1 );

    Matrix<int, -1, 3> cube;
    EXPECT_EQ( cube.size(), 0 );
    EXPECT_EQ( cube.dimension_count, 3 );
}

TEST(test_matrix, test_dim2)
{
    using namespace otus_hw6;
    Matrix<int, -1> matrix;
    EXPECT_EQ( matrix.size(), 0 );
    EXPECT_EQ( matrix.dimension_count, 2 );
}

TEST(test_matrix, test_oper2)
{
    using namespace otus_hw6;
    Matrix<int, -1> matrix;
    EXPECT_EQ( matrix.size(), 0 );
    EXPECT_EQ( matrix.dimension_count, 2 );

    matrix[0][1] = 2;
    EXPECT_EQ( matrix.size(), 1 );
    
    EXPECT_EQ(matrix[0][1], 2);
    EXPECT_EQ(matrix[0][2], matrix.empty_value);

    auto cell = *matrix.begin();
    auto etalon_coord = decltype(matrix)::coord_t{0, 1};
    EXPECT_EQ(cell.coord_, etalon_coord);
    EXPECT_EQ(cell.val_, 2);

    for( auto cell: matrix )
    {
        for(auto coord_val: cell.coord_)
            std::cout << coord_val;        
        std::cout << cell.val_;
    }

    matrix[0][1] = matrix.empty_value;
    EXPECT_EQ(matrix.size(), 0 );

}

TEST(test_matrix, test_oper3)
{
    using namespace otus_hw6;
    Matrix<int, -1, 3> cube;
    EXPECT_EQ( cube.size(), 0 );
    EXPECT_EQ( cube.dimension_count, 3 );

    cube[0][1][4] = 2;
    EXPECT_EQ( cube.size(), 1 );
    
    EXPECT_EQ(cube[0][1][4], 2);
    EXPECT_EQ(cube[0][2][5], cube.empty_value);

    cube[0][1][4] = cube.empty_value;
    EXPECT_EQ( cube.size(), 0 );

    ((cube[1][4][8] = 2) = 4) = 8;
    cube[0][1][4] = 5;
    EXPECT_EQ(cube.size(), 2 );
    EXPECT_EQ(cube[1][4][8], 8);
    EXPECT_EQ(cube[0][1][4], 5);

}
