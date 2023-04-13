#include "StreamType.h"

using namespace std;

Console::Console(std::wstring arg){
    if (arg == wstring(L"decoder")) decoder = true;
    else decoder = false;
}

File::File(std::wstring fileName):fileName(fileName){}

File::~File() {
    fin.close();
    fout.close();
}

BinFile::BinFile(wstring fileName):fileName(fileName), binary(Binfstream(fileName)){}

std::wstring Console::read(){
    wstring message;
    if (decoder) wcout << L"Введите закодированную последовательность бит: ";
    else wcout << L"Введите сообщение с символами из заданного алфавита: ";
    getline(wcin, message); // Чтение входных данных с консоли
    return message;
}

void Console::write(wstring result){
    if (decoder) wcout << L"\nРезультат декодирования:\n";
    else wcout << L"\nРезультат кодирования:\n";
    wcout << result << L"\n"; // Вывод результата на консоль
}

std::wstring File::read(){
    wstring message;
    getline(fin, message, (wchar_t)EOF); // Чтение входных данных с потока
    wstring::iterator lastChar = message.end() - 1;
    if (*lastChar == L'\n') message.erase(lastChar);
    wcout << L"\nИз файла: " << fileName << L" считано входное сообщение: " << message << L"\n";
    return message;
}

void File::write(wstring result){
    fout << result << L"\n"; // Вывод результата в поток
    wcout << L"\nРезультат записан в файл: " << fileName << L"\n"; 
}

std::wstring BinFile::read(){
    wstring message;
    binary >> message; // Чтение входных данных из бинарника
    wcout << L"\nИз бинарного файла: " << fileName << L" считана последовательность бит сжатого сообщения: " << message << L"\n";
    return message;
}

void BinFile::write(wstring result){
    binary << result; // Вывод результата в бинарник
    wcout << L"\nСжатый результат записан в бинарный файл: " << fileName << L"\n";
}

void File::r_open() {
    fin.open([] (wstring str) {
        return string(str.begin(), str.end());
    }(fileName));
    if (!fin) throw wstring(L"Ошибка чтения файла ") + fileName + wstring(L" или файл не существует");
}

void File::w_open() {
    fout.open([] (wstring str) {
        return string(str.begin(), str.end());
    }(fileName));
    if (!fout) throw wstring(L"Ошибка открытия или создания файла ") + fileName;
}

StreamType::~StreamType(){}

bool Console::isBinary() {
    return false;
}

bool File::isBinary() {
    return false;
}

bool BinFile::isBinary() {
    return true;
}