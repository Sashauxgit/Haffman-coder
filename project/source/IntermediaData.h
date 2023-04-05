#ifndef INTERDATA
#define INTERDATA

#include <iostream>
#include <fstream>
#include <vector>

class HaffCoder;
class HaffNode;

enum printMethod {original, spaces, verticalarc, horizontalarc, rightarcangle, leftarcangle};
enum demoMode {console, file, no};

using namespace std;

unsigned int LKRdetour(HaffNode *curElem, vector<HaffNode*>& nodeSequence);

class InterData{
    demoMode mode;
    ofstream fout;
public:
    InterData(demoMode mode, std::string fileName = std::string());
    ~InterData();
    template <class T>
    friend InterData& operator<<(InterData& interdata, T obj);
    void drowTree(HaffNode *root);
    bool isWriteToFile() const;
};

template <class T>
InterData& operator<<(InterData& interdata, T obj);

#endif