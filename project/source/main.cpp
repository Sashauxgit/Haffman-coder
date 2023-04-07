#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "StreamType.h"
#include "HaffCoder.h"

using namespace std;

vector<string>::iterator binary_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-b");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--binary");
    return it;
}

StreamType *in_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-i");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--in");
    if (it == arguments.end()) return new Console(arguments[0]);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == '-') throw invalid_argument("Обязательный аргумент опции '-i/--in' отсутствует");
    
    StreamType* st;
    if (arguments[0] == "encoder" || (binary_analys(arguments) == arguments.end())) {
        File *stream = new File(*it);
        stream->r_open();
        st = stream;
    } else st = new BinFile(*it);

    arguments.erase(it);
    return st;
}

StreamType *out_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-o");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--out");
    if (it == arguments.end()) return new Console(arguments[0]);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == '-') throw invalid_argument("Обязательный аргумент опции '-o/--out' отсутствует");

    StreamType* st;
    if (arguments[0] == "decoder" || (binary_analys(arguments) == arguments.end())) {
        File *stream = new File(*it);
        stream->w_open();
        st = stream;
    } else st = new BinFile(*it);

    arguments.erase(it);
    return st;
}

InterData *demo_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-d");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--demo");
    if (it == arguments.end()) return new InterData(no);
    arguments.erase(it);
    if (it == arguments.end() || (*it)[0] == '-') return new InterData(console);
    
    string fileName = *it;
    arguments.erase(it);

    return new InterData(file, fileName);
}

string instruction(char *programName) {
    string s = "Инструкция работы с программой:\n";
    s += "\nИсполняемый файл программы для запуска: " + string(programName);
    s += "\nЗапускать программу следует в терминале, "; 
    s += "предварительно установив кодировку Unicode командой: chcp 65001\n";
    s += "Программа запускается с аргументами командной строки:\n";
    s += "\tПервый главный аргумент - режим работы программы:\n";
    s += "\t\t'encoder' - исользование кодировщика для кодирования сообщения;\n";
    s += "\t\t'decoder' - исользование кодировщика для декодирования сообщения.\n\n";
    s += "\tДалее поведение ввода и вывода данных для кодирования/декодирования определяется следующими опциями:\n";
    s += "\t\t-i/--in fileName - путь до файла для чтения входных данных;\n";
    s += "\t\t-o/--our fileName - путь до файла для записи выходных данных;\n";
    s += "\t\tПри отсутствии вышеупомянутых опций считывание или запись будут производится в консоли;\n\n";
    s += "\t\t-d/--demo fileName - путь до файла для вывода пошаговой демонстрации алгоритма кодировщика.\n";
    s += "\t\tПри отсутствии имени файла вышеупомянутой опции вывод будет производится в консоль. ";
    s += "При отсутсвии самой опции вывод промежуточных данных не производится.\n\n";
    s += "\t\tОпция -b/--binary означает запись или чтение закодированных сжатых данных в бинарный файл (из бинарного файла) ";
    s += "в виде последовательности бит.\n";
    s += "\t\tПри отсутствии данной опции закодированный код для чтения или записи определяется как ";
    s += "последовательность символов '0' или '1'. Программа работает с обычными текстовыми файлами.\n";

    for (int i = 0; i < s.length(); i++)
        if (s[i] == '\t') s.replace(i, 1, 4, ' ');

    return s;
}

int main(int argc, char* argv[]){
    try {
        if (argc < 2) throw invalid_argument("Отсутствует режим работы программы: 'encoder/decoder'");

        if (argc == 2 && (string(argv[1]) == "-h" || string(argv[1]) == "--help")) throw instruction(argv[0]);

        vector<string> CLI = vector<string>(argc - 1);
        for (int i = 1; i < argc; i++) CLI[i - 1] = argv[i];
        if (CLI[0] != "encoder" && CLI[0] != "decoder") throw invalid_argument("Не удалось распознать режим работы программы: " + CLI[0]);
        
        StreamType *inStream = in_analys(CLI);
        StreamType *outStream = out_analys(CLI);
        InterData *interdata = demo_analys(CLI);

        vector<string>::iterator b_it = binary_analys(CLI);
        if (b_it != CLI.end()) {
            if (!inStream->isBinary() && !outStream->isBinary())
                throw invalid_argument("Лишняя опция: " + *b_it + ". Отсутствует аргумент для работы с бинарным файлом");
            CLI.erase(b_it);
        }
        CLI.erase(CLI.begin());
        if (CLI.size() > 0) throw invalid_argument("Не удалось распознать опцию или аргумент программы: " + CLI[0]);

        const char *alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?:() "; // Алфавит
        
        HaffCoder coder = HaffCoder(alphabet, *interdata); // Создание объекта-кодировщика
        string inMessage = inStream->read(); // Входная строка
        string outMessage; // Выходная результирующая строка
        
        *interdata << "\nПромежуточные данные:\n";

        if (CLI[0] == "decoder") outMessage = coder.decode(inMessage); // Декодирование входной последовательности бит
        else 
            for (string::iterator symbol_i = inMessage.begin(); symbol_i != inMessage.end(); symbol_i++) 
                outMessage += coder.encode(*symbol_i); // Кодирование входного сообщения
        
        outStream->write(outMessage);
        if (interdata->isWriteToFile()) {
            if (CLI[0] == "decoder") *interdata << "\nРезультат декодирования:\n";
            else *interdata << "\nРезультат кодирования:\n";
            *interdata << outMessage;
        }

        delete inStream;
        delete outStream;
        delete interdata;
    } catch (invalid_argument ex){
        cout << ex.what() << "\nДля вызова инструкции работы с программой используйте команду: '";
        cout << argv[0] << " -h' или '" << argv[0] << " --help'" << endl;
    } catch (string message) {
        cout << message << endl;
    } // Обработка исключения кодировщика и всей программы
    
    return 0;
}