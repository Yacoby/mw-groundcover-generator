#include <cstring>
#include "ESCell.h"

using namespace ES3;

void ESCell::read(std::ifstream &ifs, long recordSize) {
    std::streampos readTo = recordSize + ifs.tellg();

    while (ifs.tellg() < readTo) {
        char dataType[5];
        ifs.read(dataType, 4);
        dataType[4] = '\0';

        //we have got to the frmrecord part. quit.
        if (strcmp(dataType, "FRMR") == 0) {
            return;
        }

        if (strcmp(dataType, "NAME") == 0) {
            mCellName.read(ifs);
        } else if (strcmp(dataType, "DATA") == 0) {
            mCellData.read(ifs);
        } else if (strcmp(dataType, "RGNN") == 0) {
            mRegn.read(ifs);
        } else if (strcmp(dataType, "NAM0") == 0) {
            mColour.read(ifs);
        } else {
            handleUnknownRecord(ifs);
        }

    }

    ifs.seekg(readTo);

}
