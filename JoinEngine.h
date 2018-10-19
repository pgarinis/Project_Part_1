#include <fstream>
#include <cinttypes>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <math.h>

#include "Relation.h"
#include "Int_uint64_t.h"

using namespace std;

class JoinEngine{
    //private variables
    Relation relations[2];
    int h1_num_of_buckets;
    int h2_num_of_buckets;

    //private functions
    //hash function for segmentation
    inline int h1(uint64_t num){return num & (h1_num_of_buckets - 1);};
    //hash function for indexing
    inline int h2(uint64_t num){return (num & ((h2_num_of_buckets - 1) << (int)log2(h2_num_of_buckets))) >> (int)log2(h2_num_of_buckets);};
    int main_engine();
    int load_info(); //loads relations to memory (heap)
    int segmentation();
    int indexing(Relation& relation);
    int join(Relation r0, Relation r1);
  public:
    JoinEngine(char const *argv[]);
};
