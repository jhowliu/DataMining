#include "function.h"
#include "loader.h"

#define SIZE 2048

vector<map<int, vector<Entry> > > reader(string fileName, vector<vector<int> > &plist) {
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
        plist.push_back(p);

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

    PrintMatrix(matrix, plist);

    return matrix;
}

void PrintMatrix(vector<map<int, vector<Entry> > > matrix, vector<vector<int> > plist) {
    for (int i = 0; i != matrix.size(); i++) 
    {
        for (int j = 0; j != plist[i].size(); j++) 
        {
            cout << (char)('0' + plist[i][j]) << " ";
            for (vector<Entry>::iterator iter = matrix[i][plist[i][j]].begin(); iter != matrix[i][plist[i][j]].end(); iter++) 
                cout << "(" << iter->self << "," << iter->remain << ") ";
            cout << endl;
        }
        cout << endl;
    }
}

int main(int argc, char **argv) {
    vector<map<int, vector<Entry> > > matrices;
    vector<vector<int> > plist;

    matrices = reader(argv[1], plist);
    print();
}
