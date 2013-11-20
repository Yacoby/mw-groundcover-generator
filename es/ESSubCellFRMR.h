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


/*
		FRMR = Object Index (starts at 1) (4 bytes, long)
			This is used to uniquely identify objects in the cell.  For new files the
			index starts at 1 and is incremented for each new object added.  For modified
			objects the index is kept the same.			
		NAME = Object ID string
		XSCL = Scale (4 bytes, float) Static
		DELE = (4 byte long) Indicates that the reference is deleted.
		DODT = XYZ Pos, XYZ Rotation of exit (24 bytes, Door objects)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
		DNAM = Door exit name (Door objects)
		FLTV = Follows the DNAM optionally, lock level (long)
		KNAM = Door key
		TNAM = Trap name
		UNAM = ESReference Blocked (1 byte, 00?), only occurs once in MORROWIND.ESM
		ANAM = Owner ID string
		BNAM = Global variable/rank ID string
		INTV = Number of uses ( 4 bytes, long, 1 default), occurs even for objects that don't use it
		NAM9 = ? (4 bytes, long, 0x00000001)
		XSOL = Soul Extra Data (ID string of creature)
		DATA = ESRef Position Data (24 bytes)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate

			*/

#include "ESSubRefData.h"
#include "ESSubLong.h"
#include "ESSubFloat.h"

namespace ES3{

class ESSubCellFRMR;
typedef ESRef<ESSubCellFRMR> ESSubCellFRMRRef;

class ESSubCellFRMR : public ESSubRecord{
public:
	ESSubLong mFRMR;
	ESSubString mName;

	ESSubFloat mScale;

	ESSubLong mDel;

	ESSubRefData mPos;

public:
	std::string getID(){ return mName.get(); }
	long getFRMR(){ return mFRMR.get(); }
	float getScale(){ return mScale.get(); }
};

}