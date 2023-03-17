#ifndef INTERDATA
#define INTERDATA

#include <iostream>
#include <fstream>
#include <vector>

class HaffCoder;
class HaffNode;

using namespace std;

class InterData{
    ofstream *fout;
public:
    InterData(ofstream *fout);
    template <class T>
    friend InterData& operator<<(InterData& interdata, T obj){
        if (interdata.fout) *interdata.fout << obj;
        else cout << obj;
        return interdata;
    }
    void drowTree(HaffNode *root);
};

unsigned int LKRdetour(HaffNode *curElem, vector<HaffNode*>& nodeSequence);

void specPrint(InterData& interdata, string str, bool isWrite);
void specPrint(InterData& interdata, char c, bool isWrite);

#endif