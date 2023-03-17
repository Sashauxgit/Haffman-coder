#include "IntermediaData.h"

#include "HaffCoder.h"

#include <string>
#include <Windows.h>

InterData::InterData(ofstream *fout): fout(fout){} // Конструктор

void InterData::drowTree(HaffNode *root){ // Функция отрисовки дерева
    vector<HaffNode*> nodeSequence; // Последовательность ЛКП-обхода
    int height = LKRdetour(root, nodeSequence); // ЛКП-обход и измерение высоты

    for (int i = 1; i <= height; i++) { // Цикл по уровням отрисовки
        for (int j = 0; j < nodeSequence.size(); j++){
            bool isWrite = (i == nodeSequence[j]->getCode().length() - root->getCode().length() + 1); // Определение принадлежности узла к текущему уровню
            // Печать узла в формате веса и символа, если имеется, видимым, в случае принадлежности текущему уровню и невидимым, в обратном случае
            specPrint(*this, '(', isWrite);
            specPrint(*this, to_string(nodeSequence[j]->getWeight()), isWrite);
            if (nodeSequence[j]->symbol) {
                specPrint(*this, '[', isWrite);
                if (*nodeSequence[j]->symbol == '\0') specPrint(*this, "\\0", isWrite); 
                else specPrint(*this, *nodeSequence[j]->symbol, isWrite);
                specPrint(*this, ']', isWrite);
            }
            specPrint(*this, ')', isWrite);
        }
        *this << "\n";
    }
}

unsigned int LKRdetour(HaffNode *curElem, vector<HaffNode*>& nodeSequence){ //ЛКП-обход и измерение высоты дерева
    if (!curElem) return 0;
    int H1 = LKRdetour(curElem->getLeft(), nodeSequence); // Измерение высоты левого поддерева
    nodeSequence.push_back(curElem);
    int H2 = LKRdetour(curElem->getRight(), nodeSequence); // Измерение высоты правого поддерева
    return (H1 > H2 ? H1 : H2) + 1; // Возврат большей из высот с добавлением едицицы - высоты текущего уровня 
}

void specPrint(InterData& interdata, string str, bool isWrite){ // Печасть строки видимой или невидимой (пробелы вместо символов) в зависимости от аргумента isWrite
    if (isWrite) interdata << str;
    else {
        int len = str.length();
        for (int i = 0; i < len; i++) interdata << ' '; // Печать пробелов в количестве длинны строки
    }
}

void specPrint(InterData& interdata, char c, bool isWrite){ // Печасть символа видимой или невидимой (пробел вместо символа) в зависимости от аргумента isWrite
    if (isWrite) interdata << c;
    else interdata << ' ';
}