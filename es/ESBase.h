#ifndef _ESBASE_H_
#define _ESBASE_H_

#include <map>
#include "MapFunctions.h"


#include <vector>

#include "ESFile.h"


namespace ES3{


class ESFileContainer;
typedef ESRef<ESFileContainer> ESFileContainerRef;


/**
* A helper class. Use it to load multiple files and then access data.
* It is advisable to always use this for mutliple files.
*/
class ESFileContainer{
private:
		mutable unsigned int _ref_count;
protected:

	/**
	* List of all the records that have IDs, excluding cells. 
	* As esm/ps should be loaded in load order, this includes the data overwriting "feature"
	*/
	std::map<std::string, ESRecordRef> mRecord;

	/**
	* Contains a list of land data. As land is only for exterior cells, it is easyer to access like this;
	* \todo Could do with a marco for constructing it?
	*/
	STD_MAP2d(int, ESLandRef) mLand;

	/**
	* Contains a list of the files. This is needed as some records canot be overwriten. (Cells for example),
	* so need to be stored on a per file basis.
	*/
	std::vector<ESFileRef> mFile;
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
	* Registers all the record types
	*/
	ESFileContainer(){
		_ref_count = 0;
	}

	/**
	* Destructor. ...
	*/
	~ESFileContainer(){	}

	bool loadDataFile(const std::string& file){
		static std::map<std::string, ESFileRef> files;

		if ( files.find(file) == files.end() )
			files[file] = new ESFile;

		{
			if ( files[file]->loadFile(file) ){
				mFile.push_back(files[file]);
				files.erase(file);

				return true;
			}

			return false;

		}
		return true;
	}

	/**
	*	@return this should give the path to the ltex with da given index.
	*	\todo check this works. may not
	*/
	std::string getLTexPath(int index){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->getLTexExists(index) )
				return mFile[i]->getLTexPath(index);
		}
		return "";
	}

    ESCellRef getFirstCell(int x, int y){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->cellExists(x,y) ){
				return mFile[i]->getCell(x,y);
			}
		}
		return NULL;
	}

	/**
	*	Retuurns the land data for the selected grid
	*	@param squX The grid square x
	*	@param squY the grid square y
	*/
	ESLandRef getLand(int squX, int squY){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->landExists(squX, squY) ){
				return mFile[i]->getLand(squX, squY);
			}
		}
		return NULL;
	}

	ESFileRef getLandFile(int squX, int squY){
		for ( unsigned i = 0; i < mFile.size(); i++ ){
			if ( mFile[i]->landExists(squX, squY) ){
				return mFile[i];
			}
		}
		return NULL;
	}
};

}//namespace

#endif
