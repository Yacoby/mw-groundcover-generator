#include "ESSubLong.h"

using namespace ES3;

uint32_t ESSubLong::get() {
    return mLong;
}

void ESSubLong::read(std::ifstream &ifs) {
    uint32_t recordSize;
    ifs.read((char *) &recordSize, sizeof(uint32_t));
    ifs.read((char *) &mLong, recordSize);
}