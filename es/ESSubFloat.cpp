#include "ESSubFloat.h"

using namespace ES3;

float ESSubFloat::get(){
	return mFloat;
}

void ESSubFloat::set(float f){
	mRecordSize = 4;
	mFloat = f;
}

void ESSubFloat::read(std::ifstream &ifs){

	//read the size of the string
	ifs.read ((char *)&mRecordSize, sizeof(long));
	ifs.read((char*)&mFloat, mRecordSize);
}

void ESSubFloat::write(std::ofstream &ofs){
	long len = 4;
	ofs.write((char*)&len, 4);
	ofs.write((char*)&mFloat, 4);
}