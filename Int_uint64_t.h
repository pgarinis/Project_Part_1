#pragma once
#include <fstream>
#include <cinttypes>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

class Int_uint64_t{
public:
    int index;
    uint64_t value;
    Int_uint64_t(int index, uint64_t value){
        this->index = index;
        this->value = value;
    }
};
