#ifndef STREAM_T
#define STREAM_T

#include "Binfstream.h"

#include <iostream>
#include <fstream>
#include <string>

class StreamType {
public:
    virtual ~StreamType() = 0;
    virtual std::wstring read() = 0;
    virtual void write(std::wstring result) = 0;
    virtual bool isBinary() = 0;
};

class Console: public StreamType {
private:
    bool decoder;
public:
    Console(std::string arg);
    std::wstring read();
    void write(std::wstring result);
    bool isBinary();
};

class File: public StreamType {
private:
    std::wifstream fin;
    std::wofstream fout;
    std::string fileName;
public:
    File(std::string fileName);
    ~File();
    void r_open();
    void w_open();
    std::wstring read();
    void write(std::wstring result);
    bool isBinary();
};

class BinFile: public StreamType {
private:
    Binfstream binary;
    std::string fileName;
public:
    BinFile(std::string fileName);
    ~BinFile() = default;
    std::wstring read();
    void write(std::wstring result);
    bool isBinary();
};

#endif