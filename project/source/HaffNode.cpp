#include "HaffNode.h"

#include <iostream>

HaffNode::HaffNode(char* symbol, string wayCode): left(nullptr), right(nullptr), symbol(symbol), wayCode(wayCode){
    if (symbol){
        if (*symbol == '\0') weight = 0; // Конструктор узла
        else weight = 1;
    } else weight = 0;
}

HaffNode::~HaffNode(){ // Деструктор узла и ветви в целом
    if (this->left) delete this->left; // Очистка памяти под левое поддерево
    if (this->right) delete this->right; // Очистка памяти под правое поддерево
}

HaffNode* HaffNode::extend(char *newSymbol){ // Росто дерева путем добавления новых узлов
    if (!this->symbol || *this->symbol != '\0') return nullptr; // Проверка соблюдения инварианта: роста дерева из особого узла
    this->left = new HaffNode(this->symbol, this->wayCode + "0"); // Создание правого сына - нового особого узла
    this->right = new HaffNode(newSymbol, this->wayCode + "1"); // Создание левого сына - узла с новым добавленным символом
    this->symbol = nullptr; // Обнуление символьного значение текущего узла (так как больше не лист)
    return this->left; // Возврат адреса нового особого узла
}

void HaffNode::recount(string wayCode){ // Пересчет весов и двоичных кодов всех потомков узла
    this->wayCode = wayCode; // Пересчет двоичного кода
    if (this->symbol) return;
    this->weight = 0; // Обнуление веса

    if (this->left){
        this->left->recount(this->wayCode + "0"); // Пересчет левого сына
        this->weight += this->left->weight; // Добавления к весу текущего узла нового веса левого узла
    }

    if (this->right){
        this->right->recount(this->wayCode + "1"); // Пересчет правого сына
        this->weight += this->right->weight; // Добавления к весу текущего узла нового веса правого узла
    }
}

HaffNode* HaffNode::findChar(char character){ // Поиск узла с заданным символом
    if (symbol && character == *symbol) return this; // Возврат адреса текущего узла в случае нахождения
    
    if (this->left){
        HaffNode *resLeft = this->left->findChar(character); // Осуществление поиска в левом поддереве
        if (resLeft) return resLeft; // Если найден, возврат
    }

    if (this->right) return this->right->findChar(character); // Осуществление поиска в правом поддереве и возврат результата
    return nullptr;
}

HaffNode& HaffNode::operator ++ (int){
    this->weight += 1; // Инкремент веса узла
    return *this;
}

bool operator > (const HaffNode& obj1, const HaffNode& obj2){
    return obj1.getWeight() > obj2.getWeight(); // Возврат сравнения весов узлов
}

bool operator < (const HaffNode& obj1, const HaffNode& obj2){
    return obj1.getWeight() < obj2.getWeight(); // Возврат сравнения весов узлов
}

unsigned int HaffNode::getWeight() const{
    return this->weight; // Возврат веса узла
}

HaffNode* HaffNode::getLeft(){
    return this->left; // Возврат адреса левого сына узла
}

HaffNode* HaffNode::getRight(){
    return this->right; // Возврат адреса правого сына узла
}

string HaffNode::getCode(){
    return this->wayCode; // Вовзрат двоичного кода узла
}

void swapNode(HaffNode *node_1, HaffNode *node_2){ // обмен данными всех полей узлов
    std::swap(node_1->weight, node_2->weight);
    std::swap(node_1->left, node_2->left);
    std::swap(node_1->right, node_2->right);
    std::swap(node_1->symbol, node_2->symbol);
    std::swap(node_1->wayCode, node_2->wayCode);
}

HaffNode *paveWay(HaffNode *curElem, string code){ // Проложение пути по двоичному коду и возврат найденного по двоичному коду узла
    if (!curElem) curElem = new HaffNode; // Создание нового узла в случае отсутствия дерева
    int len = code.length();
    for (int i = 0; i < len; i++){ // Цикл по каждому биту кода
        switch (code[i]) {
            case '0':
                if (!curElem->left) curElem->left = new HaffNode(nullptr, curElem->wayCode + "0"); // Создание левого сына в случае его отсутствия
                curElem = curElem->left; // Переход к левому сыну
                break;

            case '1':
                if (!curElem->right) curElem->right = new HaffNode(nullptr, curElem->wayCode + "1"); // Создание правого сына в случае его отсутствия
                curElem = curElem->right; // Переход к правому сыну
                break;
            default:
                throw invalid_argument(string("Закодированное сообщение имеет неверный формат: встретилось: '") + code[i] + string("'\n"));
        }
    }
    return curElem; // Возврат адреса найденного узла 
}

HaffNode *paveWay(HaffNode *curElem, char code){
    if (!curElem) curElem = new HaffNode;
    
    switch (code) {
        case '0':
            if (!curElem->left) curElem->left = new HaffNode(nullptr, curElem->wayCode + "0"); // Создание левого сына в случае его отсутствия
            curElem = curElem->left; // Переход к левому сыну
            break;

        case '1':
            if (!curElem->right) curElem->right = new HaffNode(nullptr, curElem->wayCode + "1"); // Создание правого сына в случае его отсутствия
            curElem = curElem->right; // Переход к правому сыну
            break;
        default:
            throw invalid_argument(string("Ошибка кодировщика: Закодированное сообщение имеет неверный формат: встретилось: '") + code + string("'\n"));
    }

    return curElem; // Возврат адреса найденного узла 
}