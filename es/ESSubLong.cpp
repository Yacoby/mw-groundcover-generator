#include "ESSubLong.h"

using namespace ES3;

long ESSubLong::get(){
	return mLong;
}

void ESSubLong::set(long l){
	mRecordSize = 4;
	mLong = l;
}

void ESSubLong::read(std::ifstream &ifs){

	//read the size of the string
	ifs.read ((char *)&mRecordSize, sizeof(long));
	ifs.read((char*)&mLong, mRecordSize);
}
