#include <gtest/gtest.h>

class main_test : public ::testing::Test
{
public:
    main_test() {}
};

TEST_F(main_test, dummy){
    EXPECT_TRUE(false);
}