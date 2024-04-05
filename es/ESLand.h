#ifndef _ESLAND_H_
#define _ESLAND_H_

#include <cmath>
#include <memory>
#include <vector>
#include <fstream>

#include <boost/multi_array.hpp>

#include "EspReader.h"

#define PI 3.14159265


/**
* Used for holding the position in the cell grid of a land record
*/
struct LandSquare {
    uint32_t cellX;
    uint32_t cellY;
};


/**
*	Simple vector3 implementation. This removes the depandance on any other library that implements this (Ogre, Niflib)
*/
struct Vector3 {
    float x;
    float y;
    float z;
};

class ESLand;

typedef std::shared_ptr<ESLand> ESLandRef;

/**
*	Holds a LAND record.
*/

class ESLand {


    boost::multi_array<uint16_t, 2> mLandTextures;
    /**
    * height data
    * The height data is not absolute values but uses differences between adjacent pixels.
    * Thus a pixel value of 0 means it has the same height as the last pixel. Note that
    * the y-direction of the data is from the bottom up.
    */
    boost::multi_array<int, 2> mHeightData;

    /**
    * The data on where in the world the cell is
    */
    LandSquare mLandSquare;


    static void loadVtexRecord(boost::multi_array<uint16_t, 2>& textures, EspReader::SubRecord& record);
    static void loadVhgtRecord(boost::multi_array<int, 2>& heights, EspReader::SubRecord& record);
public:

    ESLand();

    const boost::multi_array<int, 2>& getHeightData() { return mHeightData; }
    const boost::multi_array<uint16_t, 2>& getLandTextures() { return mLandTextures; }


    /**
    *	@return Details about what swuare the land is on
    */
    const LandSquare& getLandPos() { return mLandSquare; }

    static ESLand load(EspReader::Record& record);
};

#endif