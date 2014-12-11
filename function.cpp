#include <iostream> 
#include <stdio.h>
#include <map>

#include "function.h"

int threshold;
// List of mapper matrix
vector<map<int, vector<Entry> > > matrices;

// Total utility of each
vector<int> total_utilities;

// Sorted sequence
vector<vector<int> > sequences;

int main(){
    return 0;
}

// Calculate the utility of current pattern after concating the new pattern.
vector<int> cal_utility(map<int, vector<Entry> > matrix, int pattern, vector<int> pattern_index, vector<int> utility, int *max_utility) {
    vector<int> utilities;
    int max = 0;
    int index = 0;
    
    for (vector<int>::iterator iter = pattern_index.begin(); iter != pattern_index.end(); iter++) 
    {
        int tmp = utility[index++] + matrix[pattern][*iter].self;
        utilities.push_back(tmp);
        if (tmp > max) 
            max = tmp;
    }

    *max_utility = max;

    return utilities;
}

// S-Concatenation in the different itemsets.
vector<vector<int> > SConcat(vector<vector<int> > p, int candidate) {
    vector<int> tmp;

    tmp.push_back(candidate);
    p.push_back(tmp);

    return p;
}

// I-Concatenation in the same itemset.
vector<vector<int> > IConcat(vector<vector<int> > p, int candidate) {
    vector<int> *tmp = &(p.back());
    
    tmp->push_back(candidate);

    return p;
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
        int len = matrix[items[0]].size(), j;
        // Find the index which all items's self utility > 0
        for (int i = prev_matched+1; i < len; i++){
            for (j = 0; j < items.size(); j++){
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
    int j;
    vector<vector<int> > items_positions;
    // Find each matrix matched positions
    for (int i = 0; i < matrices.size(); i++) { // Iterate matrix
        // Index of matched itemset
        int matched_itemset = -1;
        vector<int> temp;
        // Match step by step(itemset)
        for (j = 0; j < pattern.size(); j++) {
            matched_itemset = matched(pattern[j], matrices[i], matched_itemset);
            if (matched_itemset == -1) break;
        }
        
        // Pivot exist, find the remain matched item's position
        while (matched_itemset != -1) {
            temp.push_back(matched_itemset);
            matched_itemset = matched(pattern[j-1], matrices[i], matched_itemset);
        }

        items_positions.push_back(temp);
    }
    
    return items_positions;
}

void width_pruning(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions){
    // Pruning ilist 
    for (int i = ilist.size()-1; i >= 0; i--) {
        int swu = 0;
        int item = ilist[i];
        for (int j = 0; j < matrices.size(); j++){
            // Check the concat item if valid in the matrix
            if (matrices[j].count(item) == 0) continue;
            for (int k = 0; k < items_positions[j].size(); k++){
                int col = items_positions[j][k];
                if (matrices[j][item][col].self > 0){
                    swu += total_utilities[j];
                    break;
                }
            }
        }

        // Pruning the unquilified item
        if (swu < threshold) ilist.erase(ilist.begin()+i);
    }

    // Pruning slist 
    for (int i = slist.size()-1; i >= 0; i--){
        int swu = 0;
        vector<int> items;
        items.push_back(slist[i]);
        for (int j = 0; j < matrices.size(); j++){
            if (items_positions[j].size() > 0){
                int pivot_i = items_positions[j][0];
                
                // Check the concat item if valid
                if (matched(items, matrices[j], pivot_i) == -1) swu += total_utilities[j];
            }
        }

        // Pruning the unquilified item
        if (swu < threshold) slist.erase(slist.begin()+i);
    }
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

    return sigma >= threshold;
}

void candidate_generate(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions){
    int a = pattern.size()-1;
    int b = pattern[a].size()-1;
    int last_item = pattern[a][b];
    
    set<int> iset, sset;
    for (int i = 0; i < matrices.size(); i++){
        // Check pivot exist
        if (items_positions[i].size() > 0){
            // Item index in the sequence
            int last_item_index; 
            for (int j = 0; j < sequences[i].size(); j++){
                if (sequences[i][j] == last_item){
                    last_item_index = j;
                    break;
                }
            }

            // Add the remain item to iset
            for (int j = last_item_index+1; j < sequences[i].size(); j++) iset.insert(sequences[i][j]);
            
            // If the pivot index not the last itemset, then s concat all the item in the sequence
            if (items_positions[i][0] < matrices[i].size()-1){
                for (int j = 0; j < sequences[i].size(); j++) sset.insert(sequences[i][j]);
            }
        }
    }

    // Convert set to list
    for (set<int>::iterator it = iset.begin(); it != iset.end(); ++it) ilist.push_back(*it);
    for (set<int>::iterator it = sset.begin(); it != sset.end(); ++it) ilist.push_back(*it);
}

void USpan(vector<vector<int> > pattern, vector<int> utility){
    vector<vector<int> > items_positions = find_matched_items(pattern);
    
    // If not pass the depth pruning then return(arrive the leaf node)
    if (depth_pruning(pattern, utility, items_positions) != true) return;

    vector<int> ilist, slist;
    candidate_generate(pattern, ilist, slist, items_positions);
    width_pruning(pattern, ilist, slist, items_positions);
    
    // I-Concatenation
    ConcatenationFunc(pattern, utility, ilist);
    // S-Concatenation
    ConcatenationFunc(pattern, utility, slist);
}

void ConcatenationFunc(vector<vector<int> > pattern, vector<int> utility, vector<int> list) {
    for (int i = 0; i != list.size(); i++) {
        vector<vector<int> > new_pattern = IConcat(pattern, list[i]);

        // Find matched position of the new pattern.
        vector<vector<int> > matched_index = find_matched_items(new_pattern);

        // Calculate utilities of the new pattern.
        for (int idx = 0; idx != matrices.size(); idx++) {
            int max_utility = 0;
            vector<int> tmp_utility = cal_utility(matrices[idx], list[i], matched_index[idx], utility, &max_utility);

            // Output the new pattern if its maximum utility is larger than threshold.
            if (max_utility > threshold) PrintPattern(new_pattern);

            USpan(new_pattern, tmp_utility);
        }
    }
}

void PrintPattern(vector<vector<int> > p) {
    char buf[20];
    for (int i = 0; i != p.size(); i++) 
    {
        cout << "(";
        for (int j = 0; j != p[i].size(); j++) 
        {
            sprintf(buf, "%d", p[i][j]); 
            cout << buf;
            if (j != p[i].size()-1) cout << ",";
            buf[0] = 0;
        }
        cout << ")";
    }
    cout << endl;
}
