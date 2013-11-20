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
#ifndef _ESRECORDTHREADLOADER_H_
#define _ESRECORDTHREADLOADER_H_

//#include <boost/thread/mutex.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/bind.hpp>
#include "ESRecord.h"

namespace ES3{

/**
* @brief Class that enables recources to be loaded in a seperate sections
*
* Large records, for example, cells, that need to be loaded dynamicly may need
* to be loaded in sections. This class is an interface to allow this to happen
*/
class ESRecordSectionLoader : public ESRecord{
protected:
	/**
	* True if the resource has loaded
	*/
	bool mHasLoaded;
	bool mIsLoading;


	/**
	* The thread pointer. The same thread is used for loading an unloading (for obvious reasons)
	*/
	//boost::thread* mThread;
	//boost::mutex mLoadingMutex;



	/**
	*	Runs the thread
	*/
	//static void doRunLoadThread(ESRecordSectionLoader* pRec, bool &hasLoaded){
	//	hasLoaded = false;
	//	if ( pRec->doThreadLoad() )
	//		hasLoaded = true;
	//}

	//static void doRunUnloadThread(ESRecordSectionLoader* pRec, bool &hasLoaded){
	//	if ( pRec->doThreadUnload() )
	//		hasLoaded = false;
	//}


	/**
	* Loads the data in a new thread
	*/
	//virtual bool doThreadLoad() = 0;

	/**
	* Unloads the data loaded.
	*/
	//virtual bool doThreadUnload(){ return false; };




	/**
	* Returns true if loading of data is able to be loaded in small segments
	*/
	//virtual const bool doesSupportSegmentLoading() const { return false; }



	bool mIsSectionLoading;
	bool mIsSectionLoaded;

	/**
	* Contains a rough guess at the amount of data loaded %
	*/
	float mAmountLoaded;

	/**
	* The data offset that the data has been read to
	*/
	long mDataReadTo;

	//Contains the start of the data
	long mDataStart;
	//end of data
	long mDataEnd;

	std::ifstream* mDataStream;

	/**
	*/
	bool loadSectionStart(){
		mDataStream = new std::ifstream(mFileName.c_str(), std::ios::in | std::ios::binary);
		mDataStream->seekg(mDataStart);
		mDataReadTo = mDataStart;
		mIsSectionLoading = true;
		mIsSectionLoaded = false;
		return true;
	}

	bool loadSectionEnd(){
		if (mDataStream && mDataStream->is_open() ) mDataStream->close();
		if ( mDataStream ) 	delete mDataStream;
		mDataStream = NULL;

		mIsSectionLoaded = true;
		mIsSectionLoading = false;
		return true;
	}

	
public:
	ESRecordSectionLoader(){
//		mThread = NULL;
		mDataStream = NULL;
		mHasLoaded = false;
		mIsSectionLoading = false;
		mIsSectionLoaded = false;

	}

	//bool isLoading(){
	//	boost::mutex::scoped_lock lock(mLoadingMutex);
	//	return mIsLoading;
	//}


	/**
	* This loads part of the cell data.
	* @param amount pecent of data to load
	*/
	virtual bool loadSection(float amount) = 0;

	bool isSectionLoading(){ return mIsSectionLoading; }
	bool isSectionLoaded(){ return mIsSectionLoaded; }


	/**
	* \todo mutex lock
	*/
	//bool isLoaded(){ return mHasLoaded; }

	/**
	* Loads the data in a seprate thread.
	*/
	//void threadLoad(){
		//If thread is already in use, wait for it to finish and join it. This should never really happen...
	//	if ( mThread != NULL )
	//		join();

	//	mThread = new boost::thread(boost::bind(&doRunLoadThread, this, mHasLoaded));
	//}

//	void threadUnload(){

	//	if ( !isLoaded() )
	//		return;

	//	if ( mThread != NULL )
	//		join();



	//	mThread = new boost::thread(boost::bind(&doRunUnloadThread, this, mHasLoaded));
	//}

	/**
	* Loads the data in the current thread
	*/
	//void load(){

	//	if ( doThreadLoad() ) 
	//		mHasLoaded = true;

	//	if ( mIsLoading ){
	//		boost::mutex::scoped_lock lock(mLoadingMutex);
	//		mIsLoading = false;
	//	}
	//}

	//void unload(){

	//	if ( doThreadUnload() ) 
	//		mHasLoaded = false;
	//}

	/**
	* Waits for the loader thread to complete
	*/
	///void join(){
	//	if ( mThread ){
	//		mThread->join();
	//		delete mThread;
	//		mThread = NULL;
	//	}
	//}



};
}//namespace
#endif