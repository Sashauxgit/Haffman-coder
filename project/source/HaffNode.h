#ifndef HNODE
#define HNODE

#include "IntermediaData.h"

#include <string>
using namespace std;

class HaffNode{
    unsigned int weight;
    HaffNode *left;
    HaffNode *right;
    wstring wayCode;
public:
    const wchar_t *symbol;
    HaffNode(const wchar_t* symbol = nullptr, wstring wayCode = L"");
    ~HaffNode();
    HaffNode *extend(const wchar_t *newSymbol);
    void recount(wstring wayCode = L"");
    HaffNode *findChar(wchar_t character);
    HaffNode& operator ++ (int);
    unsigned int getWeight() const;
    HaffNode *getLeft();
    HaffNode *getRight();
    wstring getCode();
    friend void swapNode(HaffNode *node_1, HaffNode *node_2);
    friend HaffNode *paveWay(HaffNode *curElem, wstring code);
    friend HaffNode *paveWay(HaffNode *curElem, wchar_t code);
};

bool operator > (const HaffNode& obj1, const HaffNode& obj2);
bool operator < (const HaffNode& obj1, const HaffNode& obj2);

void swapNode(HaffNode *node_1, HaffNode *node_2);
HaffNode *paveWay(HaffNode *curElem, wstring code);
HaffNode *paveWay(HaffNode *curElem, wchar_t code);

#endif