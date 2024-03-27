#include "ESFile.h"

using namespace ES3;


ESFile::ESFile(){
	_ref_count = 0;
}

ESFile::~ESFile(){

}

void ESFile::loadStartup(std::ifstream* mIfs){
	mIfs->seekg (0, std::ios::end);
	mFileSize = mIfs->tellg();
	mIfs->seekg (0, std::ios::beg);

	int sig; //TES3
	mIfs->read ((char *)&sig, 4);

	////header size
	long hdrSize;
	mIfs->read ((char *)&hdrSize, sizeof(int));

	mIfs->seekg((long)mIfs->tellg() + 8);

	////skip the hdr
	mIfs->seekg(hdrSize + 16);
}

bool ESFile::loadFile(const std::string& pFile){
    std::ifstream ifStream = std::ifstream(pFile.c_str(),  std::ios::in | std::ios::binary);
    if ( !ifStream.is_open() ){
        throw std::runtime_error("Cannot open file: " + pFile);
    }

    std::ifstream* mIfs = &ifStream;
    loadStartup(&ifStream);

	long stopReadingAt = mFileSize;


	while ( mIfs->tellg() < mFileSize  && (long) mIfs->tellg() != -1  ){

		if (  mIfs->tellg() > stopReadingAt ){
			break;
		}

		//get the record type
		char tType[5];
		mIfs->get(tType, 5);

		if ( strcmp(tType, "DDCE") == 0 ){
			mIfs->seekg((long)mIfs->tellg() - 3);
			continue;
		}


		//record block size block size
		long recordSize;
		mIfs->read ((char *)&recordSize, 4);

		//hdr1
		long hdr1;
		mIfs->read ((char *)&hdr1, 4);

		//flags
		long flags;
		mIfs->read ((char *)&flags, 4);

		long recordEndPos = recordSize + mIfs->tellg();

        if ( strcmp(tType, "LAND") == 0 ){
            ESLandRef land = new ESLand();
            land->read(*mIfs, recordSize);

            mpLand[land->getLandPos().cellX][land->getLandPos().cellY] = land;
        }
        if ( strcmp(tType, "CELL") == 0 ){
            ESCellRef cell = new ESCell();
            cell->read(*mIfs, recordSize);

            if ( cell->getCellData()->isInterior() == false )
                mpExteriorCell[cell->getCellData()->gridX()][cell->getCellData()->gridY()] = cell;
            else
                mpInteriorCell[cell->getCellName()] = cell;
        }

        if ( strcmp(tType, "LTEX") == 0 ){
            ESLTexRef tex = new ESLTex();
            tex->read(*mIfs, recordSize);
            mLandTex[tex->getIndex()] = tex;
            mLandTexVec.push_back(tex);
        }


		//make sure we are in the right place
		long nowPos = mIfs->tellg();
		mIfs->seekg(recordEndPos);

	} //while


	assert( (long)mIfs->tellg() >= mFileSize );
    return true;
}