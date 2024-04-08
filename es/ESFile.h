#ifndef _ESDATA_H_
#define _ESDATA_H_


#include <map>
#include <list>
#include <set>
#include <cstring>
#include <vector>
#include <filesystem>

#include "ESCell.h"
#include "ESLand.h"
#include "ESLTex.h"

class GridId {
public:
    const int x;
    const int y;

    GridId(const int x, const int y) : x(x), y(y) {}

    bool operator==(const GridId &rhs) const {
        return std::tie(x, y) == std::tie(rhs.x, rhs.y);
    }

    bool operator!=(const GridId &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const GridId &rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }
};

class ESFile;
typedef std::shared_ptr<ESFile> ESFileRef;

/**
* Holds the data for a single Morrowind file.
*/
class ESFile {
private:
    std::filesystem::path filePath;
    /**
    * A list of exterior cells, listed by grid
    */
    std::map<GridId, ESCellRef> mpExteriorCell;

    std::map<GridId, ESLandRef> mpLand;

    std::map<long, ESLTexRef> mLandTex;
    std::vector<ESLTexRef> mLandTexVec;
public:

    const std::filesystem::path& getFilePath() {
        return filePath;
    }

    void loadFile(const std::filesystem::path &pFile);

    ESLTexRef getLTex(long id) {
        return mLandTexVec.at(id);
    }

    ESCellRef getCell(int squX, int squY) {
        ESCellRef ref = mpExteriorCell[GridId(squX, squY)];
        return ref;
    }

    ESLandRef getLand(int squX, int squY) {
        ESLandRef ref = mpLand[GridId(squX, squY)];
        return ref;
    }

    bool landExists(const int squX, const int squY) {
        return mpLand.find(GridId(squX, squY)) != mpLand.end();
    }

    bool getLTexExists(int index) {
        return mLandTex.find(index) != mLandTex.end();
    }

    const std::string& getLTexPath(int index) {
        return mLandTex[index]->getPath();
    }

    std::set<std::pair<int32_t, int32_t >> getExteriorCellCoordinates() {
        std::set<std::pair<int32_t, int32_t> > result;
        for (const auto &xCordAndCells: mpLand) {
            result.insert(std::pair(xCordAndCells.first.x, xCordAndCells.first.y));
        }
        return result;
    }
};

#endif
