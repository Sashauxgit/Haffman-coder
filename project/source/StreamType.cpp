#include "StreamType.h"

using namespace std;

Console::Console(std::string arg){
    if (arg == string("decoder")) decoder = true;
    else decoder = false;
}

File::File(std::string fileName):fileName(fileName){}

File::~File() {
    fin.close();
    fout.close();
}

BinFile::BinFile(string fileName):fileName(fileName), binary(Binfstream(fileName)){}

std::string Console::read(){
    string message;
    if (decoder) cout << "Введите закодированную последовательность бит: ";
    else cout << "Введите сообщение с символами из заданного алфавита: ";
    getline(cin, message); // Чтение входных данных с консоли
    return message;
}

void Console::write(string result){
    if (decoder) cout << "\nРезультат декодирования:\n";
    else cout << "\nРезультат кодирования:\n";
    cout << result << "\n"; // Вывод результата на консоль
}

std::string File::read(){
    string message;
    getline(fin, message); // Чтение входных данных с потока
    cout << "\nИз файла: " << fileName << " считано входное сообщение: " << message << "\n";
    return message;
}

void File::write(string result){
    fout << result << "\n"; // Вывод результата в поток
    cout << "\nРезультат записан в файл: " << fileName << "\n";
}

std::string BinFile::read(){
    string message;
    binary >> message; // Чтение входных данных из бинарника
    cout << "\nИз бинарного файла: " << fileName << " считана последовательность бит сжатого сообщения: " << message << "\n";
    return message;
}

void BinFile::write(string result){
    binary << result; // Вывод результата в бинарник
    cout << "\nСжатый результат записан в бинарный файл: " << fileName << "\n";
}

void File::r_open() {
    fin.open(fileName);
    if (!fin) throw string("Ошибка чтения файла ") + fileName + string(" или файл не существует");
}

void File::w_open() {
    fout.open(fileName);
    if (!fout) throw string("Ошибка открытия или создания файла ") + fileName;
}

StreamType::~StreamType(){}