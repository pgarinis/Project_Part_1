#include <iostream>
#include "../include/JoinEngine.h"
#include <sys/time.h>

void exit_wmsg(){
    printf("Error Occured - Exiting\n");
    exit(1);
}

using namespace std;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL))
        return 0;
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main(int argc, char const *argv[]){
    //printf("Format to run the program : ./a.out rx f1 ry f2\n");
    double t0 = get_wall_time();
    JoinEngine *joinEngine = new JoinEngine(argv);
    if(joinEngine->load_relations())
      joinEngine->error_handler();

    if(joinEngine->segmentation())
      joinEngine->error_handler();

    if(joinEngine->indexing())
      joinEngine->error_handler();

    if(joinEngine->join())
      joinEngine->error_handler();

    //joinEngine->get_result()->printList();
    int totalrec = joinEngine->get_result()->totalBytes / (2*sizeof(uint64_t));
    cout << "-=================-\n" <<"Total Records: " << totalrec << endl;
    double t1 = get_wall_time();
    cout << "Took: " << t1-t0 << " sec" << endl;
    delete(joinEngine);
    return 0;
}
