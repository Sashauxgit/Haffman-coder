#ifndef B_FSTREAM
#define B_FSTREAM

#include <string>

#include <cstdio>
#include <cstdlib>

class Binfstream {
private:
    std::wstring fileName;
public:
    Binfstream(std::wstring fileName);
    ~Binfstream() = default;
    operator bool() const;
    friend Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
    friend Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);
};

FILE *wfopen(std::wstring fileName, const char *mode);
Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);

#endif