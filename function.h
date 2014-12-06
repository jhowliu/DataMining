#include <vector>

// Matrix Entry tuple(self, remaining utility)
typedef struct{
    int self;
    int remain;
} Entry;

using namespace std;

bool width_pruning(vector<vector<int> > pattern, vector<int> utility);
bool depth_pruning(vector<vector<int> > pattern, vector<int> utility);
void candidate_generate(vector<vector<int> > pattern, vector<int> utility);
void USpan(vector<vector<int> > pattern, vector<int> utility);
