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
  argv[1] = "r0";
  argv[2] = "f0";
  argv[3] = "r1";
  argv[4] = "f1";


  JoinEngine *joinEngine = new JoinEngine(argv);
  EXPECT_EQ(joinEngine->load_relations(),0);
  //EXPECT_TRUE(strcmp(joinEngine->relations[1]->get_name(),"r1"));

  //delete joinEngine;
  // ifstream infile;
  //
  // infile.open("r0", ios::binary | ios::in);
  // EXPECT_TRUE(!infile.failbit);
  // uint64_t numRec;
  // uint64_t numCol;
  //
  // //read number of records and number of columns in the relation
  // infile.read((char*)&numRec, sizeof(uint64_t));
  // infile.read((char*)&numCol, sizeof(uint64_t));
  // printf("There are %lu records and %lu columns in relation ", numRec, numCol);
  // //cout << relations[i].name << endl;
  // EXPECT_EQ (numRec, 1561);
  // EXPECT_EQ (numCol, 3);
  // //calculate size of column
  // size_t column_size = sizeof(uint64_t) * numCol;
  //
  // //dynamically allocate space to store column
  // uint64_t* column = (uint64_t*)malloc(column_size);
  // EXPECT_TRUE(column != NULL);
  // //move file pointer to the right place and read column_size bytes; then close the binary file
  // infile.seekg(column_size * 1, ios::cur);
  // infile.read((char*)column, column_size);
  // EXPECT_TRUE(column != NULL);
  // infile.close();
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
