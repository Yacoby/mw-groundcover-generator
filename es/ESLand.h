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
#ifndef _ESLAND_H_
#define _ESLAND_H_

#include "ESRecord.h"
#include "ESRecordSectionLoader.h"
#include <math.h>

#define PI 3.14159265


namespace ES3{

#define ES3_CELL_SIZE 65

	/**
	* @brief represnets the xyz of a normal
	*/
	class Normal{
	public:
		Normal() : mIsValid(false){};
		float x, y, z;
		inline bool isValid(){return mIsValid;}
		inline void isValid(bool f){ mIsValid = f; }
	private:
		bool mIsValid;

	};

	/**
	* @brief holds a float that defines the 
	*/
	class Height{
	public:
		Height() : mIsValid(false){};
		int height;
		inline bool isValid(){return mIsValid;}
		inline void isValid(bool f){ mIsValid = f; }
	private:
		bool mIsValid;
	};

	struct ES3LandPoint{
		Height height;
		Normal normal;
	};

	class HeightMap{

		ES3LandPoint mHeightMap[ES3_CELL_SIZE][ES3_CELL_SIZE];

	public:
		inline ES3LandPoint& get(int x, int y){
#ifdef _DEBUG
			if ( x >= ES3_CELL_SIZE || y >= ES3_CELL_SIZE ){throw std::exception("Cells do not have > 64 verts");}
#endif
			return mHeightMap[x][y];
		}

	};


/**
* Used for holding the position in the cell grid of a land record
*/
struct LandSquare{
	long cellX;
	long cellY;
};



/**
*	Simple vector3 implementation. This removes the depandance on any other library that implements this (Ogre, Niflib)
*/
struct Vector3{
	float x;
	float y;
	float z;

	Vector3(){
		x = 0;
		y = 0;
		z = 0;
	}
};

class ESLand;
typedef ESRef<ESLand> ESLandRef;

typedef std::vector< std::vector<Vector3> > LandHeightMap;
typedef std::vector< std::vector<Vector3> > LandNormalMap;


/**
*	Holds a LAND record.
*/

class ESLand : public ESRecordSectionLoader{


	HeightMap mHeightmap;

	std::vector< std::vector<short> > mLandTextures;

	/**
	* List of the normals
	*/
	LandNormalMap mLandNormal;

	/**
	* height data
	* The height data is not absolute values but uses differences between adjacent pixels.
	* Thus a pixel value of 0 means it has the same height as the last pixel. Note that
	* the y-direction of the data is from the bottom up.
	*/
	std::vector< std::vector<int> > mHeightData;

	/**
	* The height offset. This moves the land up or down by x amount
	*/
	float mHeigthOffset;

	long mVTEXOffset, mVHGTOffset, mVNMLOffset;

	/**
	* The data on where in the world the cell is
	*/
	LandSquare mLandSquare;

	long mLandStartOffset;
	long mLandEndOffset;

	struct SectionState{
		enum {L_NOT, L_VNML, L_VHGT, L_VTEX};
		int VNML_x;
		int VNML_y;

		int VHGT_x;
		int VHGT_y;

		int VTEX_x1;
		int VTEX_y1;
		int VTEX_x2;
		int VTEX_y2;

		int LoadingState;
	};

	SectionState mLoadingState;

	int mVHGTx, mVHGTy;
	int mVTEXy1, mVTEXx1, mVTEXy2, mVTEXx2;
	int mReadingState;
	enum { READING_NOTHING, READING_VHGT, READING_VTEX, READING_DONE };

public:

	ESLand(){

		if ( mHeightData.size() < 65 ){
			mHeightData.resize(65);
			for ( int i = 0 ; i < 65; i++ )		mHeightData.at(i).resize(65, -256);
		}

		if ( mLandNormal.size() < 65 ){
			mLandNormal.resize(65);
			for ( unsigned x = 0; x < 65; x++ ) mLandNormal.at(x).resize(65, Vector3());
		}

		if ( mLandTextures.size() < 16 ){
			mLandTextures.resize(16);
			for ( int x = 0; x < 16; x++ ){	mLandTextures.at(x).resize(16, 0); mLandTextures.at(x).at(0) = 0; }
		}

			mHeigthOffset = 0;

		mVHGTx = 1;
		mVHGTy = 0;
		mVTEXy1 = 0;
		mVTEXx1 = 0;
		mVTEXy2 = 0;
		mVTEXx2  = 0;

		mReadingState = READING_NOTHING;


		mLoadingState.VNML_x = 0;
		mLoadingState.VNML_y = 0;
		mLoadingState.VHGT_x = 1;
		mLoadingState.VHGT_y = 0;
		mLoadingState.VTEX_x1 = 0;
		mLoadingState.VTEX_x2 = 0;
		mLoadingState.VTEX_y1 = 0;
		mLoadingState.VTEX_y2 = 0;
		mLoadingState.LoadingState = SectionState::L_NOT;

	}

	void setLandHeight(int h){
		for ( int x = 0 ; x < 65; x++ ){
			for ( int y = 0; y < 65; y++ )	mHeightData.at(x).at(y) = h;
		}
	}

	bool loadSection(float amount){


		if ( mIsSectionLoaded ) return true;
		//hackyfix
		doThreadLoad();
		mIsSectionLoaded = true;
		return true;

		if ( mVHGTOffset < 0 ) return true;

		if ( !mIsSectionLoading )
			loadSectionStart();

#ifdef _DEBUG
		if ( amount < 0 )
			return false;
#endif
		

		long amountToLoad = (long) (( mDataEnd - mDataStart ) * (float)( amount / 100 ));
		if ( amountToLoad < 1 )
			amountToLoad = 1;

		long readTo = mDataReadTo + amountToLoad;
		if ( readTo > mDataEnd )
			readTo = mDataEnd;

		if ( amount >= 100 )
			readTo = mDataEnd;

		mAmountLoaded += amount;

//		long junk;

		//while ( mDataStream->tellg() < mDataEnd ){
		//	if ( mLoadingState.LoadingState == SectionState::L_NOT ){
		//		char dataType[5];
		//		mDataStream->read(dataType, 4);
		//		dataType[4] = '\0';

		//		if ( strcmp(dataType, "VNML") == 0){
		//			mDataStream->read ((char *)&junk, sizeof(long));

		//			mLandNormal.resize(65);
		//			mLoadingState.LoadingState = SectionState::L_VNML;
		//		}else if (strcmp(dataType, "VHGT") == 0){
		//			mDataStream->read ((char *)&junk, sizeof(long));

		//			mDataStream->read ((char *)&mHeigthOffset, sizeof(float));

		//			mHeightData.resize(65);
		//			for ( int i = 0 ; i < 65; i++ )
		//				mHeightData.at(i).resize(65);

		//			mLoadingState.LoadingState = SectionState::L_VHGT;
		//		}else if ( strcmp(dataType, "VTEX") == 0 ){
		//			mDataStream->read ((char *)&junk, sizeof(long));


		//			mLandTextures.resize(16);
		//			for ( int x = 0; x < 16; x++ )
		//				mLandTextures.at(x).resize(16);

		//			mLoadingState.LoadingState = SectionState::L_VTEX;
		//		}


		//	switch(mLoadingState.LoadingState ){
		//		case SectionState::L_VNML: //at min, reads 65*3 bytes
		//			for ( mLoadingState.VNML_x; mLoadingState.VNML_x < 65; mLoadingState.VNML_x++ ){
		//				mLandNormal.at(mLoadingState.VNML_x).resize(65);
		//				for ( mLoadingState.VNML_y; mLoadingState.VNML_y < 65; mLoadingState.VNML_y++ ){
		//					mDataStream->read ((char *)
		//						&mHeightmap.get(mLoadingState.VNML_x, mLoadingState.VNML_y).normal.x, 
		//						1);
		//					mDataStream->read ((char *)
		//						&mHeightmap.get(mLoadingState.VNML_x, mLoadingState.VNML_y).normal.y, 
		//						1);
		//					
		//					mDataStream->read ((char *)
		//						&mHeightmap.get(mLoadingState.VNML_x, mLoadingState.VNML_y).normal.z, 
		//						1);

		//					mHeightmap.get(mLoadingState.VNML_x, mLoadingState.VNML_y).normal.isValid(true);
		//					//mDataStream->read ((char *)&mLandNormal.at(mLoadingState.VNML_x).at(mLoadingState.VNML_y).x, 1);
		//					//mDataStream->read ((char *)&mLandNormal.at(mLoadingState.VNML_x).at(mLoadingState.VNML_y).y, 1);
		//					//mDataStream->read ((char *)&mLandNormal.at(mLoadingState.VNML_x).at(mLoadingState.VNML_y).z, 1);
		//				}
		//				continue;
		//			}
		//			mLoadingState.LoadingState = SectionState::L_NOT;
		//			break;
		//		case SectionState::L_VHGT:
		//			{
		//				float offset = mHeigthOffset;

		//				for(mLoadingState.VHGT_y; mLoadingState.VHGT_y < 65; mLoadingState.VHGT_y++) {
		//					char x;
		//					mDataStream->get(x);	
		//					offset += x;
		//					mHeightData.at(0).at(mLoadingState.VHGT_y) =+ (int) offset;

		//					float pos = offset;

		//					for(mLoadingState.VHGT_x; mLoadingState.VHGT_x < 65; mLoadingState.VHGT_x++) {

		//						char c;
		//						int tmp = 0; //UNUSED?
		//						mDataStream->get(c);						
		//						pos += c;

		//						//mHeightData.at(mLoadingState.VHGT_x).at(mLoadingState.VHGT_y) = (int) pos;
		//						mHeightmap.get(mLoadingState.VHGT_x, mLoadingState.VHGT_y).height.height = (int) pos;
		//						mHeightmap.get(mLoadingState.VHGT_x, mLoadingState.VHGT_y).height.isValid(true);

		//					}
		//					continue;
		//				}
		//				mLoadingState.LoadingState = SectionState::L_NOT;
		//			}
		//			break;
		//		case SectionState::L_VTEX:
		//			for(mLoadingState.VTEX_y1;mLoadingState.VTEX_y1<4;mLoadingState.VTEX_y1++) {
		//				for(mLoadingState.VTEX_x1;mLoadingState.VTEX_x1<4;mLoadingState.VTEX_x1++) {
		//					for(mLoadingState.VTEX_y2;mLoadingState.VTEX_y2<4;mLoadingState.VTEX_y2++) {
		//						for(mLoadingState.VTEX_x2;mLoadingState.VTEX_x2<4;mLoadingState.VTEX_x2++) {
		//							mDataStream->read ((char *)&mLandTextures
		//								[mLoadingState.VTEX_x1*4+mLoadingState.VTEX_y2]
		//								[mLoadingState.VTEX_y1*4+mLoadingState.VTEX_y2],
		//									sizeof(short));
		//						}
		//					}
		//				}
		//			}
		//			break;
		//	}



		//	continue;
		//	}



		//}//while( mDataStream->tellg() < mDataEnd )



		//reed to the hght data we are currently reading
		mDataStream->seekg(mDataReadTo);

		if ( mReadingState == READING_NOTHING ){
			mDataStream->seekg(mVHGTOffset);
			mReadingState = READING_VHGT;
		}else
			mDataStream->seekg(mDataReadTo);

		while( mDataStream->tellg() < mDataEnd ){
			if(mReadingState == READING_VHGT){

				if ( mHeigthOffset == 0 )
					mDataStream->read ((char *)&mHeigthOffset, sizeof(float));

				float offset = mHeigthOffset;

				if ( mHeightData.size() < 65 ){
					mHeightData.resize(65);
					for ( int i = 0 ; i < 65; i++ )	mHeightData.at(i).resize(65);
				}

				for(mVHGTy; mVHGTy < 65; mVHGTy++) {
					char x;
					mDataStream->get(x);	
					offset += x;
					mHeightData.at(0).at(mVHGTy) =+ (int) offset;

					float pos = offset;

					for(mVHGTx/* = 1*/; mVHGTx< 65; mVHGTx++) {

						char c;
						int tmp = 0;
						mDataStream->get(c);						
						pos += c;

						mHeightData.at(mVHGTx).at(mVHGTy) = (int) pos;
						readNML(mVHGTx, mVHGTy);

						if ( mDataStream->tellg() > readTo - (64 * 64 * 3) ) goto exitAllLoops; //go to end of while loop

					}
					mVHGTx = 1;
				}



				short junk2 = 0;
				mDataStream->read ((char *)&junk2, sizeof(short));

				char unk;
				mDataStream->read ((char*)&unk, 1);

				//finsihed reading vhgt data
				mReadingState = READING_VTEX;

			}else if(mReadingState == READING_VTEX){

				if ( mLandTextures.size() < 16 ){
					mLandTextures.resize(16);
					for ( int x = 0; x < 16; x++ )	mLandTextures.at(x).resize(16);
				}

				for(mVTEXy1; mVTEXy1 < 4; mVTEXy1 ++) {
					for( mVTEXx1;mVTEXx1<4;mVTEXx1++ ) {
						for(mVTEXy2;mVTEXy2<4;mVTEXy2++) {
							for(mVTEXx2;mVTEXx2<4;mVTEXx2++) {
								mDataStream->read ((char *)&mLandTextures[mVTEXx1*4+mVTEXx2][mVTEXy1*4+mVTEXy2], sizeof(short));
							}
							mVTEXx2 = 0;
						}
						mVTEXy2 = 0;
						if ( mDataStream->tellg() > readTo - (64 * 64 * 3) ) goto exitAllLoops; //go to end of while loop
					}
					mVTEXx1=0;
				}

				mReadingState = READING_DONE;
				goto exitAllLoops; //go to end of while loop

			}

			

		}

exitAllLoops: //the exit point for the reading loops


		if ( ((long) mDataStream->tellg()) == mDataEnd || mReadingState == READING_DONE ) 
			return loadSectionEnd();


		return false;
		

	}

	void readNML(int x, int y){
		long curPos = mDataStream->tellg();

		long datPos = 0;
		for ( int j = 0; j < x; j++ ){ //got to be a faster way. Can't be bothered thinking
			for ( int k = 0; k < y; k++){
				datPos += 3;
			}
		}
		datPos += mVNMLOffset;


		mDataStream->seekg(datPos);

		//read data
		mDataStream->read ((char *)&mLandNormal.at(x).at(y).x, 1);
		mDataStream->read ((char *)&mLandNormal.at(x).at(y).y, 1);
		mDataStream->read ((char *)&mLandNormal.at(x).at(y).z, 1);



		mDataStream->seekg(curPos);
	}

	/**
	*	@return a 65x65 array of normals, for each vertex of the land.
	*/
	LandNormalMap getNormals(){ return mLandNormal; }
	std::vector< std::vector<int> > getHeights(){ return mHeightData; }


	std::vector< std::vector<short> > getLandTextures(){return mLandTextures;}

	bool doThreadLoad(){
		//go to the place in the file
		std::ifstream ifs(mFileName.c_str(), std::ios::in | std::ios::binary);
		ifs.seekg(mLandStartOffset);

				long junk;

		while ( ifs.tellg() < mLandEndOffset ){
			char dataType[5];
			ifs.read(dataType, 4);

			//string must be null terminated.
			dataType[4] = '\0';
			//cell data
			if ( strcmp(dataType, "VNML") == 0){
					ifs.read ((char *)&junk, sizeof(long));

					mLandNormal.resize(65);
					for ( unsigned x = 0; x < 65; x++ ){
						mLandNormal.at(x).resize(65);
						for ( unsigned y = 0; y < 65; y++ ){
							//ifs.read ((char *)&mLandNormal[x][y], sizeof(LandNormalMap));	
							ifs.read ((char *)&mLandNormal.at(x).at(y).x, 1);
							ifs.read ((char *)&mLandNormal.at(x).at(y).y, 1);
							ifs.read ((char *)&mLandNormal.at(x).at(y).z, 1);
						}
					}

					//height data
			}else if ( strcmp(dataType, "VHGT") == 0){
				ifs.read ((char *)&junk, sizeof(long));
				ifs.read ((char *)&mHeigthOffset, sizeof(float));

				float offset = mHeigthOffset;

				//mHeightData.resize(65);
				for ( int i = 0 ; i < 65; i++ ){
					mHeightData.at(i).resize(65);
					for ( int j=0;j<65;j++){
						mHeightData[i][j]=0;
					}
				}

				for(int y = 0; y < 65; y++) {
					char x;
					ifs.get(x);	
					offset += x;
					mHeightData.at(0).at(y) =+ (int) offset;

					float pos = offset;

					for(int x = 1; x< 65; x++) {

						char c;
						int tmp = 0;
						ifs.get(c);						
						pos += c;

						mHeightData.at(x).at(y) = (int) pos;

					}
				}

				
				char unk;
				ifs.read ((char*)&unk, 1);


				short junk2 = 0;
				ifs.read ((char *)&junk2, sizeof(short));
			}else if ( strcmp(dataType, "VTEX") == 0 ){
				ifs.read ((char *)&junk, sizeof(long));

				if ( mLandSquare.cellX == -7 && mLandSquare.cellY == 4 )
					int zz = 1;

				mLandTextures.resize(16);
				for ( int x = 0; x < 16; x++ ) mLandTextures.at(x).resize(16);

				for(int y1=0;y1<4;y1++) {
					for(int x1=0;x1<4;x1++) {
						for(int y2=0;y2<4;y2++) {
							for(int x2=0;x2<4;x2++) {
								ifs.read ((char *)&mLandTextures[x1*4+x2][y1*4+y2], sizeof(short));
							}
						}
					}
				}

			}else{
				handelUnknownRecord(ifs);
			}
		}
	
		ifs.close();
		return true;
	}

	float _temp(float x, int y){
		while ( x > y) x -= y;
		while ( x < -y) x += y;
		return x;
	}
	int _temp2(float x, float y){
		int z = 0;
		while ( x > 0 ){
			x -= y;
			z++;
		}
		return z;
	}
	float _temp3(float x){
		while ( x > 64 )
			x-=64;
		return x;
	}

	float getHeightAt(float x, float y){
		float cellX = _temp(x, 8192);
		float cellY = _temp(y, 8192);

		if ( cellX < 0 )cellX = 8192 + cellX;
		if ( cellY < 0 )cellY = 8192 + cellY;

		int posX = _temp2(cellX, 128)-1;
		int posY = _temp2(cellY, 128)-1;

		////gets the offset from the vert of the object
		//float offsetX = _temp3(cellX);
		//float offsetY = _temp3(cellY);

		//float tri[3];

		//float nx1,ny1,nz1;

		//if ( offsetX < offsetY ){
		//	tri[0] = mHeightData[posX-1][posY-1];
		//	tri[1] = mHeightData[posX-1][posY];
		//	tri[2] = mHeightData[posX][posY];

		//	nx1 = (mLandNormal[posX][posY].x + mLandNormal[posX-1][posY-1].x + mLandNormal[posX-1][posY].x)/3;
		//	ny1 = (mLandNormal[posX][posY].y + mLandNormal[posX-1][posY-1].y + mLandNormal[posX-1][posY].y)/3;
		//	nz1 = (mLandNormal[posX][posY].z + mLandNormal[posX-1][posY-1].z + mLandNormal[posX-1][posY].z)/3;

		//}else{
		//	tri[0] = mHeightData[posX-1][posY-1];
		//	tri[1] = mHeightData[posX][posY-1];
		//	tri[2] = mHeightData[posX][posY];

		//	nx1 = (mLandNormal[posX][posY].x + mLandNormal[posX-1][posY-1].x + mLandNormal[posX][posY-1].x)/3;
		//	ny1 = (mLandNormal[posX][posY].y + mLandNormal[posX-1][posY-1].y + mLandNormal[posX][posY-1].y)/3;
		//	nz1 = (mLandNormal[posX][posY].z + mLandNormal[posX-1][posY-1].z + mLandNormal[posX][posY-1].z)/3;

		//}

		/////*float max = tri[0];
		////max = std::min<float>(tri[1],max);
		////max = std::min<float>(tri[2],max);*/

		//float z = tri[0]*8 + (nx1 * offsetX + ny1 * offsetY ) / float(offsetX + nz1);
		//////float z = max + (nx1 * offsetX + ny1 * offsetY ) / -nz1;
		//return z;//*8;

		//Equ
		//P.z = V0.z+ (N.x * dx + N.y * dy ) / -N.z

		//float a = mHeightData[posX-1][posY-1] - mHeightData[posX-1][posY];
		//float b = a / float(128) * offsetY;

		//float c = mHeightData[posX-1][posY-1] - mHeightData[posX][posY-1];
		//float d = c / float(128) * offsetX;

		//return (b + d + mHeightData[posX-1][posY-1])*8;
	
		/*
		 start.x = _vertex( 0, 0, 0 );
		start.y = _vertex( 0, 0, 1 );
		start.z = _vertex( 0, 0, 2 );

		end.x = _vertex( mOptions->tileSize - 1, mOptions->tileSize - 1, 0 );
		end.y = _vertex( mOptions->tileSize - 1, mOptions->tileSize - 1, 1 );
		end.z = _vertex( mOptions->tileSize - 1, mOptions->tileSize - 1, 2 );
		*/

		Vector3 end, start;

		start.z = mHeightData[0][0];


		end.x = 64*64;
		end.y = 64*64;
		end.z = mHeightData[64][64];

		//float x_pct = ( x - start.x ) / ( end.x - start.x );
		//float z_pct = ( z - start.z ) / ( end.z - start.z );

		float x_pct = ( cellX - 0 ) / ( 8192 );
		float z_pct = ( cellY - 0 ) / ( 8192 );

		float x_pt = x_pct * ( float ) ( 64 );
		float z_pt = z_pct * ( float ) ( 64 );

		int x_index = ( int ) (x_pt);
		int z_index = ( int ) (z_pt);
		//int x_index = posX;
		//int z_index = posY;

		// If we got to the far right / bottom edge, move one back
		if (x_index == 64){
			--x_index;
			x_pct = 1.0f;
		}else{
			// get remainder
			x_pct = x_pt - x_index;
		}
		if (z_index == 64){
			--z_index;
			z_pct = 1.0f;
		}else{
			z_pct = z_pt - z_index;
		}

		float t1 = mHeightData[x_index][z_index]*8;
		float t2 = mHeightData[x_index+1][z_index]*8;
		float b1 = mHeightData[x_index][z_index+1]*8;
		float b2 = mHeightData[x_index+1][z_index+1]*8;

		float midpoint = (b1 + t2) / 2.0;

		if (x_pct + z_pct <= 1) {
			b2 = midpoint + (midpoint - t1);
		} else {
			t1 = midpoint + (midpoint - b2);
		}

		float t = ( t1 * ( 1 - x_pct ) ) + ( t2 * ( x_pct ) );
		float b = ( b1 * ( 1 - x_pct ) ) + ( b2 * ( x_pct ) );

		float h = ( t * ( 1 - z_pct ) ) + ( b * ( z_pct ) );

		return h + 16;

	}

	ES3::Vector3 getAngleAt(float x, float y){
		float cellX = _temp(x, 8192);
		float cellY = _temp(y, 8192);

		if ( cellX < 0 )cellX = 8192 + cellX;
		if ( cellY < 0 )cellY = 8192 + cellY;

		int posX = _temp2(cellX, 128);
		int posY = _temp2(cellY, 128);


		float v[4];
		v[0] = (float) mHeightData[posX][posY]*8;
		v[1] = (float)mHeightData[posX - 1][posY]*8;

		v[2] =(float) mHeightData[posX][posY-1]*8;
		v[3] = (float)mHeightData[posX - 1][posY-1]*8;
		ES3::Vector3 v3;
		v3.y = (atan((v[0]-v[1])/128)+atan((v[2]-v[3])/128))/2;


		v[0] =(float) mHeightData[posX][posY]*8;
		v[1] =(float) mHeightData[posX][posY-1]*8;

		v[2] =(float) mHeightData[posX-1][posY]*8;
		v[3] = (float)mHeightData[posX-1][posY-1]*8;
		v3.x = -(atan((v[0]-v[1])/128)+atan((v[2]-v[3])/128))/2;
		return v3;
	}


	/**
	*	@return Details about what swuare the land is on
	*/
	LandSquare getLandPos(){ return mLandSquare; }

	void read(std::ifstream &ifs, long recordSize){
//		long junk;
		long readTo = recordSize + ifs.tellg();
		mLandEndOffset = readTo;
		mDataEnd = mLandEndOffset;
		//mLandStartOffset = ifs.tellg();

		while ( ifs.tellg() < readTo ){
			char dataType[5];
			ifs.read(dataType, 4);

			//string must be null terminated.
			dataType[4] = '\0';
			long subRecSize;
			if ( strcmp(dataType, "INTV") == 0){
				ifs.read ((char *)&subRecSize, sizeof(long));
				ifs.read((char*)&mLandSquare, sizeof(LandSquare));
				mLandStartOffset = ifs.tellg();
				mDataStart = ifs.tellg();
				continue;
			}else if ( strcmp(dataType, "VNML") == 0){
				ifs.read ((char *)&subRecSize, sizeof(long));
				mVNMLOffset = ifs.tellg();
				ifs.seekg(mVNMLOffset + subRecSize);
			}else if ( strcmp(dataType, "VHGT") == 0){
				ifs.read ((char *)&subRecSize, sizeof(long));
				mVHGTOffset = ifs.tellg();
				ifs.seekg(mVHGTOffset + subRecSize);
			}else if ( strcmp(dataType, "VTEX") == 0){
				ifs.read ((char *)&subRecSize, sizeof(long));
				mVTEXOffset = ifs.tellg();
				ifs.seekg(mVTEXOffset + subRecSize);
			}else{
				handelUnknownRecord(ifs);
			}
		}
		ifs.seekg(readTo);
	}

/*
	void read(std::ifstream &ifs, long recordSize){
		long readTo = recordSize + ifs.tellg();

		long junk;

		while ( ifs.tellg() < readTo ){
			char dataType[5];
			ifs.read(dataType, 4);

			//string must be null terminated.
			dataType[4] = '\0';

			//cell data
			if ( strcmp(dataType, "INTV") == 0){
					ifs.read ((char *)&junk, sizeof(long));
					ifs.read((char*)&mLandSquare, sizeof(LandSquare));


					//normals
			}else if ( strcmp(dataType, "VNML") == 0){
					ifs.read ((char *)&junk, sizeof(long));

					mLandNormal.resize(65);
					for ( unsigned x = 0; x < 65; x++ ){
						mLandNormal.at(x).resize(65);
						for ( unsigned y = 0; y < 65; y++ ){
							//ifs.read ((char *)&mLandNormal[x][y], sizeof(LandNormal));	
							ifs.read ((char *)&mLandNormal.at(x).at(y).x, 1);
							ifs.read ((char *)&mLandNormal.at(x).at(y).y, 1);
							ifs.read ((char *)&mLandNormal.at(x).at(y).z, 1);
						}
					}

					//height data
			}else if ( strcmp(dataType, "VHGT") == 0){
				ifs.read ((char *)&junk, sizeof(long));

				char unk;
				ifs.read ((char*)&unk, 1);
/*
				ifs.read ((char *)&mHeigthOffset, sizeof(float));

				char unk;
				ifs.read ((char*)&unk, 1);

				mHeightData.resize(65);
				for ( unsigned x = 0; x < 65; x++ ){

					mHeightData.at(x).resize(65);
					for ( unsigned y = 0; y < 65; y++ ){
						ifs.read ((char *)&mHeightData.at(x).at(y), 1);				
					}
				}
*/
/*
				ifs.read ((char *)&mHeigthOffset, sizeof(float));

				mHeightData.resize(65);
				for ( int i = 0 ; i < 65; i++ )
					mHeightData.at(i).resize(65);



				for(int y = 0; y < 65; y++) {
					//offset+=br.ReadSByte();
					//land.Heights[0, y]=offset;
					ifs.read ((char *)&mHeightData.at(0).at(y), 1);	
					mHeightData.at(0).at(y) =+ mHeigthOffset;


					//int pos=offset;
					int pos = mHeigthOffset;

					for(int x = 1; x< 65; x++) {

						//pos+=br.ReadSByte();
						//land.Heights[x, y]=pos;				
						char tmp;
						ifs.read ((char *)&tmp, 1);	
						pos += tmp;

						mHeightData.at(x).at(y) = tmp;



						//if(pos>VeryHigh) VeryHigh=pos;
						//if(pos<VeryLow) VeryLow=pos;
					}
				}


				short junk2 = 0;
				ifs.read ((char *)&junk2, sizeof(short));
			}else{
				handelUnknownRecord(ifs);
			}
		}
	}
	*/
};

}//nspace

#endif