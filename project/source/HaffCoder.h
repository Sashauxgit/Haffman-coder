#ifndef HCODER
#define HCODER

#include "HaffNode.h"

using namespace std;

class HaffCoder {
    HaffNode *root;
    HaffNode *emptyElem;
    const char nulc = '\0';
    const char *alphabet;
    string *codes;
    InterData& interdata;
    void getCodes();
    void checkTree();
public:
    HaffCoder(const char *alphabet, InterData& interdata);
    ~HaffCoder();
    string encode(char character);
    string decode(string bitMessage);
};

#endif