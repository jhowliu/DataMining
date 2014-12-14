#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>

int main(int args, char **argv)
{
    if (args < 2) {
        printf("Usage: ./a.out <fileName>");
        exit(1);
    }

    FILE *sample, *quan_sample;
    int l, m, r, q;
    char p[100];
    std::set<std::string> items;

    sample=fopen(argv[1],"r");
    quan_sample=fopen("./data/quan_sample.txt","w");
    srand((unsigned)time(NULL));
    
    while(!feof(sample)) {
        fscanf(sample, "%d", &l);   //transaction num

        for(r=l;r>0;r--)         //scan one sequence---each transaction
        {
            fscanf(sample, "%d", &m);    //item num
            fprintf(quan_sample, "%d ", m);     //print item num in out file

            for(q=m; q>0; q--)                 //scan one transaction---each item            
            {
                fscanf(sample, "%s ", p);    //item id
                std::string str(p);
                items.insert(str);
                fprintf(quan_sample, "%s,%d ", p, rand() % 100 + 1);     //print item num and quan in out file
            }
        }

        fprintf(quan_sample, "\n");
    }
    fprintf(quan_sample, "%s", "weight\n");    
    for (std::set<std::string>::iterator it = items.begin(); it != items.end(); it++) 
    {
        fprintf(quan_sample, "%s %d ", it->c_str(), rand() % 100 + 1);
    }
    
    return 0;
}
