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
    EXPECT_TRUE( std::cout );
}

