#include <vector>

// Matrix Entry tuple(self, remaining utility)
typedef struct{
    int self;
    int remain;
} Entry;

using namespace std;

void width_pruning(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist);
bool depth_pruning(vector<vector<int> > pattern, vector<int> utility);
void candidate_generate(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist);
void USpan(vector<vector<int> > pattern, vector<int> utility);
