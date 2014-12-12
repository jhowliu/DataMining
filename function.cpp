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

vector<int> get_keys(map<int, vector<vector<UT_E> > > dict){
    vector<int> keys;
    for (map<int, vector<vector<UT_E> > >::iterator it = ilist.begin(); it != ilist.end(); ++it)
        keys.push_back(it->first);

    return keys;
}

void width_pruning(map<int, vector<vector<UT_E> > > &ilist, map<int, vector<vector<UT_E> > > &slist){
    // Pruning ilist
    vector<int> keys;
    keys = get_keys(ilist);
    for (vector<int>::iterator it = keys.begin(); it != keys.end(); ++it){
        int swu = 0;
        for (int i = 0; i < matrices.size(); i++){
            // First item in the matrix
            int item = sequences[i][0];
            vector<UT_E> candidates = ilist[*it][i];
            // Has candidate mean the pattern is matched in the matrix
            if (candidates.size() > 0){
                swu += matrices[i][item][0].self + matrices[i][item][0].remain;
            }
        }

        // Pruning the unquilified item
        if (swu < threshold) ilist.erase(*it);
    }

    // Pruning slist
    keys = get_keys(slist);
    for (vector<int>::iterator it = keys.begin(); it != keys.end(); ++it){
        int swu = 0;
        for (int i = 0; i < matrices.size(); i++){
            // First item in the matrix
            int item = sequences[i][0];
            vector<UT_E> candidates = slist[*it][i];
            // Has candidate mean the pattern is matched in the matrix
            if (candidates.size() > 0){
                swu += matrices[i][item][0].self + matrices[i][item][0].remain;
            }
        }

        // Pruning the unquilified item
        if (swu < threshold) slist.erase(*it);
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
    int item_row = -1;
    for (int i = 0; i < sequence.size(); i++){
        if (sequence[i] == last_item){
            item_row = i;
            break;
        }
    }
    
    return item_row;
}

// Find all the S-concat item after the pivot itemset
// Besides, the utility of the candidates are initiated to -1
vector<vector<UT_E> > find_s_candidates(int item, vector<vector<UT_E> > list_of_utilities){
    vector<vector<UT_E> > list_of_candidates;
    for (int i = 0; i < matrices.size(); i++){
        vector<UT_E> candidates;
        if (list_of_utilities[i].size() > 0){
            int mathed_index = list_of_utilities[i][0].index;
            while (mathed_index != -1){
                mathed_index = match(item, matrices[i], mathed_index);
                if (mathed_index != -1) {
                    UT_E e;
                    e.index = mathed_index; e.utility = -1;
                    candidates.push_back(e);
                }
            }
        }
        list_of_candidates.push_back(candidates);
    }

    return list_of_candidates;
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
        vector<vector<UT_E> > list_of_candidates;
        for (int i = 0; i < matrices.size(); i++){
            vector<UT_E> candidates;
            vector<UT_E> utilities = list_of_utilities[i];
            for (int j = 0; j < utilities.size(); j++){
                int col = utilities[j].index;
                if (matrices[i][*it][col].self > 0){
                    UT_E e;
                    e.index = col; e.utility = utilities[j].utility + matrices[i][*it][col].self;
                    candidates.push_back(e);
                }
            }
            list_of_candidates.push_back(candidates);
        }
        ilist[*it] = list_of_candidates;
    }

    // Create slist 
    for (set<int>::iterator it = sset.begin(); it != sset.end(); ++it) {
        
        vector<vector<UT_E> > list_of_candidates = find_s_candidates(*it, list_of_utilities);
        for (int i = 0; i < matrices.size(); i++){
            vector<UT_E> utilities = list_of_utilities[i];
            vector<UT_E> candidates = list_of_candidates[i];

            // Fill the max ancestor utility
            for (int j = 0; j < candidates.size(); j++){
                for (int k = 0; k < utilities.size(); k++){
                    // S-Conat item must locate after pattern
                    if (!(utilities[k].index < candidates[j].index)) break;
                    if (utilities[k].utility > candidates[j].utility) candidates[j].utility = utilities[k].utility;
                }
            }

            // Add the self utility
            for (int j = 0; j < candidates.size(); j++){
                int col = utilities[j].index;
                candidates[j].utility += matrices[i][*it][col].self;
            }
        }
        slist[*it] = list_of_candidates;
    }
}

void USpan(vector<vector<int> > pattern, vector<vector<UT_E> > utilities) {
    vector<vector<int> > items_positions = find_matched_indexes(pattern);
    
    // If not pass the depth pruning then return(arrive the leaf node)
    if (depth_pruning(pattern, utilities, items_positions) != true) return;

    map<int, vector<vector<UT_E> > > ilist, slist;

    candidate_generate(pattern, ilist, slist, items_positions);
    width_pruning(pattern, ilist, slist, items_positions);
    
    // I-Concatenation
    ConcatenationFunc(pattern, ilist, ICONCAT);
    // S-Concatenation
    ConcatenationFunc(pattern, slist, SCONCAT);
}

void ConcatenationFunc(vector<vector<int> > pattern, map<int, vector<vector<UT_E> > > list, CONCATENATION method) {
    vector<vector<int> > newPattern;

    for (map<vector<vector<UT_E> > >::iterator it = list.begin(); it != list.end(); it++) 
    {
        int maxUtilities = 0;
        // Create a new pattern
        switch (method) 
        {
            case ICONCAT:
                newPattern = IConcat(pattern, it->first); break;
            case SCONCAT:
                newPattern = SConcat(pattern, it->first); break;
        }
        // Iterate matrix
        for (vector<vector<UT_E> >::iterator v_it = (it->second).begin(); v_it != (it->second).end(); v_it++) 
        {
            int max = -1;
            // Iterate all utility of the new pattern
            for (vector<UT_E>::iterator iter; iter != v_it->begin(); iter++) 
            {
                // Get maximum utility in each matrix for new pattern
                if (iter->utility > max) max = iter->utility;
            }

            if (max != -1) maxUtilities += max;
        }
        
        if (maxUtilities > threshold) USpan(newPattern, list[it->first]);
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
