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
    Relation* relations[2]; //array of relations
    int h1_num_of_buckets;
    int h2_num_of_buckets;
    int h1_num_of_bits;
    int h2_num_of_bits;

    //hash function for segmentation
    inline int h1(uint64_t num){return (num & (h1_num_of_buckets - 1));};
    //hash function for indexing
    inline int h2(uint64_t num){return (num & ((h2_num_of_buckets - 1) << h2_num_of_bits) >> h2_num_of_bits);};

  public:
    JoinEngine(char const *argv[]);
    int load_info(); //loads relations to memory (heap)
    int segmentation();
    int indexing();
    int join();
};
