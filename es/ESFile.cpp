#include "ESFile.h"

#include "EspReader.h"

void ESFile::loadFile(const std::filesystem::path &pFile) {
    assert(filePath.empty()); // undefined behaviour to call loadFile twice
    filePath = pFile;

    for (auto record: EspReader(pFile)) {
        if (record.type == "LAND") {
            ESLandRef land = ESLandRef(new ESLand(ESLand::load(record)));
            mpLand[GridId(land->getLandPos().cellX, land->getLandPos().cellY)] = land;
        } else if (record.type == "CELL") {
            ESCellRef cell = ESCellRef(new ESCell(ESCell::load(record)));
            if (!cell->isInterior()) {
                mpExteriorCell[GridId(cell->getCellX(), cell->getCellY())] = cell;
            }
        } else if (record.type == "LTEX") {
            ESLTexRef tex = ESLTexRef(new ESLTex(ESLTex::load(record)));
            mLandTex[tex->getIndex()] = tex;
            assert(tex->getIndex() == mLandTexVec.size());
            mLandTexVec.push_back(tex);
        }
    }
}