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
#ifndef _ESDATA_H_
#define _ESDATA_H_


#include <map>
#include <list>
#include <cstring>
#include "MapFunctions.h"

#include <vector>
//#include "../SignedVector/SignedVector.h"

#include "ESConfig.h"

//Records
#include "ESRef.h"


#include "ESRecord.h"
#include "ESRecordFactory.h"

#include "ESHdr.h"
#include "ESDCEL.h"
#include "ESOBJS.h"

#include "ESStandardObject.h"
#include "ESActivator.h"
#include "ESStatic.h"
#include "ESCell.h"
#include "ESLand.h"
#include "ESLTex.h"
#include "ESPathGrid.h"
#include "ESDoor.h"


#include "ESSetup.h"


namespace ES3{

	class ESFile;
	typedef ESRef<ESFile> ESFileRef;

/**
* Holds the data for a single Morrowind file.
*/
class ESFile{
private:
	mutable unsigned int _ref_count;
protected:
	/**
	* contains pointers to all the records.
	*/
	std::map< std::string, ESRecordRef > mRecords;


	/**
	* A list of exterior cells, listed by grid
	*/
	STD_MAP2d(int, ESCellRef) mpExteriorCell;

	/**
	* List of intior cells, listed by cell name
	*/
	std::map<std::string, ESCellRef> mpInteriorCell;

	STD_MAP2d(int, ESPathGridRef) mpPathGrid;

	STD_MAP2d(int, ESLandRef) mpLand;

	/**
	* Holds the mod index. This is used to controll what objectIndex applyies to what mod
	*/
	unsigned mModIndex;

	///The file type of the mod. ESM (1), ESP (2), ESS (3)
	int mFileType;

	std::map<long, ESLTexRef> mLandTex;
	std::vector<ESLTexRef> mLandTexVec;


	///The ifs for this file
	std::ifstream* mIfs;


	long mFileSize;
	long mReadTo;
	bool mIsReading;

	ESHeaderRef mHeader;

	//std::list<ESDistantCellRef> mDistantCells;
	STD_MAP2d(int, ESDistantCellRef) mDistantCells;
	std::map<std::string, ESObjectSizeRef> mMeshSizes;

	///holds a list of meshes, containg a mesh size for each
//	std::map<std::string, ESObjectSizeRef> mMeshSizes;

public:

	inline STD_MAP2d(int, ESDistantCellRef)& getDistantCells(){ return mDistantCells; }
	inline void addDistantCell(int x, int y, ESDistantCellRef ref){
		mDistantCells[x][y] = ref;
	}
	inline ESDistantCellRef getDistantCell(int x, int y){
		if ( !map2dDoesExist<int, ESDistantCellRef>(x, y, mDistantCells) ) return NULL;
		return mDistantCells[x][y];
	}

	inline void addMeshSize(std::string& mesh, float size){
		assert(size >= 0 && "Mesh size must be greater than 0");
		ESObjectSizeRef obj = new ESObjectSize(mesh, size);
		mMeshSizes[mesh] = obj;
	}
	inline float getMeshSize(const std::string& mesh){
		if ( mMeshSizes.find(mesh) == mMeshSizes.end() ) return 0;
		return mMeshSizes[mesh]->getSize();
	}


	void ref_addESRef() const{
		++_ref_count;
	}

	 void ref_subtractESRef() const{
		_ref_count--;
		if ( _ref_count < 1 ) {
			delete this;
		}
	}


	 /**
	 * @brief Saves the file
	 *
	 * This ends up with an invalid esp file that everything except this program should fail to open.
	 * It alos only writes land data.
	 */
	 void saveFile(const std::string& path){
		 std::ofstream ofs(path.c_str(), std::ios::out | std::ios::binary );
		 long size = 308;
		 ofs.write("TES3", 4);
		 ofs.write((char*)&size, 4);
		 size = 0;
		 for ( int i = 0; i < (308 + 8); i++ ) //the +8 is the other bits of header
			ofs.write((char*)&size, 1);
		 
		 //for ( STD_MAP2d(int, ESDistantCellRef)::iterator iter1 = mDistantCells.begin(); iter1 != mDistantCells.end(); ++iter1 ){
			// for ( std::map<int, ESDistantCellRef>::iterator iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2  ){
			//	 iter2->second->write(ofs);
			// }
		 //}

		 for ( std::map<std::string, ESObjectSizeRef>::iterator iter = mMeshSizes.begin(); iter != mMeshSizes.end(); iter++ ){
			 iter->second->write(ofs);
		 }

		 ofs.close();


	 }

	bool loadEnd();

	void loadStartup(const std::string& pFile);



	bool loadFile(const std::string& pFile, float amount);


	ESFile();
	~ESFile();

	unsigned getModIndex(){return mModIndex;}
	void setModIndex(unsigned m){mModIndex = m;}

	/**
	* Returns all the records from this file.
	*/
	std::map<std::string, ESRecordRef>* getRecords(){
		return &mRecords;
	}

	/**
	* Adds a srecord that has an id into perminant storage
	* @param record The record to add
	*/
	void addStandardRecord(ESRecordRef record){
		ESStandardObjectRef idESRef = DynamicCast<ESStandardObject>(record);
		mRecords[idESRef->getID()] = record;
	}

	ESLTexRef getLTex(long id){

		return mLandTexVec.at(id);

		if ( mLandTex.find(id) == mLandTex.end() )
			return NULL;



		return mLandTex[id];
	}

	/**
	* Creates a record
	* @param pType The type of record
	*/
	virtual ESRecordRef createRecord(const char* pType);

	ESCellRef getCell(int squX, int squY){
		ESCellRef ref = mpExteriorCell[squX][squY];
		return ref;
	}

	ESCellRef getCell(const std::string &name){
		ESCellRef ref = mpInteriorCell[name];
		return ref;
	}

	ESLandRef getLand(int squX, int squY){
		ESLandRef ref = mpLand[squX][squY];
		ref->loadSection(100);
		return ref;
	}

	ESPathGridRef getPathGrid(int squX, int squY){
		ESPathGridRef ref = mpPathGrid[squX][squY];
		return ref;
	}

	bool cellExists(const std::string &name){
		if ( mpInteriorCell.find(name) != mpInteriorCell.end() )
			return true;
		return false;
	}

	bool cellExists(const int squX, const int squY){
		if ( map2dDoesExist<int, ESCellRef>(squX, squY, mpExteriorCell) )
			return true;
		return false;
	}

	bool landExists(const int squX, const int squY){
		if ( map2dDoesExist<int, ESLandRef>(squX, squY, mpLand) )
			return true;
		return false;
	}

	/**
	* Checks if the path grid for the given cell exists in this data
	* @param squX the poistion of the cell on the x axis on the cell grid
	* @param squY the poistion of the cell on the y axis on the cell grid 
	*/
	bool pathGridExists(const int squX, const int squY){
		if ( map2dDoesExist<int, ESPathGridRef>(squX, squY, mpPathGrid) )
			return true;
		return false;
	}


	/**
	* Loads an file
	* @param &pFile the name of the file
	*/
	void loadFile(const std::string& pFile);

	bool getLTexExists(int index){
		if ( mLandTex.find(index) == mLandTex.end() )
			return false;
		return true;
	}
	std::string getLTexPath(int index){

		//return mLandTexVec.at(index)->getPath();

		return mLandTex[index]->getPath();
	}
};

}//namespace
#endif
