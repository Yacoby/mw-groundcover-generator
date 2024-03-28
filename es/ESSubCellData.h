
#pragma once

#include "ESSubRecord.h"

namespace ES3 {

/**
* Holds a load of data about cells.
*
*	DATA = Cell Data
*		long Flags
*			0x01 = Interior?
*			0x02 = Has Water
*			0x04 = Illegal to Sleep here
*			0x80 = Behave like exterior (Tribunal)
*		long GridX
*		long GridY
*/
    class ESSubCellData : public ESSubRecord {
    protected:
        uint32_t mFlags;
        int32_t mGridX;
        int32_t mGridY;
    public:

        inline long getFlags() { return mFlags; }

        ///Returns true if the cell is an exterior
        inline bool isInterior() { return ((mFlags & 0x01) != 0); }

        inline bool hasWater() { return ((mFlags & 0x02) != 0); }

        inline bool isIllegalSleep() { return ((mFlags & 0x04) != 0); }

        inline bool actLikeExterior() { return ((mFlags & 0x80) != 0); }

        ///returns the gridx of the cell
        inline int32_t gridX() { return mGridX; }

        inline void setGridX(int32_t c) { mGridX = c; }

        ///returns the gridy of the cell
        inline int32_t gridY() { return mGridY; }

        inline void setGridY(int32_t c) { mGridY = c; }


        void read(std::ifstream &ifs) {
            //we know the size...
            ifs.seekg((long) ifs.tellg() + 4);

            ifs.read((char *) &mFlags, 4);
            ifs.read((char *) &mGridX, 4);
            ifs.read((char *) &mGridY, 4);
        }
    };

}