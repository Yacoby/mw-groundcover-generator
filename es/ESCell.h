#ifndef _ESCELL_H_
#define _ESCELL_H_

#include <memory>
#include <fstream>
#include <string>

#include "ESRecord.h"

#include "ESSubString.h"
#include "ESSubFloat.h"
#include "ESSubLong.h"
#include "ESSubCellData.h"

namespace ES3 {


    class ESCell;

    typedef std::shared_ptr<ESCell> ESCellRef;

/**
* Contains all data releating to a cell...
*/
    class ESCell : public ESRecord {
    private:

        ESSubLong mColour;

        ESSubString mCellName;

        ///Contains data about the cell, e.g. region, gridx, gridy ...
        ESSubCellData mCellData;

        ///Region
        ESSubString mRegn;
    public:

        const std::string& getRegn() { return mRegn.get(); }

        long getNAM0() { return mColour.get(); }

        /**
        * @return data relating to the cell. This includes grid data, interior, exterior etc
        */
        ESSubCellData *getCellData() {
            return &mCellData;
        }

        /**
        * Returns the name of the cell. If the cell is an exterior, it doesn't have a name (0 length string is returned
        */
        const std::string& getCellName() {
            return mCellName.get();
        }

        /**
        * Reads all the cell data excluding the FRMR records
        */
        void read(std::ifstream &ifs, long recordSize);
    };

}//namepsace

#endif
