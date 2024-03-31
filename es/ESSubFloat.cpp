#include "ESSubFloat.h"

#include <cassert>

using namespace ES3;

float ESSubFloat::get() {
    return mFloat;
}

void ESSubFloat::read(std::ifstream &ifs) {
    uint32_t recordSize;
    ifs.read((char *) &recordSize, sizeof(uint32_t));
    assert(recordSize == sizeof(float));
    ifs.read((char *) &mFloat, recordSize);
}