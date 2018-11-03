#include <cinttypes>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

class OutputNode {
public:
  uint64_t* data;
  OutputNode* next;
};

class OutputList {
public:
  OutputNode* headNode;
  OutputNode* curNode;
  uint64_t numBuckets;
  uint64_t totalBytes;

public:
  OutputList();
  ~OutputList();
  int InsertData(uint64_t,uint64_t);
  void printList();
};
