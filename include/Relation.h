#include <fstream>
#include <cinttypes>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "NewColumnEntry.h"
#include "Index.h"

using namespace std;

class Relation{
    string name;
    uint64_t column_index;

    uint64_t num_of_records;
    uint64_t num_of_columns;//is it needed?
    uint64_t* column;
    NewColumnEntry* new_column;

    int* hist_array;
    int* psum_array;
    Index* index_array;
public:
    //constructor
    Relation(string name, uint64_t column_index){
        //initialise ubject
        this->name = name;
        this->column_index = column_index;
        this->num_of_columns = this->num_of_records = 0;
        this->column = NULL;
        this->new_column = NULL;
        this->hist_array = NULL;
        this->psum_array = NULL;
        this->index_array = NULL;
    };
    ~Relation(){
        free(this->column);
        free(this->new_column);
        free(this->hist_array);
        free(this->psum_array);
    }

    //setters
    void set_num_of_records(uint64_t num_rec){ this->num_of_records = num_rec; };
    void set_num_of_columns(uint64_t num_col){ this->num_of_columns = num_col; };
    void set_column(uint64_t size){ this->column = (uint64_t*)malloc(size * sizeof(uint64_t)); };
    void set_new_column(uint64_t size){ this->new_column = (NewColumnEntry*)malloc(size * sizeof(NewColumnEntry)); };
    void set_hist_array(int size){ this->hist_array = (int*)malloc(size * sizeof(int)); };
    void set_psum_array(int size){ this->psum_array = (int*)malloc(size * sizeof(int)); };
    void set_index_array(int size){ this->index_array = (Index*)malloc(size * sizeof(Index)); };

    //getters
    string get_name(){ return this->name;};
    uint64_t get_column_index(){ return this->column_index; };
    uint64_t get_num_of_records(){ return this->num_of_records; };
    uint64_t get_num_of_columns(){ return this->num_of_columns; };
    uint64_t* get_column(){ return this->column; };
    NewColumnEntry* get_new_column(){ return this->new_column; };
    int* get_hist_array(){ return this->hist_array; };
    int* get_psum_array(){ return this->psum_array; }
    Index* get_index_array(){ return this->index_array; };
};
