#include <stdlib.h>
#include "function.h"


int main(int argc, char **argv){
    vector<map<int, vector<Entry> > > matrices;
    vector<vector<int> > sequences;
    matrices = reader(argv[1], sequences);

    set_args(matrices, sequences);
    set_threshold(atoi(argv[2]));
    PrintMatrix(matrices, sequences);

    run();
    //test();
    return 0;
}
