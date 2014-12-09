#include <vector>

// Matrix Entry tuple(self, remaining utility)
typedef struct{
    int self;
    int remain;
} Entry;

using namespace std;

int matched(vector<int> items, map<int, vector<Entry> > matrix, int prev_matched);

// Given the pattern, find the matched position of each transation matrix(The first position is the pivot)
vector<vector<int> > find_matched_items(vector<vector<int> > pattern);
void width_pruning(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions);
bool depth_pruning(vector<vector<int> > pattern, vector<int> utility, vector<vector<int> > items_positions);
void candidate_generate(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist);
void USpan(vector<vector<int> > pattern, vector<int> utility);
