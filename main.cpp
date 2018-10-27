#include <iostream>
#include "JoinEngine.h"

using namespace std;

int main(int argc, char const *argv[]){
    //printf("Format to run the program : ./a.out rx f1 ry f2\n");
    JoinEngine *joinEngine = new JoinEngine(argv);
    joinEngine->load_info();
    joinEngine->segmentation();
    joinEngine->indexing();
    joinEngine->join();
    return 0;
}
