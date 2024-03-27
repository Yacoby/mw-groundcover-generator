#include "ESSubRecord.h"

namespace ES3{

/**
* Contains the data for the cell ambi
*/
struct ESSubAmbiData{
	long mAmbiCol;
	long mSunCol;
	long mFogCol;
	float mFogDen;
};

/**
*		Cell Lighting data
*		AMBI = Ambient Light Level (16 bytes)
*			long AmbientColor
*			long SunlightColor
*			long FogColor
*			float FogDensity	
*/
class ESSubAmbi : public ESSubRecord{

public:
	ESSubAmbiData* getData(){ return (ESSubAmbiData*)mData.c_str() ; }

	long getAmbientColor(){ return getData()->mAmbiCol; }

};

}