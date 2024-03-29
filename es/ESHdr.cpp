#include "ESHdr.h"

void ES3::ESHeader::read(std::ifstream &ifs, long recordSize) {
    long readTo = recordSize + ifs.tellg();
    while (ifs.tellg() < readTo) {
        char dataType[5];
        ifs.read(dataType, 4);

        //string must be null terminated.
        dataType[4] = '\0';

        if (strcmp(dataType, "HEDR") == 0) {
            /*
            4 bytes, float Version (1.2)
            4 bytes, long Unknown (1)
            32 Bytes, Company Name string
            256 Bytes, ESM file description?
            4 bytes, long NumRecords (48227)
            */

            mVersion.read(ifs);
            //ifs.seekg((long)ifs.tellg() + 4);

            uint32_t g;
            ifs.read((char *) &g, sizeof(uint32_t));

            mComp.read(ifs);
            mDisc.read(ifs);
            ifs.seekg((long) ifs.tellg() + 4);

        } else {
            handleUnknownRecord(ifs);
        }
    }
    ifs.seekg(readTo);
}