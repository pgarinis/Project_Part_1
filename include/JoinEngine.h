#include <fstream>
#include <cinttypes>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <math.h>

#include "Relation.h"
#include "NewColumnEntry.h"
#include "outputList.h"

#define FILENOTFOUND 1
#define UNEXPECTEDERROR 2
#define BADALLOC 3

using namespace std;

class JoinEngine{
    Relation* relations[2]; //array of relations
    int h1_num_of_buckets;
    int h2_num_of_buckets;
    int h1_num_of_bits;
    int h2_num_of_bits;
    OutputList *result;
    int err_code;

  public:
    JoinEngine(char const *argv[]);
    JoinEngine(char const *argv[],int numBuckets);
    ~JoinEngine();
    /*  getters - setters */
    Relation** get_relations(){
      return relations;
    }

    int get_h1_num_of_buckets(){
      return h1_num_of_buckets;
    }

    int get_h2_num_of_buckets(){
      return h2_num_of_buckets;
    }

    OutputList* get_result(){
      return result;
    }


    /* Error Handler */
    void error_handler(){
      //some errors may not terminate joinengine
      if(err_code == FILENOTFOUND){
        printf("ERROR: File not found\nEXITING\n");
        delete this;
        exit(FILENOTFOUND);
      }
      if(err_code == UNEXPECTEDERROR){
        printf("ERROR: Unexpected Error->Contact Admins\nEXITING\n");
        delete this;
        exit(UNEXPECTEDERROR);
      }
      if(err_code == BADALLOC){
        delete this;
        printf("ERROR: Bad Allocation->Not enough Memory?\nEXITING\n");
        exit(BADALLOC);
      }
    }

    void throw_err(int code){
      err_code = code;
      error_handler();
    }


    //hash function for segmentation
    inline int h1(uint64_t num){return (num & (h1_num_of_buckets - 1));};
    //inline int h1(uint64_t num){return num % 131;};
    //hash function for indexing
    //inline int h2(uint64_t num){return ((num & ((h2_num_of_buckets - 1) << h2_num_of_bits)) >> h2_num_of_bits);};
    inline int h2(uint64_t num){return num % 16699;};

    /*
    loads needed columns from each relation to memory dynamically(heap)
    */
    int load_relations();
        /*
        reads header info (number of columns and number of records) from opened binary file
        */
        int read_header_info(Relation* relation, ifstream& infile);
    /*
    rearranges every column's records in order to create a new column where records
    are placed in different order (depending on their hash values from h1 hash function)
    */
    int segmentation();
        /*
        creates and computes hist array for the relation given as input
        */
        int create_and_compute_hist_array(Relation* relation);
        /*
        creates and computes psum array for the relation given as input
        */
        int create_and_compute_psum_array(Relation* relation);
        /*
        creates and computes new column of the relation(R'), where rows are sorted by their h1 hash value
        */
        int create_and_compute_new_column(Relation* relation);
    /*
    creates an index(hash table) on one of the new columns created from segmentation
    */
    int indexing();
        /*
        creates and initialises chain and bucket array from the bucket's index
        */
        int create_and_init_chain_and_bucket_array(Index& index, int hist_array_value);
    /*
    joins two columns(relations) and return index to the list that holds the results:
    return value = ---->[1MB mem block]---->[1MB mem block]--->(...)--->NULL
    [1MB mem block] format : [uint64_t|uint64_t|uint64_t|uint64_t...] without '|'
    each pair is a result from the join query
    */
    int join();
};
