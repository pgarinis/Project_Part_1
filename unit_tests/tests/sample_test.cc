#include "gtest/gtest.h"
 
TEST(SampleTestSuite, SampleTestCasePass) { 
    EXPECT_EQ (18.0, 18);
}
 
TEST (SampleTestSuite, SampleTestCaseFail) { 
    ASSERT_EQ (0.0, 1.0);
   
}
 
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
