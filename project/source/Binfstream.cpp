#include "Binfstream.h"

#include <iostream>
#include <fstream>

#include <string>
#include <bitset>

#define byte unsigned char

using namespace std;

FILE *wfopen(wstring wfileName, const char *mode) {
    string fileName = string(wfileName.begin(), wfileName.end());
    return fopen(fileName.c_str(), mode);
}

Binfstream::Binfstream(wstring fileName): fileName(fileName){}

Binfstream::operator bool() const{
    FILE *file = wfopen(fileName, "rb");
    bool boolVal = (bool)file;
    fclose(file);
    return boolVal;
}

Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits){
    FILE *file = wfopen(b_fout.fileName, "wb");
    if (!file) throw wstring(L"Ошибка открытия или создания файла ") + b_fout.fileName;

    byte addition = bits.size() % 8 == 0 ? 0 : 8 - (bits.size() % 8);

    byte *head = new byte[3];
    head[0] = 'H', head[1] = 'C', head[2] = addition;
    fwrite(head, sizeof(byte), 3, file);
    delete head;

    int buffer_size = addition == 0 ? bits.size() / 8 : bits.size() / 8 + 1;
    byte *buffer = new byte[buffer_size];

    for (int i = 0; i < buffer_size; i++) {
        try {
            if (i == buffer_size - 1) buffer[i] = (byte)stoi(bits.substr(8 * i) + wstring(addition, L'0'), nullptr, 2);
            else buffer[i] = (byte)stoi(bits.substr(8 * i, 8), nullptr, 2);
        } catch (invalid_argument e) {
            throw wstring(L"При выводе в бинарный файл в строке бит встретились некорректные символы");
        }
    }

    fwrite(buffer, sizeof(byte), buffer_size, file);

    delete buffer;
    fclose(file);

    return b_fout;
}

Binfstream& operator>>(Binfstream& b_fin, std::wstring &bits){
    FILE *file = wfopen(b_fin.fileName, "rb");
    if (!file) throw wstring(L"Ошибка чтения файла ") + b_fin.fileName + wstring(L" или файл не существует");
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size < 3) {
        fclose(file);
        throw wstring(L"Некорректный формат файла ") + b_fin.fileName;
    }

    fseek(file, 0, SEEK_SET);
    byte *head = new byte[3];
    fread(head, sizeof(byte), 3, file);
    if (head[0] != 'H' || head[1] != 'C' || head[2] > 7) {
        fclose(file);
        throw wstring(L"Некорректный формат файла ") + b_fin.fileName;
    }

    byte addition = head[2];
    delete head;

    byte buffer;
    for (int i = 0; i < file_size - 3; i++) {
        auto to_wstring = [] (string str) {
            return wstring(str.begin(), str.end());
        };

        fread(&buffer, sizeof(byte), 1, file);
        bitset<8> bs(buffer);
        if (i == file_size - 4) bits += to_wstring(bs.to_string().substr(0, 8 - addition));
        else bits += to_wstring(bs.to_string());
    }

    fclose(file);
    return b_fin;
}