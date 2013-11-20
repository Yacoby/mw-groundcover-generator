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
#ifndef _ESBASE_H_
#define _ESBASE_H_

#include <map>
#include "MapFunctions.h"


#include <vector>

#include "ESFile.h"


namespace ES3{

enum { FILE_ESM, FILE_ESP, FILE_ESS };


class ESFileContainer;
typedef ESRef<ESFileContainer> ESFileContainerRef;


/**
* A helper class. Use it to load multiple files and then access data.
* It is advisable to always use this for mutliple files.
*/
class ESFileContainer{
private:
		mutable unsigned int _ref_count;
protected:

	/**
	* List of all the records that have IDs, excluding cells. 
	* As esm/ps should be loaded in load order, this includes the data overwriting "feature"
	*/
	std::map<std::string, ESRecordRef> mRecord;

	/**
	* Contains a list of land data. As land is only for exterior cells, it is easyer to access like this;
	* \todo Could do with a marco for constructing it?
	*/
	STD_MAP2d(int, ESLandRef) mLand;

	/**
	* Contains a list of pathgrid data for exterior cells.
	* \todo Check if this is needed. It may be more problem than it is worth
	*/
	STD_MAP2d(int, ESPathGridRef) mPathGrid;

	/**
	* Contains a list of the files. This is needed as some records canot be overwriten. (Cells for example),
	* so need to be stored on a per file basis.
	*/
	std::vector<ESFileRef> mFile;


	///This contains the save data. When we want to clear the save data, we just ditch this, and clear the cache
	ESFileRef mSave;

	std::map<std::string, ESCellRef> mpInteriorCellCache;


	/**
	* loads a file. Use the loadESM, loadESP functions to use this
	*/
	void loadFile(const std::string& pFile);
public:


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
	* Registers all the record types
	*/
	ESFileContainer(){
		//ES3::ES_Startup();
		_ref_count = 0;
	}

	/**
	* Destructor. ...
	*/
	~ESFileContainer(){	}

	/**
	* Loads a percentage of a esmp file
	*/
	bool loadDataFile(const std::string& file, float amount = 100){
		static std::map<std::string, ESFileRef> files;

		if ( files.find(file) == files.end() )
			files[file] = new ESFile;

		{
			if ( files[file]->loadFile(file, amount) ){

				std::map<std::string, ESRecordRef>* recs = files[file]->getRecords();
				
				std::map<std::string, ESRecordRef>::iterator itr;
				for (itr = recs->begin(); itr != recs->end(); itr++) {
					mRecord[itr->first] = itr->second;
				}

				mFile.push_back(files[file]);
				files.erase(file);

				return true;
			}

			return false;

		}
		return true;
	}

	/**
	* Loads an esmp file
	*/
	//bool loadDataFile(const std::string& file){
	//	ESFileRef data = new ESFile;
	//	data->loadFile(file);

	//	std::map<std::string, ESRecordRef>* recs = data->getRecords();
	//	
	//	std::map<std::string, ESRecordRef>::iterator itr;
	//	for (itr = recs->begin(); itr != recs->end(); itr++) {
	//		mRecord[itr->first] = itr->second;
	//	}


	//	mFile.push_back(data);
	//	return true;
	//}

	/**
	* Loads a save. Only one save can be loaded at any one time
	*/
	bool loadSaveFile(const std::string& file){
	}



	/**
	* Gets a record from the list of records. This has to be a record with an ID.
	* @param &id The id of the record
	*/
	ESRecordRef getRecord(const std::string &id){
		if ( mRecord.find(id) == mRecord.end())
			return NULL;
		return mRecord[id];
	}


	/**
	*	@return this should give the path to the ltex with da given index.
	*	\todo check this works. may not
	*/
	std::string getLTexPath(int index){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->getLTexExists(index) )
				return mFile[i]->getLTexPath(index);
		}
		return "";
	}

	bool loadCell(const std::string& name, float amount = 100, ESCellListener* c = NULL){
		bool allDone = true;
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(name) ){

				if ( mFile[i]->getCell(name)->isSectionLoaded() )
					continue;

				if ( c )
					mFile[i]->getCell(name)->setFRMRLoadListener(c);
				mFile[i]->getCell(name)->loadSection(amount);

				if ( !mFile[i]->getCell(name)->isSectionLoaded() )
					allDone = false;
			}
		}
		return allDone;
	}

	ESDistantCellRef getDistantCell(int x, int y){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( map2dDoesExist<int, ESDistantCellRef>(x, y, mFile[i]->getDistantCells()) ){
				return mFile[i]->getDistantCells()[x][y];
			}
		}
		return NULL;

	}

	bool loadCell(int squX, int squY, float amount = 100, ESCellListener* c = NULL){
		return loadCellSection(squX, squY, amount, c);
	}

	/**
	* Loads the cell data into memory.
	* \todo remove this?
	*/
	//void loadCell(int squX, int squY){}

	bool loadCellSection(int squX, int squY, float amount, ESCellListener* c = NULL){
		bool allDone = true;
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(squX, squY) ){

				if ( mFile[i]->getCell(squX, squY)->isSectionLoaded() )
					continue;

				if ( c )
					mFile[i]->getCell(squX, squY)->setFRMRLoadListener(c);
				mFile[i]->getCell(squX, squY)->loadSection(amount);

				if ( !mFile[i]->getCell(squX, squY)->isSectionLoaded() )
					allDone = false;
			}
		}
		return allDone;
	}


	bool loadLand(int x, int y, float amount = 100){

		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->landExists(x, y) ){

				if ( mFile[i]->getLand(x, y)->isSectionLoaded() )
					return true;

				mFile[i]->getLand(x, y)->loadSection(amount);

				if ( mFile[i]->getLand(x, y)->isSectionLoaded() )
					return true;
			}
		}
		return false;
	}

	//ESLandRef getLand(int x, int y){
	//	for ( unsigned i = 0; i < mFile.size(); i++ ){
	//		if ( mFile[i]->landExists(x, y) ){
	//			return mFile[i]->getLand(x, y);
	//		}
	//	}
	//	return NULL;
	//}



	/**
	* gets the active data about a cell
	* @return A pointer to the merged data about the cell
	*/
	ESCellRef getCell(const std::string& pName){

		ESCellRef pMergedCell = new ESCell;

		//the last file that modifes the cell
		ESFileRef pLastFile = NULL;

		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(pName) ){

				//store the last file
				pLastFile = mFile.at(i);

				//transfer records here...
				ESCellRef pCell = mFile.at(i)->getCell(pName);


				if ( pCell->isSectionLoading() )
					pCell->loadSection(100);
				//if ( !pCell->isLoaded() )
				if ( !pCell->isSectionLoaded() )
					pCell->loadSection(100);


				//for ( unsigned r = 0; r < pCell->getNumESRefs(); r++ ){
				//	pMergedCell->addESRef(pCell->getESRef(r));
				//}

				pCell->resetESRefItr();
				while ( pCell->refItrGood() ){
					pMergedCell->addESRef(pCell->getNextESRef());
				}
			}
		}

		//The was no file that had that cell in it...
		if ( pLastFile == NULL )
			return pMergedCell;


		ESCellRef pLastCell = pLastFile->getCell(pName);

		///add other things that are merged...
		pMergedCell->setCellData(*pLastCell->getCellData());
		pMergedCell->setWaterHeight(pLastCell->getWaterHeight());
		pMergedCell->setAmbiData(*pLastCell->getAmbiData());

		//cache the cell name
		mpInteriorCellCache[pName] = pMergedCell;

		//return the merged here
		return pMergedCell;
	}

	ESCellRef getFirstCell(int x, int y){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(x,y) ){
				return mFile[i]->getCell(x,y);
			}
		}
		return NULL;
	}
	ESFileRef getFirstCellFile(int x, int y){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(x,y) ){
				return mFile[i];
			}
		}
		return NULL;
	}

	/**
	* This combines the cell for all the loaded data files, and returns it as a single ref.
	* Obviously, some operations won't work with the returned ref
	*/
	ESCellRef getCell(int squX, int squY){


		ESCellRef pMergedCell = new ESCell;

		//the last file that modifes the cell
		ESFileRef pLastFile = NULL;

		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile.at(i)->cellExists(squX, squY) ){

				//store the last file
				pLastFile = mFile.at(i);

				//transfer records here...
				ESCellRef pCell = mFile.at(i)->getCell(squX, squY);


				if ( pCell->isSectionLoading() )
					pCell->loadSection(100);
				//if ( !pCell->isLoaded() )
				if ( !pCell->isSectionLoaded() )
					pCell->loadSection(100);


				//for ( unsigned r = 0; r < pCell->getNumESRefs(); r++ ){
				//	pMergedCell->addESRef(pCell->getESRef(r));
				//}

				pCell->resetESRefItr();
				while ( pCell->refItrGood() ){
					pMergedCell->addESRef(pCell->getNextESRef());
				}
			}
		}

		//The was no file that had that cell in it...
		if ( pLastFile == NULL )
			return pMergedCell;


		
		//ESCell* pLastCell = pLastFile->getCell(pName);

		///add other things that are merged...
		pMergedCell->setCellData(*pLastFile->getCell(squX, squY)->getCellData());
		//pMergedCell->setWaterHeight(pLastCell->getWaterHeight());
		//pMergedCell->setAmbiData(*pLastCell->getAmbiData());
		//*/

		//cache the cell name
//		mpExteriorCellCache[pName] = pMergedCell;

		//return the merged here
		return pMergedCell;
	}

	/**
	*	Retuurns the land data for the selected grid
	*	@param squX The grid square x
	*	@param squY the grid square y
	*/
	ESLandRef getLand(int squX, int squY){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->landExists(squX, squY) ){
				return mFile[i]->getLand(squX, squY);
			}
		}
		return NULL;
	}

	ESFileRef getLandFile(int squX, int squY){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->landExists(squX, squY) ){
				return mFile[i];
			}
		}
		return NULL;
	}

	ESPathGridRef getPathGrid(int squX, int squY){
//		return &mPathGrid.at(squX).at(squY);
		return NULL;
	}
};

}//namespace

#endif
