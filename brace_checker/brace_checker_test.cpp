#include "brace_checker.h"
#include <gtest/gtest.h>

using namespace algorithms;

class brace_checker_test : public ::testing::Test {
  public:
    // brace_checker_test() = default;
};

TEST_F(brace_checker_test, check_braces_on_empty_string) {
    EXPECT_TRUE(check_brace_correctness(""));
    EXPECT_TRUE(check_brace_correctness("xdsadsa"));
    EXPECT_FALSE(check_brace_correctness("{"));
    EXPECT_TRUE(check_brace_correctness("{}"));
    EXPECT_FALSE(check_brace_correctness("}"));
    EXPECT_FALSE(check_brace_correctness("{[}]"));
    EXPECT_TRUE(check_brace_correctness("{aa[dss(xxx)(sss)]dd[ss{dd}dd]aa}"));
}
