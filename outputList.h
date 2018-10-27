#include <cinttypes>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

class OutputNode {
public:
  void* data;
  OutputNode* next;
};

class OutputList {
public:
  OutputNode* headNode;
  OutputNode* curNode;
  long int numBuckets;
  long int totalBytes;

public:
  OutputList();
  int InsertData(int*,int*);
};
