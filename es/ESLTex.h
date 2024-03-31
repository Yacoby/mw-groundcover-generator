#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <fstream>

#include "ESRecord.h"
#include "ESSubString.h"
#include "ESSubLong.h"

namespace ES3 {

    class ESLTex;

    typedef std::shared_ptr<ESLTex> ESLTexRef;

/**
* Used for holding land texture details
*/
    class ESLTex : public ESRecord {
    private:
        ///unique id
        ESSubString mName;

        ///unique index
        ESSubLong mIndex;

        ///path to the texture
        ESSubString mPath;
    public:

        /**
        *	@return the id of the ltex. I assume this is unique, but I am not sure.
        */
        std::string getID() { return mName.get(); }

        /**
        *	@return the index of the ltex. This is refered to by the landrecord. How the frek it knows between mods IDK.
        */
        uint32_t getIndex() { return mIndex.get(); }

        /**
        *	@return the path to the texture
        */
        const std::string& getPath() { return mPath.get(); }

        void read(std::ifstream &ifs, long recordSize);


    };

}