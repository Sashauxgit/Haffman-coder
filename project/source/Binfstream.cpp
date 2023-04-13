#include "Binfstream.h"

#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>

#include <string>
#include <bitset>

#define byte unsigned char

using namespace std;

Binfstream::Binfstream(wstring fileName): str_fileName(fileName), ch_fileName(const_cast<const wchar_t*>(str_fileName.c_str())){}

Binfstream::operator bool() const{
    FILE *file = _wfopen(ch_fileName, L"rb");
    bool boolVal = (bool)file;
    fclose(file);
    return boolVal;
}

Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits){
    FILE *file = _wfopen(b_fout.ch_fileName, L"wb");
    if (!file) throw wstring(L"Ошибка открытия или создания файла ") + b_fout.str_fileName;

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
    FILE *file = _wfopen(b_fin.ch_fileName, L"rb");
    if (!file) throw wstring(L"Ошибка чтения файла ") + b_fin.str_fileName + wstring(L" или файл не существует");
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size < 3) throw wstring(L"Некорректный формат файла ") + b_fin.str_fileName;

    fseek(file, 0, SEEK_SET);
    byte *head = new byte[3];
    fread(head, sizeof(byte), 3, file);
    if (head[0] != 'H' || head[1] != 'C' || head[2] > 7) throw wstring(L"Некорректный формат файла ") + b_fin.str_fileName;
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