#ifndef HCODER
#define HCODER

#include "HaffNode.h"

using namespace std;

class HaffCoder {
    HaffNode *root;
    HaffNode *emptyElem;
    char nulc = '\0';
    char *alphabet;
    string *codes;
    InterData& interdata;
    void getCodes();
    void checkTree();
public:
    HaffCoder(char *alphabet, InterData& interdata);
    ~HaffCoder();
    string encode(char character);
    string decode(string bitMessage);
};

#endif