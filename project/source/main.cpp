#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "StreamType.h"
#include "HaffCoder.h"

using namespace std;

StreamType *in_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-i");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--in");
    if (it == arguments.end()) return new Console(arguments[0]);
    it++;
    if (it == arguments.end() || (*it)[0] == '-') throw invalid_argument("Обязательный аргумент опции '-i/--in' отсутствует");
    
    if (arguments[1] == "encoder" || (find(arguments.begin(), arguments.end(), "-b") == arguments.end() 
&& find(arguments.begin(), arguments.end(), "--binary") == arguments.end())) {
        File *stream = new File(*it);
        stream->r_open();
        return stream;
    } else return new BinFile(*it);
}

StreamType *out_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-o");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--out");
    if (it == arguments.end()) return new Console(arguments[0]);
    it++;
    if (it == arguments.end() || (*it)[0] == '-') throw invalid_argument("Обязательный аргумент опции '-o/--out' отсутствует");
    
    if (arguments[1] == "decoder" || (find(arguments.begin(), arguments.end(), "-b") == arguments.end() 
&& find(arguments.begin(), arguments.end(), "--binary") == arguments.end())) {
        File *stream = new File(*it);
        stream->w_open();
        return stream;
    } else return new BinFile(*it);
}

InterData *demo_analys(vector<string> &arguments) {
    vector<string>::iterator it = find(arguments.begin(), arguments.end(), "-d");
    if (it == arguments.end()) it = find(arguments.begin(), arguments.end(), "--demo");
    if (it == arguments.end()) return new InterData(no);
    it++;
    if (it == arguments.end() || (*it)[0] == '-') return new InterData(console);
    
    return new InterData(file, *it);
}

int main(int argc, char* argv[]){
    try {
        if (argc < 2) throw invalid_argument("Отсутствует режим работы программы: 'encoder/decoder'");
        vector<string> CLI = vector<string>(argc - 1);
        for (int i = 1; i < argc; i++) CLI[i - 1] = argv[i];
        if (CLI[0] != "encoder" && CLI[0] != "decoder") throw invalid_argument("Нераспознанный режим работы программы: " + CLI[0]);
        
        StreamType *inStream = in_analys(CLI);
        StreamType *outStream = out_analys(CLI);
        InterData *interdata = demo_analys(CLI);

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
        cout << ex.what() << endl;
    } catch (string message) {
        cout << message << endl;
    } // Обработка исключения кодировщика и всей программы
    
    return 0;
}