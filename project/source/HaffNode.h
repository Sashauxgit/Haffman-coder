#ifndef HNODE
#define HNODE

#include "IntermediaData.h"

#include <string>
using namespace std;

class HaffNode{
    unsigned int weight;
    HaffNode *left;
    HaffNode *right;
    string wayCode;
public:
    const char *symbol;
    HaffNode(const char* symbol = nullptr, string wayCode = "");
    ~HaffNode();
    HaffNode *extend(const char *newSymbol);
    void recount(string wayCode = "");
    HaffNode *findChar(char character);
    HaffNode& operator ++ (int);
    unsigned int getWeight() const;
    HaffNode *getLeft();
    HaffNode *getRight();
    string getCode();
    friend void swapNode(HaffNode *node_1, HaffNode *node_2);
    friend HaffNode *paveWay(HaffNode *curElem, string code);
    friend HaffNode *paveWay(HaffNode *curElem, char code);
};

bool operator > (const HaffNode& obj1, const HaffNode& obj2);
bool operator < (const HaffNode& obj1, const HaffNode& obj2);

void swapNode(HaffNode *node_1, HaffNode *node_2);
HaffNode *paveWay(HaffNode *curElem, string code);
HaffNode *paveWay(HaffNode *curElem, char code);

#endif