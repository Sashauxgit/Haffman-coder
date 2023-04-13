#include "HaffCoder.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>

HaffCoder::HaffCoder(const wchar_t *alphabet, InterData& interdata): alphabet(alphabet), interdata(interdata){ // Конструктор
    getCodes(); // Генерация специальных кодов алфавита
    interdata << L"Алфавит и сгенерированные специальные кода каждого символа:\n";
    for (int i = 0; i < wcslen(alphabet); i++) interdata << alphabet[i] << L" - " << codes[i] << L"\n"; // Вывод алфавита
    interdata << L"\n";
    root = new HaffNode(&nulc); // Генерация корня дерева Хаффмана
    emptyElem = root; // Сохранение адреса особого узла
    symbolFrequency = vector<int>(wcslen(alphabet));
    for (vector<int>::iterator iter = symbolFrequency.begin(); iter != symbolFrequency.end(); iter++)
        *iter = 0;
}

HaffCoder::~HaffCoder(){ // Деструктор кодировщика
    delete root; // Очистка памяти под дерево Хаффмана
}

void HaffCoder::getCodes(){ // Генерация специальных кодов алфавита
	size_t count = wcslen(alphabet);
    int k = count;
    int p = 0; // Примерная длинна кодовых сообщений
    int accum = 1;
    while (k != 1){
        k = k / 2;
        p++;
        accum *= 2;
    }
    int q = count - accum; // Разница между истинным количеством символов и ближайшей степенью двойки
    
    codes = vector<wstring>(count); // Создание массива специальных кодов
	wchar_t *bitstr = new wchar_t[p+2]; // Выделение памяти под код одного символа
	for (int i = 1; i <= count; i++){ // Цикл по всем символам алфавита
		if (i >= 1 && i <= 2*q){
            _itow(i - 1, bitstr, 2); // Преобразование номера символа в его двоичный код
            codes[i - 1] = wstring(bitstr);
            while (codes[i - 1].length() != p + 1) codes[i - 1] = L"0" + codes[i - 1]; // Дополнение двоичного кода до нужной длинны нулями
        } else {
            _itow(i - q - 1, bitstr, 2); // Преобразование номера символа в его двоичный код
            codes[i - 1] = wstring(bitstr);
            while (codes[i - 1].length() != p) codes[i - 1] = L"0" + codes[i - 1]; // Дополнение двоичного кода до нужной длинны нулями
        }
	}
    delete bitstr;
}

void HaffCoder::checkTree(){ // Проверка дерева Хаффмана на упорядоченность
    root->recount(); // Пересчет весов и двоичных кодов дерева
    interdata << L"с пересчитанными весами:\n";
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

            interdata << L"\nТребуется перевесить ветки:\n";
            interdata.drowTree(branch_1);
            interdata << L"\nи\n\n";
            interdata.drowTree(branch_2);

            swapNode(branch_1, branch_2); // Перевес узлов
            nodeSequence.clear(); // Очистка последовательности

            interdata << L"\nПовторный пересчет всех весов и перевешивание дерева в случае необходимости:\n";
            interdata << L"Дерево с перевешанными ветками и ";
            checkTree(); // Повторная проверка дерева на упорядоченность после модификации
            break;
        }
    }
}

wstring HaffCoder::encode(wchar_t character){ // Функция кодирования символа
    interdata << "---------------------------------\n";
    interdata << L"На вход кодировщику поступает символ '" << character << L"'\n";
    const wchar_t *charPtr = wcschr(alphabet, character); // Проверка наличия символа в алфавите
    if (!charPtr) throw wstring(L"Ошибка кодировщика: Символ: '") + character + wstring(L"' не распознан кодировщиком (не принадлежит его алфавиту)\n");
    
    interdata << L"Текущее дерево кодировщика:\n";
    interdata.drowTree(root);
    wstring result; // Для последовательности бит - результата кодирования
    HaffNode *curElem = root->findChar(character); // Проверка наличия узла с текущим символом в дереве
    
    if (curElem){ // При наличии
        result = ((*curElem)++).getCode(); // Добавления к пустому результату код найденного узла и инкремент его веса
        interdata << L"\nСимвол '" << character << L"' уже встречался в кодируемом сообщении и присутствует в дереве => ";
        interdata << L"вес узла с этим символом в дереве учеличивается на 1:\n";
        interdata.drowTree(root);
        interdata << L"\nКод символа '" << character << L"', собранный по пути к узлу дерева: " << result << L"\n";
    } else { // При отсутствии
        interdata << L"\nСимвол '" << character << L"' еще не встречался в кодируемом сообщении =>\n";
        interdata << L"1) Берется код пустого символа: " << emptyElem->getCode() << L"\n";
        interdata << L"2) К нему добавляется спец-код символа '" << character << L"' из алфавита: " << codes[charPtr - alphabet] << L"\n";
        result = emptyElem->getCode() + codes[charPtr - alphabet]; // Добавление к пустому результату код пустого узла а затем спец код символа
        emptyElem = emptyElem->extend(charPtr); // Расширение дерева путем добавления новых узлов (с новым символом) к пустому узлу, сохранение нового пустого узла
        interdata << L"3) К узлу с пустым символом добавляются два новых узла:\n";
        interdata << L"Левый сын - новый пустой узел, а правый - узел с новым добавленным символом: '" << character << L"':\n";
        interdata.drowTree(root);
    }
    interdata << L"\nПроизводится пересчет всех весов и перевешивание дерева в случае необходимости:\n";
    interdata << L"Дерево ";
    checkTree(); // Проверка дерева на упорядоченность

    processedSymbolCount++;
    totalCodeLength += result.length();
    symbolFrequency[charPtr - alphabet]++;
    if (result.length() >= maxSymbolCode) maxSymbolCode = result.length();
    if (result.length() <= minSymbolCode) minSymbolCode = result.length();

    interdata << L"\nРезультат кодирования символа '" << character << L"' - " << result << L"\n";
    interdata << printState();
    interdata << L"---------------------------------\n";
    return result; // Возврат результата
}

wstring HaffCoder::decode(wstring bitMessage){ // Функция декодирования сообщения
    wstring result; // Создание пустого результата

    interdata << L"Начало работы декодировщика: на вход декодеру подается последовательность бит:\n";
    interdata << bitMessage << "\n";

    HaffNode *specTree = new HaffNode;
    int len = wcslen(alphabet);
    for (int i = 0; i < len; i++) paveWay(specTree, codes[i])->symbol = alphabet + i; // Генерация дерева специальных кодов алфавита
    
    len = bitMessage.length();
    HaffNode *curElem;
    int i = 0;
    while (i < len) { // Главный цикл декодирования
        int i_prev = i;
        interdata << L"---------------------------------\n";
        interdata << L"Текущее дерево кодировщика:\n";
        interdata.drowTree(root);
        interdata << L"\nОставшееся нераскодированное сообщение: " << bitMessage.substr(i) << L"\n";
        interdata << L"\nПо первым битам нераскодированной части сообщения строится путь в дереве Хаффмана до тех пор, пока не встретится лист\n";
        curElem = root; // Начало движения от корня дерева
        while (!curElem->symbol){
            if (i >= len) throw L"Ошибка кодировщика: Закодированное сообщение имеет неверный формат: не хватает бит для полного декодирования\n";
            curElem = paveWay(curElem, bitMessage[i]); // Прокладка маршрута до листа в дереве Хаффмана
            i++;
        }
        interdata << L"Следующий набор бит: " << curElem->getCode() << L" привел декодера к узлу с символом '";
        if (*curElem->symbol == '\0'){ // Если встретился особый узел
            interdata << L"\\0' - пустым символом =>\n=> по следующим битам нераскодированной части сообщения определяется символ из специальной кодировки\n";
            interdata << L"\nОставшееся нераскодированное сообщение: " << bitMessage.substr(i) << L"\n";
            curElem = specTree;
            while (!curElem->symbol){
                if (i >= len) throw L"Ошибка кодировщика: Закодированное сообщение имеет неверный формат: не хватает бит для полного декодирования\n";
                curElem = paveWay(curElem, bitMessage[i]); // Прокладка маршрута до листа в дереве специальной кодировки
                i++;
            }
            interdata << L"Следующий набор бит: " << curElem->getCode() << L" привел декодера к узлу с символом '" << *curElem->symbol << L"' в дереве специальной кодировки, который и будет очередным результатом декодирования\n";
            emptyElem = emptyElem->extend(curElem->symbol); // Расширение дерева путем добавления новых узлов (с новым символом) к пустому узлу, сохранение нового пустого узла
            interdata << L"\nК узлу с пустым символом добавляются два новых узла:\n";
            interdata << L"Левый сын - новый пустой узел, а правый - узел с новым добавленным символом: '" << *curElem->symbol << L"':\n";
            interdata.drowTree(root);
        } else { // Если встретился узел с уже имеющимся в сообщении символом
            interdata << *curElem->symbol << L"', который и будет очередным результатом декодирования\n";
            (*curElem)++; // Инкремент веса встретившегося узла
            interdata << L"Вес узла с этим символом в дереве учеличивается на 1:\n";
            interdata.drowTree(root);
        }
        
        result.push_back(*curElem->symbol); // Добавление символа узла, на котором декодировщик остановился, к результату
        
        processedSymbolCount++;
        totalCodeLength = i;
        symbolFrequency[curElem->symbol - alphabet]++;
        if (i - i_prev >= maxSymbolCode) maxSymbolCode = i - i_prev;
        if (i - i_prev <= minSymbolCode) minSymbolCode = i - i_prev;

        interdata << L"\nПроизводится пересчет всех весов и перевешивание дерева в случае необходимости:\n";
        interdata << L"Дерево ";
        checkTree(); // Проверка дерева на упорядоченность

        interdata << L"\nТекущий результат декодирования сообщения: '" << result << "'\n";
        interdata << printState();
        interdata << L"---------------------------------\n";
    }
    interdata << L"Конец работы декодировщика.\n";
    delete specTree; // Зачистка памяти под дерево специальной кодировки
    return result; // Возврат результата
}


wstring HaffCoder::printState() const {
    wchar_t buf[100];
    swprintf(buf, L"Текущий процент сжатия: %.2f %\n", (1 - (float)totalCodeLength / (float)(processedSymbolCount * 8)) * 100);
    return wstring(buf);
}