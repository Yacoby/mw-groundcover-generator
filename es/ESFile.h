#ifndef _ESDATA_H_
#define _ESDATA_H_


#include <map>
#include <list>
#include <cstring>
#include "MapFunctions.h"

#include <vector>

//Records
#include "ESRef.h"


#include "ESRecord.h"

#include "ESHdr.h"

#include "ESCell.h"
#include "ESLand.h"
#include "ESLTex.h"


namespace ES3{

	class ESFile;
	typedef ESRef<ESFile> ESFileRef;

/**
* Holds the data for a single Morrowind file.
*/
class ESFile{
private:
	mutable unsigned int _ref_count;
protected:
	/**
	* A list of exterior cells, listed by grid
	*/
	STD_MAP2d(int, ESCellRef) mpExteriorCell;

	/**
	* List of intior cells, listed by cell name
	*/
	std::map<std::string, ESCellRef> mpInteriorCell;

	STD_MAP2d(int, ESLandRef) mpLand;

	std::map<long, ESLTexRef> mLandTex;
	std::vector<ESLTexRef> mLandTexVec;

	long mFileSize;

	ESHeaderRef mHeader;
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

	void loadStartup(std::ifstream* mIfs);
	bool loadFile(const std::string& pFile);


	ESFile();
	~ESFile();

	ESLTexRef getLTex(long id){
		return mLandTexVec.at(id);
	}

	ESCellRef getCell(int squX, int squY){
		ESCellRef ref = mpExteriorCell[squX][squY];
		return ref;
	}

	ESCellRef getCell(const std::string &name){
		ESCellRef ref = mpInteriorCell[name];
		return ref;
	}

	ESLandRef getLand(int squX, int squY){
		ESLandRef ref = mpLand[squX][squY];
		return ref;
	}

	bool cellExists(const std::string &name){
		if ( mpInteriorCell.find(name) != mpInteriorCell.end() )
			return true;
		return false;
	}

	bool cellExists(const int squX, const int squY){
		if ( map2dDoesExist<int, ESCellRef>(squX, squY, mpExteriorCell) )
			return true;
		return false;
	}

	bool landExists(const int squX, const int squY){
		if ( map2dDoesExist<int, ESLandRef>(squX, squY, mpLand) )
			return true;
		return false;
	}

	bool getLTexExists(int index){
		if ( mLandTex.find(index) == mLandTex.end() )
			return false;
		return true;
	}
	std::string getLTexPath(int index){

		//return mLandTexVec.at(index)->getPath();

		return mLandTex[index]->getPath();
	}
};

}//namespace
#endif
