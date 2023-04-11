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
    wofstream fout;
public:
    InterData(demoMode mode, std::string fileName = std::string());
    ~InterData();
    template <class T>
    friend InterData& operator<<(InterData& interdata, T obj);
    friend InterData& operator<<(InterData& interdata, HaffCoder& coder);
    void drowTree(HaffNode *root);
    bool isWriteToFile() const;
};

template <class T>
InterData& operator<<(InterData& interdata, T obj) {
    switch (interdata.mode) {
        case file:
            interdata.fout << obj;
            break;
        case console:
            wcout << obj;
            break;
    }

    return interdata;
}

InterData& operator<<(InterData& interdata, HaffCoder& coder);

#endif