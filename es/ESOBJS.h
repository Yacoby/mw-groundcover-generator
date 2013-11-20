#include "ESRecord.h"
#include "ESSubString.h"
#include "ESSubCellData.h"

namespace ES3{

	class ESObjectSize;
	typedef ESRef<ESObjectSize> ESObjectSizeRef;

	/**
	* @brief holds a mesh size. This can be used for sorting view distance
	*
	* @remarks this is not a standard part of the es3 file format. 
	*/
	class ESObjectSize : public ESRecord{
		ESSubFloat mSize;
		ESSubString mMeshName;
	public:
		ESObjectSize(){}
		ESObjectSize(std::string& mesh, float size){
			mMeshName.set(mesh);
			mSize.set(size);
		}

		inline float getSize(){ return mSize.get(); }
		inline void setSize(float s){mSize.set(s);}
		inline std::string getMeshName(){return mMeshName.get();}
		inline void setMeshName(const std::string& s){ mMeshName.set(s); }

		void read(std::ifstream &ifs, long recordSize){
			long readTo = recordSize + ifs.tellg();
			mMeshName.read(ifs);
			mSize.read(ifs);
		}

		void write(std::ofstream& ofs){
			ofs.write("OBJS", 4);

			long totalSize = mMeshName.writeSize() + 8;
			ofs.write((char*)&totalSize, 4);

			//write unknown long
			ofs.write((char*)&totalSize, 4);

			//write flags
			ofs.write((char*)&totalSize, 4);
			
			mMeshName.write(ofs);
			mSize.write(ofs);
		}
	};

}