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
#include <cstring>
#include "ESCell.h"

using namespace ES3;

void ESCell::read(std::ifstream &ifs, long recordSize){
	long readTo = recordSize + ifs.tellg();

//	boost::mutex::scoped_lock scoped_lock(mMutexAllData);
//	mFRMREndOffset = readTo;
	mDataEnd = readTo;


	mDataStart = -1; //by default, nothing foruym.

	//ifs.seekg(readTo);
	//return;

	//Don't read any of this


	bool isReadingESRefObj = false;

	
	ESSubCellFRMR* newCellESRef = NULL;

	long tmpOffset;

	while ( ifs.tellg() < readTo ){

		tmpOffset = ifs.tellg();


		char dataType[5];
		ifs.read(dataType, 4);

		
		//string must be null terminated.
		dataType[4] = '\0';

		//we have got to the frmrecord part. quit.
		if ( strcmp(dataType, "FRMR") == 0){
//			mFRMRStartOffset = tmpOffset;
			mDataStart = tmpOffset;
			return;

			/*
			if ( newCellESRef != NULL ){
				if ( mCellESRef.size() < newCellESRef->getFRMR() + 1 )
					mCellESRef.resize(newCellESRef->getFRMR() + 1, NULL);
				mCellESRef.at(newCellESRef->getFRMR()) = newCellESRef;
			}

			newCellESRef = new ESSubCellFRMR;
			isReadingESRefObj = true;
			*/
		}

		//if ( isReadingESRefObj == false ){

			if ( strcmp(dataType, "NAME") == 0)
				mCellName.read(ifs);
			else if ( strcmp(dataType, "DATA") == 0)
				mCellData.read(ifs);
			else if ( strcmp(dataType, "RGNN") == 0)
				mRegn.read(ifs);
			else if ( strcmp(dataType, "WHGT") == 0)
				mWaterHeight.read(ifs);
			else if ( strcmp(dataType, "AMBI") == 0)
				mAmbiData.read(ifs);
			else if ( strcmp(dataType, "NAM0") == 0)
				mColour.read(ifs);
			else
				handelUnknownRecord(ifs);

			/*
		}else{

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
		*/
	}
/*
	if ( newCellESRef != NULL ){
		if ( mCellESRef.size() < newCellESRef->getFRMR() + 1 )
			mCellESRef.resize(newCellESRef->getFRMR() + 1, NULL);
		mCellESRef.at(newCellESRef->getFRMR()) = newCellESRef;
	}
	*/

	ifs.seekg(readTo);

}
