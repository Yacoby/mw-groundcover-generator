#include <cassert>
#include "ESSubUnknown.h"

using namespace ES3;

void ESSubUnknown::read(std::ifstream &ifs) {

    //read the size of the string
    ifs.read((char *) &mRecordSize, sizeof(uint32_t));
    long recordStart = ifs.tellg();

    ifs.seekg(mRecordSize, std::ios_base::cur);
    assert(ifs.tellg() == recordStart + mRecordSize);
}