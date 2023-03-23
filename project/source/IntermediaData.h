#ifndef INTERDATA
#define INTERDATA

#include <iostream>
#include <fstream>
#include <vector>

class HaffCoder;
class HaffNode;

enum printMethod {original, spaces, verticalarc, horizontalarc, rightarcangle, leftarcangle};

using namespace std;

unsigned int LKRdetour(HaffNode *curElem, vector<HaffNode*>& nodeSequence);

class InterData{
    ofstream *fout;
public:
    InterData(ofstream *fout);
    template <class T>
    friend InterData& operator<<(InterData& interdata, T obj);
    void drowTree(HaffNode *root);
};

template <class T>
InterData& operator<<(InterData& interdata, T obj);

#endif