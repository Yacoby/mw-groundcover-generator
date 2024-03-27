#ifndef _ESRECORD_H_
#define _ESRECORD_H_

#include <string>
#include <vector>
#include <fstream>
#include <cassert>

#include "ESSubString.h"
#include "ESSubUnknown.h"

#include "ESRef.h"

namespace ES3{


class ESRecord;
typedef ESRef<ESRecord> ESRecordRef;

/**
* Contains the data about a base record
*/
class ESRecord{
private:
	mutable unsigned int _ref_count;
protected:
	/**
	* Used by inherited classes to deal with unknown records
	* @param &ifs ifstream positioned before the unknown record
	*/
	void handelUnknownRecord(std::ifstream &ifs){
		ESSubUnknown unk;
		unk.read(ifs);
	}

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
