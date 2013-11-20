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
#include "ESSubString.h"
#include "ESSubLong.h"

namespace ES3{

class ESLTex;
typedef ESRef<ESLTex> ESLTexRef;

/**
* Used for holding land texture details
*/
class ESLTex : public ESRecord{
private:
	///unique id
	ESSubString mName;

	///unique index
	ESSubLong mIndex;

	///path to the texture
	ESSubString mPath;
public:

	/**
	*	@return the id of the ltex. I assume this is unique, but I am not sure.
	*/
	std::string getID(){ return mName.get(); }

	/**
	*	@return the index of the ltex. This is refered to by the landrecord. How the frek it knows between mods IDK.
	*/
	long getIndex(){ return mIndex.get(); }

	/**
	*	@return the path to the texture
	*/
	std::string getPath(){ return mPath.get(); }

	void read(std::ifstream &ifs, long recordSize){
	long readTo = recordSize + ifs.tellg();

		while ( ifs.tellg() < readTo ){
			char dataType[5];
			ifs.read(dataType, 4);

			//string must be null terminated.
			dataType[4] = '\0';

			if ( strcmp(dataType, "NAME") == 0)
				mName.read(ifs);
			else if ( strcmp(dataType, "INTV") == 0)
				mIndex.read(ifs);
			else if ( strcmp(dataType, "DATA") == 0)
				mPath.read(ifs);
			else
				handelUnknownRecord(ifs);
		}
	}


};

}