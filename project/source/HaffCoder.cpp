#include "HaffCoder.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>

HaffCoder::HaffCoder(const char *alphabet, InterData& interdata): alphabet(alphabet), interdata(interdata){ // Конструктор
    getCodes(); // Генерация специальных кодов алфавита
    interdata << "Алфавит и сгенерированные специальные кода каждого символа:\n";
    for (int i = 0; i < strlen(alphabet); i++) interdata << alphabet[i] << " - " << codes[i] << "\n"; // Вывод алфавита
    interdata << "\n";
    root = new HaffNode(&nulc); // Генерация корня дерева Хаффмана
    emptyElem = root; // Сохранение адреса особого узла
}

HaffCoder::~HaffCoder(){ // Деструктор кодировщика
    delete root; // Очистка памяти под дерево Хаффмана
    delete codes; // Зачистка памяти под массив специальных кодов
}

void HaffCoder::getCodes(){ // Генерация специальных кодов алфавита
	size_t count = strlen(alphabet);
    int k = count;
    int p = 0; // Примерная длинна кодовых сообщений
    int accum = 1;
    while (k != 1){
        k = k / 2;
        p++;
        accum *= 2;
    }
    int q = count - accum; // Разница между истинным количеством символов и ближайшей степенью двойки
    
    codes = new string[count]; // Создание массива специальных кодов
	char *bitstr = new char[p+2]; // Выделение памяти под код одного символа
	for (int i = 1; i <= count; i++){ // Цикл по всем символам алфавита
		if (i >= 1 && i <= 2*q){
            itoa(i - 1, bitstr, 2); // Преобразование номера символа в его двоичный код
            codes[i - 1] = string(bitstr);
            while (codes[i - 1].length() != p + 1) codes[i - 1] = "0" + codes[i - 1]; // Дополнение двоичного кода до нужной длинны нулями
        } else {
            itoa(i - q - 1, bitstr, 2); // Преобразование номера символа в его двоичный код
            codes[i - 1] = string(bitstr);
            while (codes[i - 1].length() != p) codes[i - 1] = "0" + codes[i - 1]; // Дополнение двоичного кода до нужной длинны нулями
        }
	}
    delete bitstr;
}

void HaffCoder::checkTree(){ // Проверка дерева Хаффмана на упорядоченность
    root->recount(); // Пересчет весов и двоичных кодов дерева
    interdata << "с пересчитанными весами:\n";
    interdata.drowTree(root);
    vector<HaffNode*> nodeSequence; // Последовательность горизонтального обхода
    queue<HaffNode*> nodeQueue; // Очередь для горизонтального обхода
    nodeQueue.push(root);
    while (!nodeQueue.empty()) { // Горизонтальный обход
        nodeSequence.push_back(nodeQueue.front()); // Образование последовательности горизонтального обхода
        nodeQueue.pop();
        if (nodeSequence.back()->getRight()) nodeQueue.push(nodeSequence.back()->getRight());
        if (nodeSequence.back()->getLeft()) nodeQueue.push(nodeSequence.back()->getLeft());
    }

    HaffNode* branch_1; // Адреса ветвей, которые требуется перевесить
    HaffNode* branch_2;
    
    for (int i = nodeSequence.size() - 1; i > 0; i--){ // Проверка на неубывающий порядок обхода последовательности
        if (*(nodeSequence[i]) > *(nodeSequence[i - 1])){ // Обнаружение конфликтного узла
            branch_1 = nodeSequence[i]; // Приготовление его к перевесу
            int j = i - 1;
            while(*(nodeSequence[j]) < *branch_1) j--; // Поиск подходящего места для первого узла и подходящего второго узла на его места
            branch_2 = nodeSequence[j + 1]; // Приготовление последнего к перевесу

            interdata << "\nТребуется перевесить ветки:\n";
            interdata.drowTree(branch_1);
            interdata << "\nи\n\n";
            interdata.drowTree(branch_2);

            swapNode(branch_1, branch_2); // Перевес узлов
            nodeSequence.clear(); // Очистка последовательности

            interdata << "\nПовторный пересчет всех весов и перевешивание дерева в случае необходимости:\n";
            interdata << "Дерево с перевешанными ветками и ";
            checkTree(); // Повторная проверка дерева на упорядоченность после модификации
            break;
        }
    }
}

string HaffCoder::encode(char character){ // Функция кодирования символа
    interdata << "---------------------------------\n";
    interdata << "На вход кодировщику поступает символ '" << character << "'\n";
    const char *charPtr = strchr(alphabet, character); // Проверка наличия символа в алфавите
    if (!charPtr) throw invalid_argument(string("Ошибка кодировщика: Символ: '") + character + string("' не распознан кодировщиком (не принадлежит его алфавиту)\n"));
    
    interdata << "Текущее дерево кодировщика:\n";
    interdata.drowTree(root);
    string result; // Для последовательности бит - результата кодирования
    HaffNode *curElem = root->findChar(character); // Проверка наличия узла с текущим символом в дереве
    
    if (curElem){ // При наличии
        result = ((*curElem)++).getCode(); // Добавления к пустому результату код найденного узла и инкремент его веса
        interdata << "\nСимвол '" << character << "' уже встречался в кодируемом сообщении и присутствует в дереве => ";
        interdata << "вес узла с этим символом в дереве учеличивается на 1:\n";
        interdata.drowTree(root);
        interdata << "\nКод символа '" << character << "', собранный по пути к узлу дерева: " << result << "\n";
    } else { // При отсутствии
        interdata << "\nСимвол '" << character << "' еще не встречался в кодируемом сообщении =>\n";
        interdata << "1) Берется код пустого символа: " << emptyElem->getCode() << "\n";
        interdata << "2) К нему добавляется спец-код символа '" << character << "' из алфавита: " << codes[charPtr - alphabet] << "\n";
        result = emptyElem->getCode() + codes[charPtr - alphabet]; // Добавление к пустому результату код пустого узла а затем спец код символа
        emptyElem = emptyElem->extend(charPtr); // Расширение дерева путем добавления новых узлов (с новым символом) к пустому узлу, сохранение нового пустого узла
        interdata << "3) К узлу с пустым символом добавляются два новых узла:\n";
        interdata << "Левый сын - новый пустой узел, а правый - узел с новым добавленным символом: '" << character << "':\n";
        interdata.drowTree(root);
    }
    interdata << "\nПроизводится пересчет всех весов и перевешивание дерева в случае необходимости:\n";
    interdata << "Дерево ";
    checkTree(); // Проверка дерева на упорядоченность

    interdata << "\nРезультат кодирования символа '" << character << "' - " << result << "\n";
    interdata << "---------------------------------\n";
    return result; // Возврат результата
}

string HaffCoder::decode(string bitMessage){ // Функция декодирования сообщения
    string result; // Создание пустого результата

    interdata << "Начало работы декодировщика: на вход декодеру подается последовательность бит:\n";
    interdata << bitMessage << "\n";

    HaffNode *specTree = new HaffNode;
    int len = strlen(alphabet);
    for (int i = 0; i < len; i++) paveWay(specTree, codes[i])->symbol = alphabet + i; // Генерация дерева специальных кодов алфавита
    
    len = bitMessage.length();
    HaffNode *curElem;
    int i = 0;
    while (i < len) { // Главный цикл декодирования
        interdata << "---------------------------------\n";
        interdata << "Текущее дерево кодировщика:\n";
        interdata.drowTree(root);
        interdata << "\nОставшееся нераскодированное сообщение: " << bitMessage.substr(i) << "\n";
        interdata << "\nПо первым битам нераскодированной части сообщения строится путь в дереве Хаффмана до тех пор, пока не встретится лист\n";
        curElem = root; // Начало движения от корня дерева
        while (!curElem->symbol){
            if (i >= len) throw invalid_argument("Ошибка кодировщика: Закодированное сообщение имеет неверный формат: не хватает бит для полного декодирования\n");
            curElem = paveWay(curElem, bitMessage[i]); // Прокладка маршрута до листа в дереве Хаффмана
            i++;
        }
        interdata << "Следующий набор бит: " << curElem->getCode() << " привел декодера к узлу с символом '";
        if (*curElem->symbol == '\0'){ // Если встретился особый узел
            interdata << "\\0' - пустым символом =>\n=> по следующим битам нераскодированной части сообщения определяется символ из специальной кодировки\n";
            interdata << "\nОставшееся нераскодированное сообщение: " << bitMessage.substr(i) << "\n";
            curElem = specTree;
            while (!curElem->symbol){
                if (i >= len) throw invalid_argument("Ошибка кодировщика: Закодированное сообщение имеет неверный формат: не хватает бит для полного декодирования\n");
                curElem = paveWay(curElem, bitMessage[i]); // Прокладка маршрута до листа в дереве специальной кодировки
                i++;
            }
            interdata << "Следующий набор бит: " << curElem->getCode() << " привел декодера к узлу с символом '" << *curElem->symbol << "' в дереве специальной кодировки, который и будет очередным результатом декодирования\n";
            emptyElem = emptyElem->extend(curElem->symbol); // Расширение дерева путем добавления новых узлов (с новым символом) к пустому узлу, сохранение нового пустого узла
            interdata << "\nК узлу с пустым символом добавляются два новых узла:\n";
            interdata << "Левый сын - новый пустой узел, а правый - узел с новым добавленным символом: '" << *curElem->symbol << "':\n";
            interdata.drowTree(root);
        } else { // Если встретился узел с уже имеющимся в сообщении символом
            interdata << *curElem->symbol << "', который и будет очередным результатом декодирования\n";
            (*curElem)++; // Инкремент веса встретившегося узла
            interdata << "Вес узла с этим символом в дереве учеличивается на 1:\n";
            interdata.drowTree(root);
        }
        
        result.push_back(*curElem->symbol); // Добавление символа узла, на котором декодировщик остановился, к результату

        interdata << "\nПроизводится пересчет всех весов и перевешивание дерева в случае необходимости:\n";
        interdata << "Дерево ";
        checkTree(); // Проверка дерева на упорядоченность

        interdata << "\nТекущий результат декодирования сообщения: '" << result << "'\n";
        interdata << "---------------------------------\n";
    }
    interdata << "Конец работы декодировщика.\n";
    delete specTree; // Зачистка памяти под дерево специальной кодировки
    return result; // Возврат результата
}