#ifndef B_FSTREAM
#define B_FSTREAM

#include <string>

class Binfstream {
private:
    std::string str_fileName;
    const char *ch_fileName;
public:
    Binfstream(std::string fileName);
    ~Binfstream() = default;
    operator bool() const;
    friend Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
    friend Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);
};

Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);

#endif