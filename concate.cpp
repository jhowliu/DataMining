#include "concate.h"
#include <iostream>

// S-Concatenation in the different itemsets.
vector<vector<string> > *SConcat(vector<vector<string> > *p, string candidate) {
    vector<string> tmp;

    tmp.push_back(candidate);
    p->push_back(tmp);

    return p;
}

// I-Concatenation in the same itemset.
vector<vector<string> > *IConcat(vector<vector<string> > *p, string candidate) {
    vector<string> *tmp = &(p->back());

    tmp->push_back(candidate);

    return p;
}

// Testing code
int main(int argc, char** argv) {
    vector<vector<string> > b;
    vector<string> a;

    a.push_back("Yoo");
    b.push_back(a);
    IConcat(&b, "Noo");
    IConcat(&b, "HaHa");
    SConcat(&b, "Hello");
    SConcat(&b, "World");

    for (vector<vector<string> >::iterator v_iter = b.begin(); v_iter != b.end(); v_iter++) 
    {
        for (vector<string>::iterator iter = v_iter->begin(); iter != v_iter->end(); iter++) 
        {
            cout << *iter + " ";
        }
        cout << endl;
    }

    return 0;
}
