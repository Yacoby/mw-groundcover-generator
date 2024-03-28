#include <cassert>
#include "ESSubFloat.h"

using namespace ES3;

float ESSubFloat::get() {
    return mFloat;
}

void ESSubFloat::set(float f) {
    mRecordSize = sizeof(float);
    mFloat = f;
}

void ESSubFloat::read(std::ifstream &ifs) {

    //read the size of the string
    ifs.read((char *) &mRecordSize, sizeof(uint32_t));
    assert(mRecordSize == sizeof(float));
    ifs.read((char *) &mFloat, mRecordSize);
}

void ESSubFloat::write(std::ofstream &ofs) {
    uint32_t len = sizeof(float);
    ofs.write((char *) &len, sizeof(uint32_t));
    ofs.write((char *) &mFloat, sizeof(float));
}