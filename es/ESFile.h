#ifndef _ESDATA_H_
#define _ESDATA_H_


#include <map>
#include <list>
#include <set>
#include <cstring>
#include <vector>

#include "MapFunctions.h"

#include "ESRecord.h"

#include "ESHdr.h"

#include "ESCell.h"
#include "ESLand.h"
#include "ESLTex.h"


namespace ES3 {

    class ESFile;

    typedef std::shared_ptr<ESFile> ESFileRef;

/**
* Holds the data for a single Morrowind file.
*/
    class ESFile {
    private:
    protected:
        /**
        * A list of exterior cells, listed by grid
        */
        STD_MAP2d(int, ESCellRef) mpExteriorCell;

        STD_MAP2d(int, ESLandRef) mpLand;

        std::map<long, ESLTexRef> mLandTex;
        std::vector<ESLTexRef> mLandTexVec;

        ESHeaderRef mHeader;
    public:

        void readHeader(std::ifstream *mIfs);

        bool loadFile(const std::string &pFile);

        ESLTexRef getLTex(long id) {
            return mLandTexVec.at(id);
        }

        ESCellRef getCell(int squX, int squY) {
            ESCellRef ref = mpExteriorCell[squX][squY];
            return ref;
        }

        ESLandRef getLand(int squX, int squY) {
            ESLandRef ref = mpLand[squX][squY];
            return ref;
        }

        bool cellExists(const int squX, const int squY) {
            if (map2dDoesExist<int, ESCellRef>(squX, squY, mpExteriorCell)) {
                return true;
            }
            return false;
        }

        bool landExists(const int squX, const int squY) {
            if (map2dDoesExist<int, ESLandRef>(squX, squY, mpLand)) {
                return true;
            }
            return false;
        }

        bool getLTexExists(int index) {
            if (mLandTex.find(index) == mLandTex.end()) {
                return false;
            }
            return true;
        }

        const std::string& getLTexPath(int index) {
            return mLandTex[index]->getPath();
        }

        std::set<std::pair<int32_t, int32_t >> getExteriorCellCoordinates() {
            std::set<std::pair<int32_t, int32_t> > result;
            for (const auto &xCordAndCells: mpLand) {
                for (const auto &yCordAndCell: xCordAndCells.second) {
                    result.insert(std::pair(xCordAndCells.first, yCordAndCell.first));
                }
            }
            return result;
        }
    };

}//namespace
#endif
