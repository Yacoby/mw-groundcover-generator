#include <cassert>
#include "ESSubUnknown.h"

using namespace ES3;

void ESSubUnknown::read(std::ifstream &ifs) {
    uint32_t recordSize;
    ifs.read((char *) &recordSize, sizeof(uint32_t));
    std::streampos recordStart = ifs.tellg();
    ifs.seekg(recordSize, std::ios_base::cur);
    assert(ifs.tellg() == recordStart + std::streampos(recordSize));
}