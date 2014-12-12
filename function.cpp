#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>

int threshold;
// List of mapper matrix
vector<map<int, vector<Entry> > > matrices;

// Total utility of each
vector<int> total_utilities;

// Sorted sequence
vector<vector<int> > sequences;

void set_args(vector<map<int, vector<Entry> > > mx, vector<vector<int> > seq){
    matrices = mx;
    sequences = seq;
}

void test(){
    PrintMatrix(matrices, sequences);
}

vector<map<int, vector<Entry> > > reader(char *fileName, vector<vector<int> > &sequences) {
    vector<map<int, vector<Entry> > > matrix;
    char buf[SIZE];
    char *token;
    fstream file;

    file.open(fileName, ios::in);

    if (!file) {
        cout << "Can't open file!" << endl;
        exit(1);
    }

    vector<vector<string> > tmpVector;
    
    // Split sequences by ';'
    while (file.getline(buf, sizeof(buf))) {
        token = strtok(buf, ";");
        vector<string> s;
        while (token != NULL) {
            string tmp = string(token);
            s.push_back(tmp);
            token = strtok(NULL, ";");
        }
        buf[0] = 0;
        tmpVector.push_back(s);
    }
    
    // Parse each transaction
    int idx = 0;
    for (vector<vector<string> >::iterator v_iter = tmpVector.begin(); v_iter != tmpVector.end(); v_iter++, idx++) 
    {
        vector<string>::iterator iter = v_iter->begin();
        char tmp[100];
        // Split header
        strcpy(tmp, iter->c_str());
        int col = atoi(strtok(tmp, ",")); 
        int row = atoi(strtok(NULL, ","));
        tmp[0] = 0;
        iter++;

        
        // Split pattern
        strcpy(tmp, iter->c_str());
        vector<int> p;
        char *token;
        token = strtok(tmp, ",");

        while (token!= NULL) {
            p.push_back(token[0] - '0');
            token = strtok(NULL, ",");
        }

        iter++;

        // Maintain pattern list
        sequences.push_back(p);

        // Split itemset
        map<int, vector<Entry> > emap;
        for (int i = 0; i != row; i++) 
        {
            vector<Entry> entry;
            for (int j = 0; j != col; j++) 
            {
                Entry e;
                strcpy(tmp, (iter + i + j*row)->c_str());
                e.self = atoi(strtok(tmp, ","));
                e.remain = atoi(strtok(NULL,","));
                entry.push_back(e);
            }
            emap[p[i]] = entry;
        }
        matrix.push_back(emap);
    }


    return matrix;
}

void PrintMatrix(vector<map<int, vector<Entry> > > matrix, vector<vector<int> > sequences) {
    for (int i = 0; i != matrix.size(); i++) 
    {
        for (int j = 0; j != sequences[i].size(); j++) 
        {
            cout << (char)('0' + sequences[i][j]) << " ";
            for (vector<Entry>::iterator iter = matrix[i][sequences[i][j]].begin(); iter != matrix[i][sequences[i][j]].end(); iter++) 
                cout << "(" << iter->self << "," << iter->remain << ") ";
            cout << endl;
        }
        cout << endl;
    }
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

int match(vector<int> items, map<int, vector<Entry> > matrix, int prev_matched){
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

vector<vector<int> > find_matched_indexes(vector<vector<int> > pattern){
    int j;
    vector<vector<int> > items_positions;
    // Find each matrix matched positions
    for (int i = 0; i < matrices.size(); i++) { // Iterate matrix
        // Index of matched itemset
        int matched_itemset = -1;
        vector<int> temp;
        // Match step by step(itemset)
        for (j = 0; j < pattern.size(); j++) {
            matched_itemset = match(pattern[j], matrices[i], matched_itemset);
            if (matched_itemset == -1) break;
        }
        
        // Pivot exist, find the remain matched item's position
        while (matched_itemset != -1) {
            temp.push_back(matched_itemset);
            matched_itemset = match(pattern[j-1], matrices[i], matched_itemset);
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

bool depth_pruning(int last_item, vector<vector<UT_E> > list_of_utilities){
    int sigma = 0;
    for (int i = 0; i < matrices.size(); i++){
        vector<UT_E> utilities = list_of_utilities[i];

        // Exist at least one end item
        if (utilities.size() != 0){
            int col = utilities[0].index;
            // Rest utility + Current pattern utility(pivot)
            sigma = matrices[i][last_item][col].remain + utilities[0].utility;
        }
    }

    return sigma >= threshold;
}

// Find the item index in the given sequence, if not find return -1
int find_row_index(int last_item, vector<int> sequence){
    int item_row = -1
    for (int i = 0; i < sequence.size(); i++){
        if (sequence[i] == last_item){
            item_row = j;
            break;
        }
    }
    
    return item_row;
}

void candidate_generate(int last_item, vector<vector<UT_E> > list_of_utilities, map<int, vector<vector<UT_E> > > &ilist, map<int, vector<vector<UT_E> > > &slist){
    set<int> iset, sset;
    for (int i = 0; i < matrices.size(); i++){
        vector<UT_E> utilities = list_of_utilities[i];

        // Exist at least one end item
        if (utilities.size() != 0){
            // Find I-Concat item
            int item_row = find_row_index(last_item, sequences[i]);
            for (int j = item_row+1; j < sequences[i].size(); j++) iset.insert(sequences[i][j]);

            // Find S-Concat item
            if (utilities[0].index < matrices[i].size()-1){
                for (int j = item_row+1; j < sequences[i].size(); j++) sset.insert(sequences[i][j]);
            }
        }
    }

    // Create ilist
    for (set<int>::iterator it = iset.begin(); it != iset.end(); ++it){
        vector<vector<UT_E> > temp;
        for (int i = 0; i < matrices.size(); i++){
            vector<UT_E> end_items;
            vector<UT_E> utilities = list_of_utilities[i];
            for (int j = 0; j < utilities.size(); j++){
                int col = utilities[j].index;
                int cum_utility = utilities[j].utility;
                if (matrices[i][last_item][col].self > 0){
                    vector<UT_E> e;
                    e.index = col; e.utility = cum_utility + matrices[i][last_item][col].self;
                    end_items.push_back(e);
                }
            }
            temp.push_back(end_items);
        }
        ilist[*it] = temp;
    }
   
    // Create slist 
    for (set<int>::iterator it = sset.begin(); it != sset.end(); ++it) ilist.push_back(*it){
        vector<vector<UT_E> > temp;
        for (int i = 0; i < matrices.size(); i++){
        }
    }
}

void USpan(vector<vector<int> > pattern, vector<int> utility){
    vector<vector<int> > items_positions = find_matched_indexes(pattern);
    
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
        vector<vector<int> > matched_index = find_matched_indexes(new_pattern);

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
