#include <fstream>
#include <cinttypes>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Int_uint64_t.h"
#include "Index.h"

using namespace std;

class Relation{
public:
    string name;
    uint64_t column_index;
    uint64_t num_of_records;
    uint64_t num_of_columns;//is it needed?
    uint64_t* column;
    Int_uint64_t* new_column;

    int* hist_array;
    int* psum_array;
    Index* index_array;
};
