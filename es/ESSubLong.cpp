#include "ESSubLong.h"

using namespace ES3;

uint32_t ESSubLong::get() {
    return mLong;
}

void ESSubLong::set(uint32_t l) {
    mRecordSize = 4;
    mLong = l;
}

void ESSubLong::read(std::ifstream &ifs) {

    //read the size of the string
    ifs.read((char *) &mRecordSize, sizeof(uint32_t));
    ifs.read((char *) &mLong, mRecordSize);
}
