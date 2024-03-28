#include "ESSubString.h"

using namespace ES3;

std::string ESSubString::get() {
    return mSubStr;
}

void ESSubString::read(std::ifstream &ifs) {
    uint32_t recordSize;
    ifs.read((char *) &recordSize, sizeof(uint32_t));

    char *tSubStr;
    tSubStr = new char[recordSize + 4];
    ifs.read(tSubStr, recordSize);
    tSubStr[recordSize] = '\0';
    mSubStr = tSubStr;
    delete[] tSubStr;
}