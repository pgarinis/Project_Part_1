#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <cinttypes>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include "../../include/JoinEngine.h"

using namespace std;

TEST(LoadRelationsTest, LoadRelationsToMemory) {
  char const *argv[4];
  argv[1] = "../datasets/r0";
  argv[2] = "0";
  argv[3] = "../datasets/r1";
  argv[4] = "1";


  JoinEngine *joinEngine = new JoinEngine(argv);
  EXPECT_EQ(joinEngine->load_relations(),0);
  EXPECT_TRUE(joinEngine->get_relations()[1] != NULL);
  EXPECT_TRUE(strcmp("../datasets/r1",joinEngine->get_relations()[1]->get_name().c_str()) == 0);
  EXPECT_TRUE(joinEngine->get_relations()[1] != NULL);
  EXPECT_TRUE(joinEngine->get_relations()[1]->get_num_of_records() > 0);
  EXPECT_TRUE(joinEngine->get_relations()[1]->get_column() != NULL);
  EXPECT_TRUE(joinEngine->get_relations()[1]->get_column()[0] > 0);
  delete joinEngine;
}

TEST(SegmentationTest, ComputeHistArray) {
  char const *argv[4];
  argv[1] = "relation0";
  argv[2] = "0";
  argv[3] = "relation1";
  argv[4] = "1";

  /* Initialize join engine with a mock relation */
  JoinEngine *joinEngine = new JoinEngine(argv);
  joinEngine->get_relations()[0]->set_num_of_records(3);
  size_t column_size = sizeof(uint64_t) * joinEngine->get_relations()[0]->get_num_of_records();
  joinEngine->get_relations()[0]->set_column(column_size);
  joinEngine->get_relations()[0]->get_column()[0] = (uint64_t) 17; // 10001
  joinEngine->get_relations()[0]->get_column()[8] = (uint64_t) 16; // 10000
  joinEngine->get_relations()[0]->get_column()[16] = (uint64_t) 15; // 01111

  // hist array expectations
  EXPECT_EQ(joinEngine->create_and_compute_hist_array(joinEngine->get_relations()[0]),0);
  EXPECT_TRUE(sizeof(joinEngine->get_relations()[0]->get_hist_array()) > 0);
  EXPECT_TRUE(joinEngine->get_relations()[0]->get_hist_array()[1] == 1);
  // must check more


  delete joinEngine;
}

TEST(SegmentationTest, ComputePsumArray) {
  char const *argv[4];
  argv[1] = "relation0";
  argv[2] = "0";
  argv[3] = "relation1";
  argv[4] = "1";

  /* Initialize join engine with a mock relation */
  JoinEngine *joinEngine = new JoinEngine(argv);
  joinEngine->get_relations()[0]->set_num_of_records(3);
  size_t column_size = sizeof(uint64_t) * joinEngine->get_relations()[0]->get_num_of_records();
  joinEngine->get_relations()[0]->set_column(column_size);
  joinEngine->get_relations()[0]->get_column()[0] = (uint64_t) 17; // 10001
  joinEngine->get_relations()[0]->get_column()[8] = (uint64_t) 16; // 10000
  joinEngine->get_relations()[0]->get_column()[16] = (uint64_t) 15; // 01111

  // hist array expectations
  EXPECT_EQ(joinEngine->create_and_compute_psum_array(joinEngine->get_relations()[0]),0);
  EXPECT_TRUE(sizeof(joinEngine->get_relations()[0]->get_psum_array()) > 0);
  EXPECT_TRUE(joinEngine->get_relations()[0]->get_psum_array()[0] == -1);
  EXPECT_TRUE(joinEngine->get_relations()[0]->get_psum_array()[1] > -1);
  // must check more


  delete joinEngine;
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
