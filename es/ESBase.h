#ifndef _ESBASE_H_
#define _ESBASE_H_

#include <map>
#include <vector>
#include <memory>
#include <set>
#include <string>
#include <cassert>

#include "MapFunctions.h"
#include "ESFile.h"


namespace ES3 {

    const unsigned CELL_SIZE = 8192;
    const unsigned LTEX_GRID_SIZE = 512;


    class ESFileContainer;

    typedef std::shared_ptr<ESFileContainer> ESFileContainerRef;


/**
* A helper class. Use it to load multiple files and then access data.
* It is advisable to always use this for mutliple files.
*/
    class ESFileContainer {
    protected:

        /**
        * Contains a list of the files. This is needed as some records canot be overwriten. (Cells for example),
        * so need to be stored on a per file basis.
        */
        std::vector<ESFileRef> mFile;
    public:
        bool loadDataFile(const std::filesystem::path &file);

        ESCellRef getFirstCell(int x, int y);

        /**
        *	Retuurns the land data for the selected grid
        *	@param squX The grid square x
        *	@param squY the grid square y
        */
        ESLandRef getLand(int squX, int squY);

        ESFileRef getLandFile(int squX, int squY);

        std::optional<float> getHeightAt(float posX, float posY) {
            int cellX = (int) floor(posX / (float) CELL_SIZE);
            int cellY = (int) floor(posY / (float) CELL_SIZE);
            const auto land = getLand(cellX, cellY);
            if (!land) {
                return std::nullopt;
            }
            return land->getHeightAt(posX, posY);
        }

        std::optional<Vector3> getAngleAt(float posX, float posY) {
            int cellX = (int) floor(posX / (float) CELL_SIZE);
            int cellY = (int) floor(posY / (float) CELL_SIZE);
            const auto land = getLand(cellX, cellY);
            if (!land) {
                return std::nullopt;
            }
            return land->getAngleAt(posX, posY);
        }

        ESLTexRef getLandTexture(float posX, float posY) {
            int cellX = (int) floor(posX / (float) CELL_SIZE);
            int cellY = (int) floor(posY / (float) CELL_SIZE);

            float offx = (int) (posX) % (int) CELL_SIZE;
            int squx = floor(offx / (float) LTEX_GRID_SIZE);
            if (squx < 0) {
                squx = 16 + squx;
            }

            float offy = (int) (posY) % (int) CELL_SIZE;
            int squy = floor(offy / (float) LTEX_GRID_SIZE);
            if (squy < 0) {
                squy = 16 + squy;
            }

            const auto land = getLand(cellX, cellY);
            if (land == nullptr) {
                return nullptr;
            }

            const auto& ltex = land->getLandTextures();
            const auto ltexIdx = ltex[squx][squy];
            if (ltexIdx == 0) {
                return nullptr;
            }

            const auto file = getLandFile(cellX, cellY);
            assert(file != nullptr);
            return file->getLTex(ltexIdx - 1);
        }

        std::set<std::pair<int32_t, int32_t >> getExteriorCellCoordinates();
    };

}//namespace

#endif
