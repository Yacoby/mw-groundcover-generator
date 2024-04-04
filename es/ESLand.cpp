#include "ESLand.h"

#include <cassert>
#include <cstring>

using namespace ES3;

ESLand::ESLand() : mHeightData(boost::multi_array<int, 2>(boost::extents[65][65])),
                   mLandTextures(boost::multi_array<uint16_t, 2>(boost::extents[16][16])) {
}

void ESLand::loadVtexRecord(std::ifstream &ifs) {
    std::fill(mLandTextures.data(), mLandTextures.data() + mLandTextures.num_elements(), 0);

    for (int y1 = 0; y1 < 4; y1++) {
        for (int x1 = 0; x1 < 4; x1++) {
            for (int y2 = 0; y2 < 4; y2++) {
                for (int x2 = 0; x2 < 4; x2++) {
                    ifs.read((char *) &mLandTextures[x1 * 4 + x2][y1 * 4 + y2], sizeof(uint16_t));
                }
            }
        }
    }
}

void ESLand::loadVhgtRecord(std::ifstream &ifs) {
    float offset;
    ifs.read((char *) &offset, sizeof(float));

    std::fill(mHeightData.data(), mHeightData.data() + mHeightData.num_elements(), 0);

    for (int y = 0; y < 65; y++) {
        char x;
        ifs.get(x);
        offset += x;
        mHeightData[0][y] = +(int) offset;

        float pos = offset;

        for (int x = 1; x < 65; x++) {

            char c;
            ifs.get(c);
            pos += c;

            mHeightData[x][y] = (int) pos;

        }
    }

    char unk;
    ifs.read((char *) &unk, 1);


    uint16_t junk2 = 0;
    ifs.read((char *) &junk2, sizeof(uint16_t));
}

void ESLand::read(std::ifstream &ifs, long recordSize) {
    std::streampos readTo = recordSize + ifs.tellg();

    while (ifs.tellg() < readTo) {
        char dataType[5];
        ifs.read(dataType, 4);

        //string must be null terminated.
        dataType[4] = '\0';
        uint32_t subRecSize;
        if (strcmp(dataType, "INTV") == 0) {
            ifs.read((char *) &subRecSize, sizeof(uint32_t));
            std::streampos recordStart = ifs.tellg();
            assert(sizeof(LandSquare) == 8);
            ifs.read((char *) &mLandSquare, sizeof(LandSquare));
            assert(ifs.tellg() == recordStart + std::streampos(subRecSize));
        } else if (strcmp(dataType, "VHGT") == 0) {
            ifs.read((char *) &subRecSize, sizeof(uint32_t));
            std::streampos recordStart = ifs.tellg();
            loadVhgtRecord(ifs);
            assert(ifs.tellg() == recordStart + std::streampos(subRecSize));
        } else if (strcmp(dataType, "VTEX") == 0) {
            ifs.read((char *) &subRecSize, sizeof(uint32_t));
            std::streampos recordStart = ifs.tellg();
            loadVtexRecord(ifs);
            assert(ifs.tellg() == recordStart + std::streampos(subRecSize));
        } else {
            handleUnknownRecord(ifs);
        }
    }
    ifs.seekg(readTo);
}
