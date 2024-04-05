#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <fstream>

#include "EspReader.h"


class ESLTex;
typedef std::shared_ptr<ESLTex> ESLTexRef;

/**
* Used for holding land texture details
*/
class ESLTex {
private:
    ///unique id
    std::string mName;

    ///unique index
    uint32_t mIndex;

    ///path to the texture
    std::string mPath;
public:

    /**
    *	@return the id of the ltex. I assume this is unique, but I am not sure.
    */
    std::string getID() { return mName; }

    /**
    *	@return the index of the ltex. This is refered to by the landrecord. How the frek it knows between mods IDK.
    */
    uint32_t getIndex() { return mIndex; }

    /**
    *	@return the path to the texture
    */
    const std::string& getPath() { return mPath; }

    static ESLTex load(EspReader::Record& record);


};
