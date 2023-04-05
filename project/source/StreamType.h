#ifndef STREAM_T
#define STREAM_T

#include "Binfstream.h"

#include <iostream>
#include <fstream>
#include <string>

class StreamType {
public:
    virtual ~StreamType() = 0;
    virtual std::string read() = 0;
    virtual void write(std::string result) = 0;
};

class Console: public StreamType {
private:
    bool decoder;
public:
    Console(std::string arg);
    std::string read();
    void write(std::string result);
};

class File: public StreamType {
private:
    std::ifstream fin;
    std::ofstream fout;
    std::string fileName;
public:
    File(std::string fileName);
    ~File();
    void r_open();
    void w_open();
    std::string read();
    void write(std::string result);
};

class BinFile: public StreamType {
private:
    Binfstream binary;
    std::string fileName;
public:
    BinFile(std::string fileName);
    ~BinFile() = default;
    std::string read();
    void write(std::string result);
};

#endif