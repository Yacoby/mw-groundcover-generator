#ifndef _ESBASE_H_
#define _ESBASE_H_

#include <map>
#include <vector>
#include <memory>
#include <set>
#include <string>

#include "MapFunctions.h"
#include "ESFile.h"


namespace ES3 {


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

        std::set<std::pair<int32_t, int32_t >> getExteriorCellCoordinates();
    };

}//namespace

#endif
