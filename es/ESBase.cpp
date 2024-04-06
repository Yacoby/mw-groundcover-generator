#include "ESBase.h"

#include <ranges>

void ESFileContainer::loadDataFile(const std::filesystem::path &file) {
    auto esFile = ESFileRef(new ESFile());
    esFile->loadFile(file);
    mFile.push_back(esFile);
}

ESCellRef ESFileContainer::getFirstCell(int x, int y) {
    for (unsigned i = 0; i < mFile.size(); i++) {
        if (mFile[i]->cellExists(x, y)) {
            return mFile[i]->getCell(x, y);
        }
    }
    return NULL;
}

ESFileContainer::CellInformation ESFileContainer::getCellInformation(int x, int y) {
    CellInformation info;
    for (const auto& it : std::ranges::reverse_view(mFile)) {
        const auto cell = it->getCell(x, y);
        if (!cell) {
            continue;
        }

        if (!info.name.has_value() && cell->getCellName().has_value()) {
            info.name = cell->getCellName();
        }
        if (!info.region.has_value() && cell->getRegn().has_value()) {
            info.region = cell->getRegn();
        }
        if (info.name.has_value() && info.region.has_value()) {
            break;
        }
    }

    return info;
}

ESLandRef ESFileContainer::getLand(int squX, int squY) {
    for (unsigned i = 0; i < mFile.size(); i++) {
        if (mFile[i]->landExists(squX, squY)) {
            return mFile[i]->getLand(squX, squY);
        }
    }
    return NULL;
}

ESFileRef ESFileContainer::getLandFile(int squX, int squY) {
    for (unsigned i = 0; i < mFile.size(); i++) {
        if (mFile[i]->landExists(squX, squY)) {
            return mFile[i];
        }
    }
    return NULL;
}

std::set<std::pair<int32_t, int32_t >> ESFileContainer::getExteriorCellCoordinates() {
    std::set<std::pair<int32_t, int32_t> > result;
    for (unsigned i = 0; i < mFile.size(); i++) {
        const auto& coordinates = mFile[i]->getExteriorCellCoordinates();
        result.insert(coordinates.begin(), coordinates.end());
    }
    return result;
}
