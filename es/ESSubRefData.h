#ifndef _ESSUBREFDATA_H_
#define _ESSUBREFDATA_H_

#include "ESSubRecord.h"
namespace ES3{
/**
* Contains ESRef Position Data (24 bytes), found within a cell record...
*/
struct ESRefData{

	//position data
	float mXPos;
	float mYPos;
	float mZPos;

	//rotation data
	float mXRotate;
	float mYRotate;
	float mZRotate;
};

/**
* Contains the following data relating to info on a cell record:
*			float XPos
*			float YPos
*			float ZPos
*			float XRotate
*			float YRotate
*			float ZRotate
*
*	FYI it is a DATA block
*/


class ESSubRefData : public ESSubRecord{
public:
	/**
	* Gets the data record casted to the correct type
	*/
	ESRefData* getData(){
		return (ESRefData*)mData.c_str(); 
	}

	/**
	* Returns the x position of the object
	*/
	float getXPos(){ return getData()->mXPos; }

	/**
	* Returns the y position of the object
	*/
	float getYPos(){ return getData()->mYPos; }

	/**
	* Returns the z position of the object
	*/
	float getZPos(){ return getData()->mZPos; }

	float getXRot(){ return getData()->mXRotate; }
	float getYRot(){ return getData()->mYRotate; }
	float getZRot(){ return getData()->mZRotate; }
};
}
#endif