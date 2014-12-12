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

// Parameter set
void set_args(vector<map<int, vector<Entry> > > mx, vector<vector<int> > seq);
void set_threshold(int th);

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

int match(vector<int> items, map<int, vector<Entry> > matrix, int prev_matched);

// Given the pattern, find the matched position of each transation matrix(The first position is the pivot)
vector<vector<int> > find_matched_indexes(vector<vector<int> > pattern);
vector<int> get_keys(map<int, vector<vector<UT_E> > > dict);

void width_pruning(map<int, vector<vector<UT_E> > > &ilist, map<int, vector<vector<UT_E> > > &slist);
bool depth_pruning(int last_item, vector<vector<UT_E> > list_of_utilities);

int find_row_index(int last_item, vector<int> sequence);
vector<vector<UT_E> > find_s_candidates(int item, vector<vector<UT_E> > list_of_utilities);
void candidate_generate(int last_item, vector<vector<UT_E> > list_of_utilities, map<int, vector<vector<UT_E> > > &ilist, map<int, vector<vector<UT_E> > > &slist);

void USpan(vector<vector<int> > pattern, vector<vector<UT_E> > list_of_utilities);
