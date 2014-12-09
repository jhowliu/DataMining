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
            temp.push_back(matched_itemset);
            matched_itemset = matched(pattern[j-1], matrices[i], matched_itemset);
        }

        items_positions.psuh(temp);
    }
    
    return items_positions;
}

void width_pruning(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions){
    // Pruning ilist 
    for (int i = ilist.size()-1; i >= 0; i--){
        int swu = 0;
        int item = ilist[i];
        for (int j = 0; j < matrice.size(); j++){
            if (items_positions[j].size() > 0){
                int pivot_i = items_positions[j][0];
                // Check the concat item if valid
                if (matrice[j].count(item) > 0 && matrice[j][item][pivot_i].self > 0) swu += total_utilities[j];
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
        for (int j = 0; j < matrice.size(); j++){
            if (items_positions[j].size() > 0){
                int pivot_i = items_positions[j][0];
                
                // Check the concat item if valid
                if (matched(items, matrice[j], pivot_i) == -1) swu += total_utilities[j];
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

    return sigma >= threshold
}

void candidate_generate(vector<vector<int> > pattern, vector<int> &ilist, vector<int> &slist, vector<vector<int> > items_positions){
    int last_item = pattern[pattern.size()-1];
    
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
            if (items_positions[i][0] < matrice[i].size()-1){
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
    candidate_generate(pattern, utility, ilist, slist);
    width_pruning(pattern, items_positions, ilist, slist);
    
    // Other work(Jhow)
}
