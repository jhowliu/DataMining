#include "concate.h"
#include "function.h"
#include <iostream>
#include <stdio.h>

// S-Concatenation in the different itemsets.
vector<vector<int> > *SConcat(vector<vector<int> > *p, int candidate) {
    vector<int> tmp;

    tmp.push_back(candidate);
    p->push_back(tmp);

    return p;
}

// I-Concatenation in the same itemset.
vector<vector<int> > *IConcat(vector<vector<int> > *p, int candidate) {
    vector<int> *tmp = &(p->back());

    tmp->push_back(candidate);

    return p;
}

// Testing code
int main(int argc, char** argv) {
    vector<vector<int> > b;
    vector<int> a;

    a.push_back(123);
    b.push_back(a);
    IConcat(&b, 789);
    SConcat(&b, 999);
    IConcat(&b, 456);
    IConcat(&b, 789);
    SConcat(&b, 999);
    SConcat(&b, 100);

    char buf[64];
    for (vector<vector<int> >::iterator v_iter = b.begin(); v_iter != b.end(); v_iter++) 
    {
        for (vector<int>::iterator iter = v_iter->begin(); iter != v_iter->end(); iter++) 
        {
            sprintf(buf, "%d ", *iter);
            cout << buf;
        }
        buf[0] = 0;
        cout << endl;
    }

    return 0;
}
