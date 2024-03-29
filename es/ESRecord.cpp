#include "ESRecord.h"

ES3::ESRecord::~ESRecord() {
}

void ES3::ESRecord::handleUnknownRecord(std::ifstream &ifs) {
    ESSubUnknown unk;
    unk.read(ifs);
}
