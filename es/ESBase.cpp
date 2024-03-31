#include "ESBase.h"

using namespace ES3;

bool ESFileContainer::loadDataFile(const std::filesystem::path &file) {
    static std::map<std::filesystem::path, ESFileRef> files;

    if (files.find(file) == files.end()) {
        files[file] = ESFileRef(new ESFile());
    }

    {
        if (files[file]->loadFile(file)) {
            mFile.push_back(files[file]);
            files.erase(file);

            return true;
        }

        return false;

    }
    return true;
}

ESCellRef ESFileContainer::getFirstCell(int x, int y) {
    for (unsigned i = 0; i < mFile.size(); i++) {
        if (mFile[i]->cellExists(x, y)) {
            return mFile[i]->getCell(x, y);
        }
    }
    return NULL;
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
