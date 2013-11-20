#include "ESRecord.h"
#include "ESSubString.h"
#include "ESSubCellData.h"


namespace ES3{

	class DOBJ;
	typedef ESRef<DOBJ> DOBJRef;

	/**
	* @brief holds data about a single distant object
	*/
	class DOBJ{
	private:
		mutable unsigned int _ref_count;
	public:

		/**
		* @brief class contains data about the position etc of the object
		*/
		struct DOBJData{
			float posX;
			float posY;
			float posZ;

			float rotX;
			float rotY;
			float rotZ;

			//the object size is used as a mult for view distances
			float size;
		};

		void ref_addESRef() const{
			++_ref_count;
		}

		void ref_subtractESRef() const{
			_ref_count--;
			if ( _ref_count < 1 ) {
				delete this;
			}
		}


		ESSubString meshName;
		DOBJData data;




	};

	class ESDistantCell;
	typedef ESRef<ESDistantCell> ESDistantCellRef;
	/**
	* @brief custom implemented record for a esmp. Contains a list of distant objects. It is always an exterior cell.
	*
	*	
	*/
	class ESDistantCell :public ESRecord{
	
		ESSubCellData mCellData;
		std::list<DOBJRef> mObjects;
	public:

		inline long getCellX(){ return mCellData.gridX();}
		inline long getCellY(){ return mCellData.gridY();}
		inline void setCellX(long c){mCellData.setGridX(c);}
		inline void setCellY(long c){mCellData.setGridY(c);}

		inline std::list<DOBJRef>& getDistantObjects(){ return mObjects; }

		void read(std::ifstream &ifs, long recordSize){
			long readTo = recordSize + ifs.tellg();

			mCellData.read(ifs);

			while ( ifs.tellg() < readTo ){
				ifs.seekg((long)ifs.tellg() + 8); //skip hdr
				DOBJRef ref = new DOBJ;
				ref->meshName.read(ifs);
				ifs.read((char*)&ref->data.posX, 4);
				ifs.read((char*)&ref->data.posY, 4);
				ifs.read((char*)&ref->data.posZ, 4);
				ifs.read((char*)&ref->data.rotX, 4);
				ifs.read((char*)&ref->data.rotY, 4);
				ifs.read((char*)&ref->data.rotZ, 4);
				ifs.read((char*)&ref->data.size, 4);
				mObjects.push_back(ref);
			}

		}

		void write(std::ofstream& ofs){
			ofs.write("DCEL", 4);


			long totalSize = 12 + 4; //12 is the lengh of mCellData

			//calculate size and write
			for ( std::list<DOBJRef>::iterator iter = mObjects.begin(); iter != mObjects.end(); ++iter)
				totalSize += (*iter)->meshName.writeSize() + (9 * 4);

			ofs.write((char*)&totalSize, 4);

			//write unknown long
			ofs.write((char*)&totalSize, 4);

			//write flags
			ofs.write((char*)&totalSize, 4);
			
			mCellData.write(ofs);

			for ( std::list<DOBJRef>::iterator iter = mObjects.begin(); iter != mObjects.end(); ++iter){
				ofs.write("DOBJ", 4);
				long subSize = (*iter)->meshName.writeSize() + (7 * 4);
				ofs.write((char*)&subSize, 4);
				(*iter)->meshName.write(ofs);
				ofs.write((char*)&(*iter)->data.posX, 4);
				ofs.write((char*)&(*iter)->data.posY, 4);
				ofs.write((char*)&(*iter)->data.posZ, 4);
				ofs.write((char*)&(*iter)->data.rotX, 4);
				ofs.write((char*)&(*iter)->data.rotY, 4);
				ofs.write((char*)&(*iter)->data.rotZ, 4);
				ofs.write((char*)&(*iter)->data.size, 4);
			}

		}

	};
}