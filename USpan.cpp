#include <stdlib.h>
#include <stdio.h>
#include "function.h"


int main(int args, char **argv){
    if (args < 3) {
        printf("USAGE: ./a.out <fileName> <threshold>\n");
        exit(1);
    }
    vector<map<int, vector<Entry> > > matrices;
    vector<vector<int> > sequences;
    matrices = reader(argv[1], sequences);

    set_args(matrices, sequences);
    set_threshold(atoi(argv[2]));
    //PrintMatrix(matrices, sequences);
    
    printf("RESULT\n");
    run();
    //test();
    return 0;
}
