#include "../include/outputList.h"
#include <cstdlib>

OutputList::OutputList(){
    this->numBuckets = 0;
    this->headNode = NULL;
    this->curNode = NULL;
    this->totalBytes = 0;
}

OutputList::~OutputList(){
  OutputNode *currentNode = this->headNode;
  while(currentNode != NULL){
    OutputNode *temp = currentNode;
    currentNode = currentNode->next;
    free(temp);
  }
}

void OutputList::printList(){
  OutputNode *currentNode = this->headNode;
  while(currentNode != NULL){
    for(int i = 0; i < (1024*1024)/sizeof(uint64_t) && i < this->totalBytes/sizeof(uint64_t); i+=2){
      printf("rowId: %lu == rowId: %lu\n",this->curNode->data[i],this->curNode->data[i+1]);
      //int c = getchar();
    }
    currentNode = currentNode->next;
  }
}

int OutputList::InsertData(uint64_t row1, uint64_t row2){
  int sizeToInsert = sizeof(row1) + sizeof(row2);
  //if no data has been inserted
  if(this->headNode == NULL){
    OutputNode* outNode = new OutputNode();
    outNode->data = (uint64_t*)malloc(1024*1024);
    outNode->next = NULL;
    outNode->data[0] = row1;
    outNode->data[1] = row2;

    this->headNode = outNode;
    this->curNode = outNode;
    this->numBuckets++;
    this->totalBytes += sizeToInsert;
    return 0;
  }
  int node_index = (this->totalBytes + sizeToInsert) / (1024*1024);
  int node_offset = (this->totalBytes ) % (1024*1024);

  if(numBuckets != node_index +1){
    //make a new node
    OutputNode* outNode = new OutputNode();
    outNode->data = (uint64_t*)malloc(1024*1024);
    outNode->next = NULL;
    this->curNode->next = outNode;
    this->curNode = outNode;
    this->numBuckets++;

    outNode->data[0] = row1;
    outNode->data[1] = row2;
    this->totalBytes += sizeToInsert;
    return 0;
  }
  else{
    //insert data to node with node_index
    this->curNode->data[node_offset/sizeof(uint64_t)] = row1;
    this->curNode->data[(node_offset+ sizeof(row1))/sizeof(uint64_t)] = row2;
    this->totalBytes += sizeToInsert;
    return 0;
  }



}
