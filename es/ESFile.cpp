#include "ESFile.h"

#include <cstring>

using namespace ES3;

void ESFile::readHeader(std::ifstream *mIfs) {
    char sig[4]; //TES3
    mIfs->read((char *) &sig, 4);

    ////header size
    uint32_t hdrSize;
    mIfs->read((char *) &hdrSize, sizeof(uint32_t));

    mIfs->seekg((long) mIfs->tellg() + 8);

    ////skip the hdr
    mIfs->seekg(hdrSize + 16);
}

bool ESFile::loadFile(const std::string &pFile) {
    std::ifstream ifStream = std::ifstream(pFile.c_str(), std::ios::in | std::ios::binary);
    if (!ifStream.is_open()) {
        throw std::runtime_error("Cannot open file: " + pFile);
    }
    readHeader(&ifStream);

    while (ifStream.tellg() != -1) {
        //get the record type
        char tType[5];
        ifStream.get(tType, 5);

        if (strcmp(tType, "DDCE") == 0) {
            ifStream.seekg((long) ifStream.tellg() - 3);
            continue;
        }

        //record block size
        uint32_t recordSize;
        ifStream.read((char *) &recordSize, sizeof(uint32_t));

        //hdr1
        uint32_t hdr1;
        ifStream.read((char *) &hdr1, sizeof(uint32_t));

        //flags
        uint32_t flags;
        ifStream.read((char *) &flags, sizeof(uint32_t));

        std::streampos recordEndPos = recordSize + ifStream.tellg();

        if (strcmp(tType, "LAND") == 0) {
            ESLandRef land = ESLandRef(new ESLand());
            land->read(ifStream, recordSize);

            mpLand[land->getLandPos().cellX][land->getLandPos().cellY] = land;
        } else if (strcmp(tType, "CELL") == 0) {
            ESCellRef cell = ESCellRef(new ESCell());
            cell->read(ifStream, recordSize);

            if (cell->getCellData()->isInterior() == false) {
                mpExteriorCell[cell->getCellData()->gridX()][cell->getCellData()->gridY()] = cell;
            }
        } else if (strcmp(tType, "LTEX") == 0) {
            ESLTexRef tex = ESLTexRef(new ESLTex());
            tex->read(ifStream, recordSize);
            mLandTex[tex->getIndex()] = tex;
            mLandTexVec.push_back(tex);
        }

        // Advance to read the next record (as in some cases we don't read anything)
        ifStream.seekg(recordEndPos);

    } //while

    return true;
}