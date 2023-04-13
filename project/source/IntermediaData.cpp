#include "IntermediaData.h"
#include "HaffCoder.h"
#include <string>

InterData::InterData(demoMode mode, std::wstring fileName){
    this->mode = mode;
    if (this->mode == file) {
        fout.open([] (wstring str) {
        return string(str.begin(), str.end());
    }(fileName));
        if (!fout) throw wstring(L"Ошибка открытия или создания файла ") + fileName;
    }
} // Конструктор

InterData::~InterData() {fout.close();} // Деструктор

void InterData::drowTree(HaffNode *root){ // Функция отрисовки дерева
    if (mode == no) return;

    vector<HaffNode*> nodeSequence; // Последовательность ЛКП-обхода
    int height = LKRdetour(root, nodeSequence); // ЛКП-обход и измерение высоты

    const printMethod selection_0[2][2] = { // layer = 0
        {spaces, leftarcangle}, // onArc = false
        {horizontalarc, rightarcangle} // onArc = true
    };
    
    const printMethod selection[4][2] = {
        {spaces, verticalarc}, // layer = 1
        {spaces, original}, // layer = 2
        {spaces, verticalarc} // layer = 3
    };

    for (int level = 1; level <= height; level++) { // Цикл по уровням отрисовки
        for (int layer = level == 1 ? 2 : 0; layer < (level == height ? 3 : 4); layer++) { // Цикл по слоям отрисовки уровня: узлы и соединительные дуги
            bool onArc = false; // Рисуем ли горизонтальную дугу на слое
            for (int i = 0; i < nodeSequence.size(); i++){ // Цикл по всем узлам дерева
                bool isNode_on_level = (level == nodeSequence[i]->getCode().length() - root->getCode().length() + 1); // Определение принадлежности узла к текущему уровню
                wstring nodeText = L"(" + to_wstring(nodeSequence[i]->getWeight()); // собираем строковое представление узла в формате веса и символа
                if (nodeSequence[i]->symbol) {
                    nodeText += L'[';
                    if (*nodeSequence[i]->symbol == L'\0') nodeText += L"\\0"; 
                    else nodeText += *nodeSequence[i]->symbol;
                    nodeText += L']';
                }
                nodeText += L')';

                printMethod renderMode; // Режим отрисовки узла
                if (layer == 0) renderMode = selection_0[(int)onArc][(int)isNode_on_level];
                else renderMode = selection[layer - 1][(int)isNode_on_level];
                if (layer == 3 && nodeSequence[i]->getRight() == nullptr && nodeSequence[i]->getLeft() == nullptr) renderMode = spaces;
                if (isNode_on_level) onArc = !onArc;

                int len = nodeText.length();
                switch (renderMode) {
                    case original:
                        *this << nodeText; // Печать узла
                        break;
                    case spaces:
                        for (int j = 0; j < len; j++) *this << L' '; // Печать пробелов в количестве длинны строки узла
                        break;
                    case verticalarc:
                        for (int j = 0; j < len / 2 - 1; j++) *this << L' ';
                        *this << L"||"; // Печать вертикальной дуги посередине узла
                        for (int j = len / 2 + 1; j < len; j++) *this << L' ';
                        break;
                    case horizontalarc:
                        for (int j = 0; j < len; j++) *this << L'-'; // Печать символов '-' в количестве длинны строки узла (горизонтальная дуга)
                        break;
                    case rightarcangle:
                        for (int j = 0; j < len / 2 + 1; j++) *this << L'-'; // Конец горизонтальной дуги над вертикальной (правый сын)
                        for (int j = len / 2 + 1; j < len; j++) *this << L' ';
                        break;
                    case leftarcangle:
                        for (int j = 0; j < len / 2 - 1; j++) *this << L' ';
                        for (int j = len / 2 - 1; j < len; j++) *this << L'-'; // Начало горизонтальной дуги над вертикальной (левый сын)
                        break;
                }
            }
            *this << L"\n";
        }
    }
}

unsigned int LKRdetour(HaffNode *curElem, vector<HaffNode*>& nodeSequence){ //ЛКП-обход и измерение высоты дерева
    if (!curElem) return 0;
    int H1 = LKRdetour(curElem->getLeft(), nodeSequence); // Измерение высоты левого поддерева
    nodeSequence.push_back(curElem);
    int H2 = LKRdetour(curElem->getRight(), nodeSequence); // Измерение высоты правого поддерева
    return (H1 > H2 ? H1 : H2) + 1; // Возврат большей из высот с добавлением едицицы - высоты текущего уровня 
}

InterData& operator<<(InterData& interdata, HaffCoder& coder) {
    switch (interdata.mode) {
        case file:
            interdata.fout << coder;
            break;
        case console:
            wcout << coder;
            break;
    }

    return interdata;
}

bool InterData::isWriteToFile() const {
    return this->mode == file;
}