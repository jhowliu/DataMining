#include <vector>
#include <set>
#include <map>

#define SIZE 2048

// Utility entry
typedef struct{
    int index;
    int utility;
}UT_E;

// Matrix Entry tuple(self, remaining utility)
typedef struct {
    int self;
    int remain;
} Entry;

using namespace std;

void set_args(vector<map<int, vector<Entry> > > mx, vector<vector<int> > seq);
void test();

// Loader
void PrintMatrix(vector<map<int, vector<Entry> > > matrix, vector<vector<int> > sequences);
vector<map<int, vector<Entry> > > reader(char *fileName, vector<vector<int> > &sequences);

// Tool 
void ConcatenationFunc(vector<vector<int> > pattern, vector<int> utility, vector<int> list);
void PrintPattern(vector<vector<int> > p);

// Concatenation
vector<vector<int> > IConcat(vector<vector<int> > *p, int candidate);
vector<vector<int> > SConcat(vector<vector<int> > *p, int candidate);

// Utility calculation with candidates.
vector<int> cal_utility(map<int, vector<Entry> > matrix, int pattern, vector<int> pattern_index, vector<int> utility, int *max_utility);

int matched(vector<int> items, map<int, vector<Entry> > matrix, int prev_matched);

// Given the pattern, find the matched position of each transation matrix(The first position is the pivot)
vector<vector<int> > find_matched_items(vector<vector<int> > pattern);
void width_pruning(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions);
bool depth_pruning(vector<vector<int> > pattern, vector<int> utility, vector<vector<int> > items_positions);
void candidate_generate(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions);
void USpan(vector<vector<int> > pattern, vector<int> utility);
