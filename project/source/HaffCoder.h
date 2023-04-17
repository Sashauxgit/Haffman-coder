#ifndef HCODER
#define HCODER

#include "HaffNode.h"

using namespace std;

class HaffCoder {
    HaffNode *root;
    HaffNode *emptyElem;
    const wchar_t nulc = L'\0';
    const wchar_t *alphabet;
    vector<wstring> codes;
    InterData& interdata;

    int processedSymbolCount = 0;
    int totalCodeLength = 0;
    vector<int> symbolFrequency;
    int minSymbolCode;
    int maxSymbolCode = 0;

    void getCodes();
    void checkTree();
public:
    HaffCoder(const wchar_t *alphabet, InterData& interdata);
    ~HaffCoder();
    wstring printState() const;
    wstring encode(wchar_t character);
    wstring decode(wstring bitMessage);

    template <class T> requires is_same<T, wostream>::value || is_same<T, wofstream>::value
    friend T& operator << (T& stream, const HaffCoder& coder);
};

template <class T> requires is_same<T, wostream>::value || is_same<T, wofstream>::value
T& operator << (T& stream, const HaffCoder& coder) {
    auto printBorder = [] (T& stream, wchar_t first) {
        stream << first;
        for (int i = 0; i < 40; i++) stream << L"_";
        stream << L"\n";
    };
    
    printBorder(stream, L' ');

    stream << L"|Итоговая статистика кодировщика:\n";
    printBorder(stream, L'|');
    stream << L"|Частота символов в обработанном тексте:\n";
    for (int i = 0; i < coder.symbolFrequency.size(); i++)
        if (coder.symbolFrequency[i] > 0)
            stream << L"|'" << coder.alphabet[i] << L"' - " << coder.symbolFrequency[i] << L";\n";
    
    wchar_t buf[100];
    swprintf(buf, 100, L"|\n|Средняя длина кода: %.2f\n", ((float)coder.totalCodeLength / (float)coder.processedSymbolCount));
    stream << buf;
    stream << L"|Максимальная длина кода: " << coder.maxSymbolCode << L"\n";
    stream << L"|Минимальная длина кода: " << coder.minSymbolCode << L"\n";
    
    stream << L"|" << coder.printState();
    printBorder(stream, L'|');
    return stream;
}

#endif