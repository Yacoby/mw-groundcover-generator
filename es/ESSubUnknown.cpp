#include <cassert>
#include "ESSubUnknown.h"

using namespace ES3;

void ESSubUnknown::read(std::ifstream &ifs) {
    uint32_t recordSize;
    ifs.read((char *) &recordSize, sizeof(uint32_t));
    long recordStart = ifs.tellg();
    ifs.seekg(recordSize, std::ios_base::cur);
    assert(ifs.tellg() == recordStart + recordSize);
}