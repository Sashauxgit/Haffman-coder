#ifndef B_FSTREAM
#define B_FSTREAM

#include <string>

class Binfstream {
private:
    std::wstring str_fileName;
    const wchar_t *ch_fileName;
public:
    Binfstream(std::wstring fileName);
    ~Binfstream() = default;
    operator bool() const;
    friend Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
    friend Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);
};

Binfstream& operator<<(Binfstream &b_fout, const std::wstring &bits);
Binfstream& operator>>(Binfstream &b_fin, std::wstring &bits);

#endif