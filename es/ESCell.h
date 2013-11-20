/*_LICENCE_BLOCK_
------------------------------------------------------------------
This source file is part of Morrowind Remake

Copyright (c) 2007 Jacob Essex
Also see acknowledgements in the readme

Morrowind Remake is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Morrowind Remake is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/gpl.txt

------------------------------------------------------------------
_LICENCE_BLOCK_*/
#ifndef _ESCELL_H_
#define _ESCELL_H_

#include "ESRecord.h"
#include "ESRecordSectionLoader.h"

#include "ESSubString.h"
#include "ESSubFloat.h"
#include "ESSubCellData.h"
#include "ESSubCellFRMR.h"
#include "ESSubAmbi.h"

namespace ES3{


class ESCellListener{
public:
	virtual void FRMRAdded(ESSubCellFRMRRef ref) = 0;
};


class ESCell;
typedef ESRef<ESCell> ESCellRef;

/**
* Contains all data releating to a cell...
*/
class ESCell : public ESRecordSectionLoader{
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

	std::map<unsigned, ESSubCellFRMRRef>::iterator refIter;   

	/**
	* Data on every ref in the cell
	* \todo Change to map?
	*/
	std::map<unsigned, ESSubCellFRMRRef> mCellESRef;
	//std::vector<ESSubCellFRMR*> mCellESRef;

	/**
	* the mutex lock for all the data.
	* \todo Split this up into segments
	*/
//	boost::mutex mMutexAllData;

	/**
	* Stores the offset of the point in the file where the frmrecords start
	*/
	//long mFRMRStartOffset;

	//long mFRMREndOffset;




	/**
	* Loads the cell frm records.
	*/
/*
	bool doThreadLoad(){


		if ( mDataStart == -1 ) //obviously no FRMRs found.
			return false;

		boost::mutex::scoped_lock scoped_lock(mMutexAllData);

		//go to the place in the file
		std::ifstream ifs(mFileName.c_str(), std::ios::in | std::ios::binary);

		ifs.seekg(mDataStart);

		ESSubCellFRMRRef newCellESRef = NULL;
		
		while ( ifs.tellg() < mDataEnd ){

			char dataType[5];
			ifs.read(dataType, 4);
			
			//string must be null terminated.
			dataType[4] = '\0';

			if ( strcmp(dataType, "FRMR") == 0){

				if ( newCellESRef != NULL ){
					mCellESRef[newCellESRef->getFRMR()] = newCellESRef;
				}

				newCellESRef = new ESSubCellFRMR;
			}

			if ( strcmp(dataType, "FRMR") == 0)
				newCellESRef->mFRMR.read(ifs);

			else if ( strcmp(dataType, "NAME") == 0)
				newCellESRef->mName.read(ifs);

				else if ( strcmp(dataType, "XSCL") == 0)
					newCellESRef->mScale.read(ifs);

				else if ( strcmp(dataType, "DELE") == 0)
					newCellESRef->mDel.read(ifs);

				else if ( strcmp(dataType, "DATA") == 0)
					newCellESRef->mPos.read(ifs);
				else
					handelUnknownRecord(ifs);
		}

		//add the last cell record...
		if ( newCellESRef != NULL ){
			mCellESRef[newCellESRef->getFRMR()] = newCellESRef;
		}


		ifs.close();


		return true;
	}
	*/


	ESCellListener* mClass;



public:


	ESCell(){
		mClass = NULL;
	}

	~ESCell(){
		//boost::mutex::scoped_lock scoped_lock(mMutexAllData);
		//for ( unsigned i = 0; i < mCellESRef.size(); i++ ){
		//	delete mCellESRef[i];
		//}
	}

	/**
	* Use this to define a function to be called when a FRMR is read
	* I dunno. May be usefull.
	*/
	void setFRMRLoadListener(ESCellListener* c){
		mClass = c;
		//mFRMRListener = fp;
		//mClass = c;
	}

	std::string getRegn(){ return mRegn.get();}
	long getNAM0(){return mColour.get();}


	bool loadSection(float amount){

		if ( !mIsSectionLoading )
			loadSectionStart();

#ifdef _DEBUG
		if ( amount < 0 ) //this should NEVER happen
			return false;
#endif
		
		if ( mDataStart == -1 ) //obviously no FRMRs found.
			return loadSectionEnd(); //loaded as much as we can

		if ( mDataReadTo == mDataEnd )//loaded everything
			return loadSectionEnd();


		long amountToLoad = (long) (( mDataEnd - mDataStart ) * (float)( amount / 100 ));
		if ( amountToLoad < 1 )
			amountToLoad = 1;

		long readTo = mDataReadTo + amountToLoad;
		if ( readTo > mDataEnd )
			readTo = mDataEnd;

		if ( amount >= 100 )
			readTo = mDataEnd;

		mAmountLoaded += amount;


		//--------------------------------
				//Loading Section
		//--------------------------------
		
		ESSubCellFRMRRef newCellESRef = NULL;
		
		while ( mDataStream->tellg() < mDataEnd ){

			char dataType[5];
			mDataStream->read(dataType, 4);
			
			//string must be null terminated.
			dataType[4] = '\0';

			mDataReadTo = mDataStream->tellg();

			if ( strcmp(dataType, "FRMR") == 0){

				if ( mDataStream->tellg() > readTo ){
					mDataStream->seekg((long)mDataStream->tellg() - 4);
					break;
				}


				if ( newCellESRef != NULL ){

					if ( mClass )
						//(*mFRMRListener)(newCellESRef);
						//mFRMRListener->call(newCellESRef);
						mClass->FRMRAdded(newCellESRef);

					mCellESRef[newCellESRef->getFRMR()] = newCellESRef;
				}
				newCellESRef = new ESSubCellFRMR;
			}

			assert(newCellESRef);

			if ( strcmp(dataType, "FRMR") == 0)
				newCellESRef->mFRMR.read(*mDataStream);

			else if ( strcmp(dataType, "NAME") == 0)
				newCellESRef->mName.read(*mDataStream);

			else if ( strcmp(dataType, "XSCL") == 0)
				newCellESRef->mScale.read(*mDataStream);

			else if ( strcmp(dataType, "DELE") == 0)
				newCellESRef->mDel.read(*mDataStream);

			else if ( strcmp(dataType, "DATA") == 0)
				newCellESRef->mPos.read(*mDataStream);
			else
				handelUnknownRecord(*mDataStream);
		}

		//add the last cell record...
		if ( newCellESRef != NULL ){

			if ( mClass )
				mClass->FRMRAdded(newCellESRef);
				//mFRMRListener->call(newCellESRef);
				//(*mFRMRListener)(newCellESRef);

			mCellESRef[newCellESRef->getFRMR()] = newCellESRef;
		}

		mDataReadTo = mDataStream->tellg();

		if ( mDataReadTo == mDataEnd )
			return loadSectionEnd(); // aka reutrn true

		return false;

	}

	void resetESRefItr(){
		refIter = mCellESRef.begin();
	}

	bool refItrGood(){
		if ( refIter == mCellESRef.end() )
			return false;
		if ( mCellESRef.begin() == mCellESRef.end() )
			return false;
		return true;
	}

	ESSubCellFRMRRef getNextESRef(){

		if ( refIter == mCellESRef.end() )
			return NULL;

		ESSubCellFRMRRef ref = refIter->second;

		refIter++;

		return ref;
	}


	/**
	* @return the number of refrances in the cell
	*/
	//unsigned getESRefBegin(){
	//	boost::mutex::scoped_lock scoped_lock(mMutexAllData);
	//	return (unsigned) mCellESRef.begin();
	//}

	//unsigned getESRefEnd(){
	//}

	/**
	* @param index the FRMR of the record
	* @return a ref with the FRMR of index
	* \todo Make it return a readonly version of a cell ref? Not thread safe.
	*/
	/*
	ESSubCellFRMR* getESRef(unsigned index){
		boost::mutex::scoped_lock scoped_lock(mMutexAllData);

		return mCellESRef.at(index);
	}
	*/

	ESSubCellFRMR* getESRef(unsigned index){
//		boost::mutex::scoped_lock scoped_lock(mMutexAllData);
		return mCellESRef[index];
	}

	/**
	* Adds a ref to the cell. Any ref with the same FRMR is overwriten
	* @param ref the ref to add
	*/
	void addESRef(ESSubCellFRMR* ref){

		if ( ref == NULL )
			return;

		//if ( (long)mCellESRef.size() < ref->getFRMR() + 1 ){
		//	mCellESRef.resize(ref->getFRMR() + 1, NULL);
		//}

		if ( mCellESRef.find(ref->getFRMR()) != mCellESRef.end() )
			delete mCellESRef[ref->getFRMR()];

		mCellESRef[ref->getFRMR()] = ref;
	}


	/**
	* @return data relating to the cell. This includes grid data, interior, exterior etc
	*/
	ESSubCellData* getCellData(){ 
		//boost::mutex::scoped_lock scoped_lock(mMutexAllData);
		return &mCellData; 
	}

	void setCellData(ESSubCellData &cellData){
		//boost::mutex::scoped_lock scoped_lock(mMutexAllData);
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
		//boost::mutex::scoped_lock scoped_lock(mMutexAllData);
		return mCellName.get(); 
	}

	/**
	* Reads all the cell data excluding the FRMR records
	*/
	void read(std::ifstream &ifs, long recordSize);
};

}//namepsace

#endif
