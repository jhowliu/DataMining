#include <stdio.h>
#include <map>

#include "function.h"

int threshold;
// List of mapper matrix
vector<map<int, vector<Entry> > > matrices;

int main(){
    return 0;
}


void width_pruning(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist){
    //for ()
}

bool depth_pruning(vector<vector<int> > pattern, vector<int> utility){
    int sigma = 0;
    int a = pattern.size()-1;
    int b = pattern[a].size()-1;
    int lastPattern = pattern[a][b];

    // Self utility
    for (int i = 0; i < utility.size(); i++) sigma += utility[i];
    // Remain utility
    for (int i = 0; i < matrices.size(); i++){
        if (matrices[i].find(lastPattern) != -1){
            // Find pivot j
            sigma += matrices[i][lastPattern][j].remain;
        }
    }

    return sigma >= threshold
}

void candidate_generate(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist);

void USpan(vector<vector<int> > pattern, vector<int> utility){
    // If not pass the depth pruning then return(arrive the leaf node)
    if (depth_pruning(pattern, utility) != true) return;

    vector<int> ilist, slist;
    candidate_generate(pattern, utility, ilist, slist);
    width_pruning(pattern, utility, ilist, slist);
    
    // Other work(Jhow)
}
