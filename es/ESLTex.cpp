#include "ESLTex.h"

using namespace ES3;

void ESLTex::read(std::ifstream &ifs, long recordSize) {
    std::streampos readTo = recordSize + ifs.tellg();

    while (ifs.tellg() < readTo) {
        char dataType[5];
        ifs.read(dataType, 4);

        //string must be null terminated.
        dataType[4] = '\0';

        if (strcmp(dataType, "NAME") == 0) {
            mName.read(ifs);
        } else if (strcmp(dataType, "INTV") == 0) {
            mIndex.read(ifs);
        } else if (strcmp(dataType, "DATA") == 0) {
            mPath.read(ifs);
        } else {
            handleUnknownRecord(ifs);
        }
    }
}
