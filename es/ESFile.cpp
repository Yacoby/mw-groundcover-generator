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

#include "ESFile.h"

using namespace ES3;


ESFile::ESFile(){
	//this shoudl default at null.
	mIfs = NULL;
	mIsReading = false;
	_ref_count = 0;
}

ESFile::~ESFile(){

	//close stream if open
	if ( mIfs != NULL ){
		if ( mIfs->is_open() )
			mIfs->close();
		delete mIfs;
	}

	//clean up all pointers
	//for ( unsigned i = 0; i < mpCell.size(); i++ ){
	//	delete mpCell[i];
	//}
}

ESRecordRef ESFile::createRecord(const char* pType){

	//Uses the record factory to create a record for the type
	ESRecord* rec = ESRecordFactory::getInstance()->create(pType);

	if ( rec )
		rec->setType(pType);	
	//else
		//the record is null, so the record type can't be found...

	return rec;
}

bool ESFile::loadEnd(){
	mIsReading = false;
	return true;
}

void ESFile::loadStartup(const std::string& pFile){

	mReadTo = 0;

	mIfs = new std::ifstream(pFile.c_str(),  std::ios::in | std::ios::binary);

	if ( !mIfs->is_open() ){
		std::exception("Cannot open file");
		return;
	}

	mIfs->seekg (0, std::ios::end);
	mFileSize = mIfs->tellg();
	mIfs->seekg (0, std::ios::beg);

	int sig; //TES3
	mIfs->read ((char *)&sig, 4);

	////header size
	long hdrSize;
	mIfs->read ((char *)&hdrSize, sizeof(int));

	mIfs->seekg((long)mIfs->tellg() + 8);

	////skip the hdr
	mIfs->seekg(hdrSize + 16);

	mIsReading = true;

}

bool ESFile::loadFile(const std::string& pFile, float amount){


	if ( !mIsReading )
		loadStartup(pFile);

	long readAount = (long)(mFileSize * (float)( amount / 100 ));
	long stopReadingAt = mReadTo + readAount;

	if ( amount == 100) stopReadingAt = mFileSize;


	int lastX;
	int lastY;

	while ( mIfs->tellg() < mFileSize  && (long) mIfs->tellg() != -1  ){

		if (  mIfs->tellg() > stopReadingAt ){
			mReadTo = mIfs->tellg();
			break;
		}

		//get the record type
		char tType[5];
		mIfs->get(tType, 5);

		if ( strcmp(tType, "DDCE") == 0 ){
			mIfs->seekg((long)mIfs->tellg() - 3);
			continue;
		}


		//record block size block size
		long recordSize;
		mIfs->read ((char *)&recordSize, 4);

		//hdr1
		long hdr1;
		mIfs->read ((char *)&hdr1, 4);

		//flags
		long flags;
		mIfs->read ((char *)&flags, 4);

		long recordEndPos = recordSize + mIfs->tellg();

		ESRecordRef record = createRecord(tType);

		if ( record == NULL ){

		}else{
			record->setFileName(pFile);
			record->setOffset(mIfs->tellg());

			
			if ( strcmp(record->getType(), "ACTI") == 0 ||
			strcmp(record->getType(), "STAT") == 0 ||
			strcmp(record->getType(), "DOOR") == 0 ){
				
				record->read(*mIfs, recordSize);
				addStandardRecord(record);
			}
			if ( strcmp(record->getType(), "LAND") == 0 ){
				ESLandRef land = DynamicCast<ESLand>(record);
				land->read(*mIfs, recordSize);

				mpLand[land->getLandPos().cellX][land->getLandPos().cellY] = land;
			}
			if ( strcmp(record->getType(), "CELL") == 0 ){
				ESCellRef cell = DynamicCast<ESCell>(record);
				cell->read(*mIfs, recordSize);

				if ( cell->getCellData()->isInterior() == false )
					mpExteriorCell[cell->getCellData()->gridX()][cell->getCellData()->gridY()] = cell;
				else
					mpInteriorCell[cell->getCellName()] = cell;

			}

			if ( strcmp(record->getType(), "LTEX") == 0 ){
				ESLTexRef tex = DynamicCast<ESLTex>(record);
				tex->read(*mIfs, recordSize);
				mLandTex[tex->getIndex()] = tex;
				mLandTexVec.push_back(tex);
			}
			if ( strcmp(record->getType(), "OBJS") == 0 ){
				ESObjectSizeRef ref = DynamicCast<ESObjectSize>(record);
				ref->read(*mIfs, recordSize);
				mMeshSizes[ref->getMeshName()] = ref;

			}
			if ( strcmp(record->getType(), "DCEL") == 0 ){
				ESDistantCellRef ref = DynamicCast<ESDistantCell>(record);
				ref->read(*mIfs, recordSize);
				mDistantCells[ref->getCellX()][ref->getCellY()] = ref;
				lastX = ref->getCellX();
				lastY = ref->getCellY();
			}
		}

		//make sure we are in the right place
		long nowPos = mIfs->tellg();
		mIfs->seekg(recordEndPos);

	} //while


	if ( (long)mIfs->tellg() >= mFileSize )
		return loadEnd();
	return false;


}


void ESFile::loadFile(const std::string& pFile){

	//if ( mIfs != NULL ){
	//	if ( mIfs->is_open() )
	//		mIfs->close();
	//	delete mIfs;
	//}

	mIfs = new std::ifstream(pFile.c_str(),  std::ios::in | std::ios::binary);

	//make sure it is open
	if ( !mIfs->is_open() )
		return;

	//store the file type
	//mFileType = fileType;

	//get the size
	long esmSize;
	mIfs->seekg (0, std::ios::end);
	esmSize = mIfs->tellg();
	mIfs->seekg (0, std::ios::beg);


	int sig;
	mIfs->read ((char *)&sig, sizeof(int));

	//header size
	int hdrSize;
	mIfs->read ((char *)&hdrSize, sizeof(int));

	//skip the hdr
	mIfs->seekg(hdrSize + 16);

	//loop through the data file
	while ( mIfs->tellg() < esmSize ){

		//get the record type
		char tType[5];
		mIfs->get(tType, 5);

		//record block size block size
		long recordSize;
		mIfs->read ((char *)&recordSize, 4);

		//hdr1
		long hdr1;
		mIfs->read ((char *)&hdr1, 4);

		//flags
		long flags;
		mIfs->read ((char *)&flags, 4);

		long recordEndPos = recordSize + mIfs->tellg();

		ESRecordRef record = createRecord(tType);

		if ( record == NULL ){

		}else{
			record->setFileName(pFile);
			record->setOffset(mIfs->tellg());

			
			if ( strcmp(record->getType(), "ACTI") == 0 || strcmp(record->getType(), "STAT") == 0 ){
				record->read(*mIfs, recordSize);
				addStandardRecord(record);
			}
			if ( strcmp(record->getType(), "LAND") == 0 ){
				ESLandRef land = DynamicCast<ESLand>(record);
				land->read(*mIfs, recordSize);

				mpLand[land->getLandPos().cellX][land->getLandPos().cellY] = land;
			}
			if ( strcmp(record->getType(), "CELL") == 0 ){
				ESCellRef cell = DynamicCast<ESCell>(record);
				cell->read(*mIfs, recordSize);

				if ( cell->getCellData()->isInterior() == false )
					mpExteriorCell[cell->getCellData()->gridX()][cell->getCellData()->gridY()] = cell;
				else
					mpInteriorCell[cell->getCellName()] = cell;

			}

			if ( strcmp(record->getType(), "LTEX") == 0 ){
				ESLTexRef tex = DynamicCast<ESLTex>(record);
				tex->read(*mIfs, recordSize);
				mLandTex[tex->getIndex()] = tex;
				mLandTexVec.push_back(tex);
			}




		}
		/*
		ESBase* pESBase = ESBase::getInstance();

		//WE END UP LOSING SA LOAD OF MEMORY HERE>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
		ESRecord* pRecord = createRecord(tType);
		if ( pRecord == NULL ){
			//the record type was not defined.

		}else{
			//setup some default stuff for the file...
			pRecord->setFileName(pFile);
			pRecord->setOffset(mIfs->tellg());

			ESStandardObject* pStdObj = dynamic_cast<ESStandardObject*>(pRecord);

			if ( pStdObj == NULL ){

				ESCell* pCell = dynamic_cast<ESCell*>(pRecord);
				if ( pCell != NULL ){
					
					pCell->read(*mIfs, recordSize);

					if ( pCell->getCellData()->isInterior() ){
						mpInteriorCell[pCell->getCellName()] = pCell;
					}else{
						//there shouldn't ever be two cells with the same id in the same file, so don't check
						mpExteriorCell[pCell->getCellData()->gridX()][pCell->getCellData()->gridY()] = pCell;
					}

					mpCell.push_back(pCell);
				}

				ESLand* pLand = dynamic_cast<ESLand*>(pRecord);
				if ( pLand != NULL ){
					pLand->read(*mIfs, recordSize);


					if ( map2dDoesExist<int, ESLand*>(pLand->getLandPos()->cellX, pLand->getLandPos()->cellY, pESBase->mLand) ){
						delete pESBase->mLand[pLand->getLandPos()->cellX][pLand->getLandPos()->cellY];
					}

					pESBase->mLand[pLand->getLandPos()->cellX][pLand, pLand->getLandPos()->cellY] = pLand;

				}
				
			}else{

				//read the data, and add to the array list
				pStdObj->read(*mIfs, recordSize);

				if ( pESBase->mpRecord.find(pStdObj->getID()) != pESBase->mpRecord.end() )
					delete pESBase->mpRecord[pStdObj->getID()];
		
				pESBase->mpRecord[pStdObj->getID()] = pStdObj;

			}
			//delete pStdObj;

		}

		//delete pRecord;
		*/

		//make sure we are in the right place
		mIfs->seekg(recordEndPos);

	} //while

}