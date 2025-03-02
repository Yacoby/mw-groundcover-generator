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
            if (tex->getIndex() != mLandTexVec.size()) {
                throw std::runtime_error(fmt::format(
                        "Non contiguous index in LTEX record. Expected LTEX index to be {} but was {}",
                        mLandTexVec.size(), tex->getIndex()
                ));
            }
            mLandTexVec.push_back(tex);
        }
    }
}

GridId::GridId(const int x, const int y) : x(x), y(y) {}

GridId GridId::fromPosition(const double x, const double y) {
    return GridId(
            static_cast<int>(floor(x / 8192)),
            static_cast<int>(floor(y / 8192))
    );
}

bool GridId::operator==(const GridId& rhs) const {
    return std::tie(x, y) == std::tie(rhs.x, rhs.y);
}

bool GridId::operator!=(const GridId& rhs) const {
    return !(rhs == *this);
}

bool GridId::operator<(const GridId& rhs) const {
    return std::tie(x, y) < std::tie(rhs.x, rhs.y);
}

std::ostream& operator<<(std::ostream& os, const GridId& id) {
    os << "( " << id.x << ", " << id.y << ")";
    return os;
}