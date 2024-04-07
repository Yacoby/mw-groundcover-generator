#ifndef _ESBASE_H_
#define _ESBASE_H_

#include <map>
#include <vector>
#include <memory>
#include <set>
#include <string>
#include <cassert>

#include "ESFile.h"


const unsigned CELL_SIZE = 8192;
const unsigned LTEX_GRID_SIZE = 512;
const int DEFAULT_LAND_HEIGHT = -2048;

class ESFileContainer {
private:

    /**
    * Contains a list of the files. This is needed as some records canot be overwriten. (Cells for example),
    * so need to be stored on a per file basis.
    */
    std::vector<ESFileRef> mFile;

    struct LandCache {
        int squX;
        int squY;
        ESLandRef land;
    };
    std::optional<LandCache> cache;

public:
    struct CellInformation {
        std::optional<std::string> name;
        std::optional<std::string> region;
    };

    ESLandRef getLand(int squX, int squY);
    ESFileRef getLandFile(int squX, int squY);

    void loadDataFile(const std::filesystem::path &file);

    CellInformation getCellInformation(int x, int y);

    ESLTexRef getLandTexture(float posX, float posY);

    std::set<std::pair<int32_t, int32_t >> getExteriorCellCoordinates();

    int getHeightAtVertex(int vertexX, int vertexY);

    float getHeightAt(float positionX, float positionY);

    Vector3 getAngleAt(float x, float y);
};

#endif
