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
    EXPECT_EQ( vec.dimension_count, 0 );

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

    matrix.get_dim(0).get_dim(1);

}
