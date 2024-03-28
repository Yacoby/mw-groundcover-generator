#include "ESSubRecord.h"

using namespace ES3;

void ESSubRecord::write(std::ofstream &ofs){}

void ESSubRecord::read(std::ifstream &ifs){

	//read the size of the string
	ifs.read ((char *)&mRecordSize, sizeof(uint32_t));

	char* tSubStr;
	tSubStr = new char[mRecordSize + 2];
	ifs.read(tSubStr, mRecordSize);

	for ( int i = 0; i < mRecordSize; i++ ){
		mData += tSubStr[i];
	}
	delete [] tSubStr;

}
