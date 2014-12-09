#include <stdio.h>
#include <map>

#include "function.h"

int threshold;
// List of mapper matrix
vector<map<int, vector<Entry> > > matrices;

int main(){
    return 0;
}

int matched(vector<int> items, map<int, vector<Entry> > matrix, int prev_matched){
    int matched_index = -1;
    bool exist = true;
    vector<Entry> entries;

    // Check if all the item is exist in the map
    for (int i = 0; i < items.size(); i++){
        if (matrix.count(items[i]) == 0){
            exist = false;
            break;
        }
    }

    if (exist == true){
        // The length of all the entries in the matrix is equal
        int len = matrix[items[0]].size();
        // Find the index which all items's self utility > 0
        for (int i = prev_matched+1; i < len; i++){
            for (int j = 0; j < items.size(); j++){
                entries = matrix[items[j]];
                if (entries[i].self == 0) break;
            }
            
            // All the items's self utility > 0
            if (j == items.size()){
                matched_index = i;
                break;
            }
        }
    }

    return matched_index;
}

vector<vector<int> > find_matched_items(vector<vector<int> > pattern){
    vector<vector<int> > items_positions;
    // Find each matrix matched positions
    for (int i = 0; i < matrices.size(); i++){
        // Index of matched itemset
        int matched_itemset = -1;
        vector<int> temp;
        // Match step by step(itemset)
        for (int j = 0; j < pattern.size(); j++){
            matched_itemset = matched(pattern[j], matrices[i], matched_itemset);
            if (matched_itemset == -1) break;
        }
        
        // Pivot exist, find the remain matched item's position
        while (matched_itemset != -1){
            temp.push(matched_itemset);
            matched_itemset = matched(pattern[j-1], matrices[i], matched_itemset);
        }

        items_positions.psuh(temp);
    }
    
    return items_positions;
}

void width_pruning(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist){
    //for ()
}

bool depth_pruning(vector<vector<int> > pattern, vector<int> utility,  vector<vector<int> > items_positions){
    int sigma = 0;
    int a = pattern.size()-1;
    int b = pattern[a].size()-1;
    int lastPattern = pattern[a][b];

    // Self utility
    for (int i = 0; i < utility.size(); i++) sigma += utility[i];
    // Remain utility
    for (int i = 0; i < matrices.size(); i++){
        // Check the pattern matched at least once in the transaction matrix
        if (items_positions[i].size() > 0){
            int pivot_i = items_positions[i][0];
            sigma += matrices[i][lastPattern][pivot_i].remain;
        }
    }

    return sigma >= threshold
}

void candidate_generate(vector<vector<int> > pattern, vector<int> utility, vector<int> &ilist, vector<int> &slist);

void USpan(vector<vector<int> > pattern, vector<int> utility){
    vector<vector<int> > items_positions = find_matched_items(pattern);
    
    // If not pass the depth pruning then return(arrive the leaf node)
    if (depth_pruning(pattern, utility, items_positions) != true) return;

    vector<int> ilist, slist;
    candidate_generate(pattern, utility, ilist, slist);
    width_pruning(pattern, utility, ilist, slist);
    
    // Other work(Jhow)
}
