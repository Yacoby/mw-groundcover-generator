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
    const std::string& getID() const { return mName; }

    /**
    *	@return the index of the ltex. This is refered to by the landrecord. How the frek it knows between mods IDK.
    */
    uint32_t getIndex() const { return mIndex; }

    /**
    *	@return the path to the texture
    */
    const std::string& getPath() const { return mPath; }

    static ESLTex load(EspReader::Record& record);


};
