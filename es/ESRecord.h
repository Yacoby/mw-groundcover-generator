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
#ifndef _ESRECORD_H_
#define _ESRECORD_H_

#include <string>
#include <vector>
#include <fstream>
#include <cassert>

#include "ESRecordFactory.h"
//#include "Type.h"

#include "ESSubString.h"
#include "ESSubUnknown.h"

#include "ESRef.h"

namespace ES3{

#define RECORD_TYPE_BASE 0
#define RECORD_TYPE_STAT 1
#define RECORD_TYPE_ACTI 2
#define RECORD_TYPE_CELL 3

	enum { REC_BASE, REC_STAT, REC_ACTI, REC_CELL };

class ESRecord;
typedef ESRef<ESRecord> ESRecordRef;

/**
* Contains the data about a base record
*/
class ESRecord{
private:
	mutable unsigned int _ref_count;
protected:

	std::vector<ESSubUnknown> mUnknownRecords;

	/**
	* Used by inherited classes to deal with unknown records
	* @param &ifs ifstream positioned before the unknown record
	*/
	void handelUnknownRecord(std::ifstream &ifs){
		ESSubUnknown unk;
		unk.read(ifs);
		//mUnknownRecords.push_back(unk);
	}
	
	/***********RECORD LOCATION**********************/
	//While the record doesn't have to contain any data, it does need to know where the data is located
	///The name of the file that contains the data.
	std::string mFileName;

	///The offset of the data withing mpFileStream
	long mRecordOffset;

	/*************END LOCATION**********************/

	///ObjectIndex of the data
	unsigned long mObjectIndex;

	//Is the object deleted.
	bool mIsDeleted;

	//the class type
	const char* pType;



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

	ESRecord(){
		_ref_count = 0;
	}
	~ESRecord(){}

	void addSubRecord(){};

	void setFileName(const std::string &fileName){
		mFileName = fileName;
	}

	void setOffset(long offset){
		mRecordOffset = offset;
	}

	//record types
	const char* getType(){ return pType; }
	void setType(const char* pT){ pType = pT; }

	//deleted
	bool getIsDeleted(){ return mIsDeleted; }
	void setIsDeleted(bool d) { mIsDeleted = d; }

	/**
	* Read the subrecord
	* @param &ifs the pointer to the filestream
	* @param recordSize the size of the record to read
	*/
	virtual void read(std::ifstream &ifs, long recordSize) = 0;


};


template <class T> ESRef<T> DynamicCast( ESRecord* object ) {
		return dynamic_cast<T*>(object);
}

}//namespace

#endif
