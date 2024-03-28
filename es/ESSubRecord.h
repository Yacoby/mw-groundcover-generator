#ifndef _ESSubRecord_H_
#define _ESSubRecord_H_

#include <fstream>
#include <string>
namespace ES3{
class ESSubRecord{
private:
	mutable unsigned int _ref_count;
protected:
    uint32_t mRecordSize;	//Size of the record when output

	std::string	mData;			//The record data for an unknown type

	char mRecordType[4]; //conatains the record type as a 4 char string
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
	* Reads the subrecord from the filestream
	*/
	virtual void read(std::ifstream &ifs);
	virtual void write(std::ofstream &ofs);
};
}
#endif

