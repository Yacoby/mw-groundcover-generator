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

#pragma once

#include "ESSubRecord.h"

namespace ES3{

/**
* Holds a load of data about cells.
*
*	DATA = Cell Data
*		long Flags
*			0x01 = Interior?
*			0x02 = Has Water
*			0x04 = Illegal to Sleep here
*			0x80 = Behave like exterior (Tribunal)
*		long GridX
*		long GridY
*/
class ESSubCellData : public ESSubRecord{
protected:
	long mFlags;
	long mGridX;
	long mGridY;
public:

	inline long getFlags(){return mFlags; }

	///Returns true if the cell is an exterior
	inline bool isInterior(){ return ((mFlags & 0x01) != 0); }
	inline bool hasWater(){ return ((mFlags & 0x02) != 0); }
	inline bool isIllegalSleep(){ return ((mFlags & 0x04) != 0); }
	inline bool actLikeExterior(){ return ((mFlags & 0x80) != 0); }

	///returns the gridx of the cell
	inline long gridX(){ return mGridX; }
	inline void setGridX(long c){ mGridX = c; }

	///returns the gridy of the cell
	inline long gridY(){ return mGridY; }
	inline void setGridY(long c){ mGridY = c; }


	void read(std::ifstream &ifs){
		//we know the size...
		ifs.seekg((long)ifs.tellg() + 4);

		ifs.read((char*) &mFlags, 4);
		ifs.read((char*) &mGridX, 4);
		ifs.read((char*) &mGridY, 4);
	}
	void write(std::ofstream &ofs){
		long size = 12;
		ofs.write((char*) &size, 4);
		ofs.write((char*) &mFlags, 4);
		ofs.write((char*) &mGridX, 4);
		ofs.write((char*) &mGridY, 4);
	}
};

}