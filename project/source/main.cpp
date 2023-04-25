#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#ifdef __linux__ // Если Linux
#include <clocale> // linux
#endif


#include "StreamType.h"
#include "HaffCoder.h"

using namespace std;

wchar_t *readAlphabet() {
    wifstream alphabetFile("alphabet");
    if (!alphabetFile) return nullptr;

    wchar_t c;
    size_t symbolCount = 0;
    while (!alphabetFile.eof()) {
        alphabetFile.get(c);
        symbolCount++;
    }
    alphabetFile.close();

    wchar_t *alphabet = new wchar_t[symbolCount];

    alphabetFile.open("alphabet");
    for (int i = 0; i < symbolCount - 1; i++) alphabetFile.get(alphabet[i]);
    alphabet[symbolCount - 1] = L'\0';
    alphabetFile.close();

    return alphabet;
}

vector<wstring>::iterator binary_analys(vector<wstring> &arguments) {
    vector<wstring>::iterator it = find(arguments.begin(), arguments.end(), L"-b");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), L"--binary");
    return it;
}

StreamType *in_analys(vector<wstring> &arguments) {
    vector<wstring>::iterator it = find(arguments.begin(), arguments.end(), L"-i");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), L"--in");
    if (it == arguments.end()) return new Console(arguments[0]);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == '-') throw wstring(L"Обязательный аргумент опции '-i/--in' отсутствует");
    
    StreamType* st;
    if (arguments[0] == L"encoder" || (binary_analys(arguments) == arguments.end())) {
        File *stream = new File(*it);
        stream->r_open();
        st = stream;
    } else st = new BinFile(*it);

    arguments.erase(it);
    return st;
}

StreamType *out_analys(vector<wstring> &arguments) {
    vector<wstring>::iterator it = find(arguments.begin(), arguments.end(), L"-o");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), L"--out");
    if (it == arguments.end()) return new Console(arguments[0]);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == '-') throw wstring(L"Обязательный аргумент опции '-o/--out' отсутствует");

    StreamType* st;
    if (arguments[0] == L"decoder" || (binary_analys(arguments) == arguments.end())) {
        File *stream = new File(*it);
        stream->w_open();
        st = stream;
    } else st = new BinFile(*it);

    arguments.erase(it);
    return st;
}

InterData *demo_analys(vector<wstring> &arguments) {
    vector<wstring>::iterator it = find(arguments.begin(), arguments.end(), L"-d");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), L"--demo");
    if (it == arguments.end()) return new InterData(no);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == L'-') return new InterData(console);
    
    wstring fileName = *it;
    arguments.erase(it);

    return new InterData(file, fileName);
}

wstring instruction(wstring programName) {
    wstring s = L"Инструкция работы с программой:\n";
    s += L"\nИсполняемый файл программы для запуска: " + programName;
    s += L"\nПрограмма запускается с аргументами командной строки:\n";
    s += L"\tПервый главный аргумент - режим работы программы:\n";
    s += L"\t\t'encoder' - использование кодировщика для кодирования сообщения;\n";
    s += L"\t\t'decoder' - использование кодировщика для декодирования сообщения.\n\n";
    s += L"\tДалее поведение ввода и вывода данных для кодирования/декодирования определяется следующими опциями:\n";
    s += L"\t\t-i/--in fileName - путь к файлу для чтения входных данных;\n";
    s += L"\t\t-o/--out fileName - путь к файлу для записи выходных данных;\n";
    s += L"\t\tПри отсутствии вышеупомянутых опций считывание или запись будут производится в консоли;\n\n";
    s += L"\t\t-d/--demo fileName - путь к файлу для вывода пошаговой демонстрации алгоритма кодировщика.\n";
    s += L"\t\tПри отсутствии имени файла вышеупомянутой опции вывод будет производится в консоль. ";
    s += L"При отсутствии самой опции вывод промежуточных данных не производится.\n\n";
    s += L"\t\tОпция -b/--binary означает запись или чтение закодированных сжатых данных в бинарный файл (из бинарного файла) ";
    s += L"в виде последовательности бит.\n";
    s += L"\t\tПри отсутствии данной опции закодированный код для чтения или записи определяется как ";
    s += L"последовательность символов '0' или '1'. Программа работает с обычными текстовыми файлами.\n";

    for (int i = 0; i < s.length(); i++)
        if (s[i] == L'\t') s.replace(i, 1, 4, L' ');

    return s;
}

wstring charStr_to_wstring(const char *c_str) {
    string str = string(c_str);
    return wstring(str.begin(), str.end());
}

int main(int argc, const char* argv[]){
#ifndef __linux__ // Если не Linux
    // Для Windows
    setlocale(LC_ALL, ""); // Устанавливаем программе кодировку по умолчанию для windows
#else
    // Для Linux
    locale::global(locale("ru_RU.UTF-8"));  //Устанавливаем лоакль системы Linux
#endif

    try {
        if (argc < 2) throw wstring(L"Отсутствует режим работы программы: 'encoder/decoder'");

        if (argc == 2 && (charStr_to_wstring(argv[1]) == L"-h" ||charStr_to_wstring(argv[1]) == L"--help")) throw instruction(charStr_to_wstring(argv[0]));

        vector<wstring> CLI = vector<wstring>(argc - 1);
        for (int i = 1; i < argc; i++) CLI[i - 1] = charStr_to_wstring(argv[i]);
        if (CLI[0] != L"encoder" && CLI[0] != L"decoder") throw wstring(L"Не удалось распознать режим работы программы: ") + CLI[0];
        
        bool decoder = CLI[0] == L"decoder";

        StreamType *inStream = in_analys(CLI);
        StreamType *outStream = out_analys(CLI);
        InterData *interdata = demo_analys(CLI);

        vector<wstring>::iterator b_it = binary_analys(CLI);
        if (b_it != CLI.end()) {
            if (!inStream->isBinary() && !outStream->isBinary())
                throw wstring(L"Лишняя опция: " + *b_it + L". Отсутствует аргумент для работы с бинарным файлом");
            CLI.erase(b_it);
        }
        CLI.erase(CLI.begin());
        if (CLI.size() > 0) throw wstring(L"Не удалось распознать опцию или аргумент программы: ") + CLI[0];

        const wchar_t *alphabet = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?:() "; // Алфавит
        const wchar_t *fileAlphabet = readAlphabet();
        if (fileAlphabet) alphabet = fileAlphabet;
        
        HaffCoder coder = HaffCoder(alphabet, *interdata); // Создание объекта-кодировщика
        wstring inMessage = inStream->read(); // Входная строка
        if (inMessage.size() == 0) throw wstring(L"Нечего кодировать/декодировать (Пустое входное сообщение)");
        wstring outMessage; // Выходная результирующая строка
        
        *interdata << L"\nПромежуточные данные:\n";

        if (decoder) outMessage = coder.decode(inMessage); // Декодирование входной последовательности бит
        else 
            for (wstring::iterator symbol_i = inMessage.begin(); symbol_i != inMessage.end(); symbol_i++) 
                outMessage += coder.encode(*symbol_i); // Кодирование входного сообщения
        
        outStream->write(outMessage);
        wcout << L"\n" << coder;
        if (interdata->isWriteToFile()) {
            if (decoder) *interdata << L"\nРезультат декодирования:\n";
            else *interdata << L"\nРезультат кодирования:\n";
            *interdata << outMessage << L"\n\n";
            *interdata << coder;
        }
        
        delete inStream;
        delete outStream;
        delete interdata;
        delete[] fileAlphabet;
    } catch (wstring message){
        wcout << message << L"\nДля вызова инструкции работы с программой используйте команду: '";
        wcout << charStr_to_wstring(argv[0]) << L" -h' или '" << charStr_to_wstring(argv[0]) << L" --help'" << endl;
    }  catch (const wchar_t *message){
        wcout << message << L"\nДля вызова инструкции работы с программой используйте команду: '";
        wcout << charStr_to_wstring(argv[0]) << L" -h' или '" << charStr_to_wstring(argv[0]) << L" --help'" << endl;
    } // Обработка исключения кодировщика и всей программы

    return 0;
}