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
    /*
    loads needed columns from each relation to memory dynamically(heap)
    */
    int load_relations();
    /*
    rearranges every column's records in order to create a new column where records
    are placed in different order (depending on their hash values from h1 hash function)
    */
    int segmentation();
    /*
    creates an index(hash table) on one of the new columns created from segmentation
    */
    int indexing();
    /*
    joins two columns(relations) and return index to the list that holds the results:
    return value = ---->[1MB mem block]---->[1MB mem block]--->(...)--->NULL
    [1MB mem block] format : [uint64_t|uint64_t|uint64_t|uint64_t...] without '|'
    each pair is a result from the join query
    */
    int join();
};
