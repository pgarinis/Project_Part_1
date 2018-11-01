#include <iostream>
#include "../include/JoinEngine.h"

using namespace std;

int main(int argc, char const *argv[]){
    //printf("Format to run the program : ./a.out rx f1 ry f2\n");
    JoinEngine *joinEngine = new JoinEngine(argv);
    joinEngine->load_relations();
    joinEngine->segmentation();
    joinEngine->indexing();
    joinEngine->join();
    delete(joinEngine);
    return 0;
}
