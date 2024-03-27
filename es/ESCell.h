#ifndef _ESCELL_H_
#define _ESCELL_H_

#include "ESRecord.h"

#include "ESSubString.h"
#include "ESSubFloat.h"
#include "ESSubLong.h"
#include "ESSubCellData.h"
#include "ESSubAmbi.h"

namespace ES3{


class ESCell;
typedef ESRef<ESCell> ESCellRef;

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

	///Water height
	ESSubFloat mWaterHeight;

	///Light data
	ESSubAmbi mAmbiData;
public:


	ESCell(){
	}

	~ESCell(){
	}


	std::string getRegn(){ return mRegn.get();}
	long getNAM0(){return mColour.get();}

	/**
	* @return data relating to the cell. This includes grid data, interior, exterior etc
	*/
	ESSubCellData* getCellData(){ 
		return &mCellData;
	}

	void setCellData(ESSubCellData &cellData){
		mCellData = cellData	;
	}

	/**
	* @return the water height of the cell
	*/
	float getWaterHeight(){ return mWaterHeight.get(); }

	/**
	* @param f the float to set the water heigth to
	*/
	void setWaterHeight(float f){ mWaterHeight.set(f); }

	/**
	*	@return the lighting (ambinat) data for the current cell
	*/
	ESSubAmbi* getAmbiData(){ return &mAmbiData; }

	/**
	* @param &a the ambi data to set for the current cell
	*/
	void setAmbiData(ESSubAmbi &a){ mAmbiData = a; }

	/**
	* Returns the name of the cell. If the cell is an exterior, it doesn't have a name (0 length string is returned
	*/
	std::string getCellName(){
		return mCellName.get();
	}

	/**
	* Reads all the cell data excluding the FRMR records
	*/
	void read(std::ifstream &ifs, long recordSize);
};

}//namepsace

#endif
