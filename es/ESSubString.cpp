#include "ESSubString.h"

using namespace ES3;

std::string ESSubString::get(){
	return mSubStr;
}

void ESSubString::set(const std::string &s){
	mRecordSize = (long) s.length();
	mSubStr = s;
}

void ESSubString::read(std::ifstream &ifs){

	//read the size of the string
	ifs.read ((char *)&mRecordSize, sizeof(long));

	char* tSubStr;
	tSubStr = new char[mRecordSize + 4];
	ifs.read(tSubStr, mRecordSize);
	tSubStr[mRecordSize] = '\0';
	mSubStr = tSubStr;
	delete [] tSubStr;

}

void ESSubString::write(std::ofstream &ofs){
	long len = (long) mSubStr.length();
	ofs.write((char*)&len, 4);
	ofs.write(mSubStr.c_str(), (std::streamsize)mSubStr.length());
}