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
#include "ESSubRecord.h"

namespace ES3{

/**
* Contains the data for the cell ambi
*/
struct ESSubAmbiData{
	long mAmbiCol;
	long mSunCol;
	long mFogCol;
	float mFogDen;
};

/**
*		Cell Lighting data
*		AMBI = Ambient Light Level (16 bytes)
*			long AmbientColor
*			long SunlightColor
*			long FogColor
*			float FogDensity	
*/
class ESSubAmbi : public ESSubRecord{

public:
	ESSubAmbiData* getData(){ return (ESSubAmbiData*)mData.c_str() ; }

	long getAmbientColor(){ return getData()->mAmbiCol; }

};

}