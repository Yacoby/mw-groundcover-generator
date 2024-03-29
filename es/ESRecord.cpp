#include "ESRecord.h"

void ES3::ESRecord::handleUnknownRecord(std::ifstream &ifs) {
    ESSubUnknown unk;
    unk.read(ifs);
}
