#include "function.h"

int main(int argc, char **argv){
    vector<map<int, vector<Entry> > > matrices;
    vector<vector<int> > sequences;
    matrices = reader(argv[1], sequences);

    set_args(matrices, sequences); 
    test();
    //reader(argv[1], sequences);
    return 0;
}
