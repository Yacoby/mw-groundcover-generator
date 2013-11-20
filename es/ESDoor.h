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
#ifndef _ESDOOR_H_
#define _ESDOOR_H_

#include "ESStandardObject.h"
#include "ESSubString.h"


namespace ES3{

class ESDoor;
typedef ESRef<ESDoor> ESDoorRef;

/**
* @brief Class used for reading door details
* \todo Fix so it reads sounds aswell...
*/
class ESDoor : public ESStandardObject{
private:
	ESSubString mName;
	ESSubString mScript;
public:
	void addSubRecord();

	inline std::string getName(){ return mName.get(); }

	inline std::string getScript(){ return mScript.get(); }
	
	/**
	* Reads the activator block into memory
	* @param &ifs The filestream positioned 4 bytes into the activator block (just after ACTI)
	* @param recordSize the size of the record to read
	*/
	void read(std::ifstream &ifs, long recordSize){

		long readTo = recordSize + ifs.tellg();

		while ( ifs.tellg() < readTo ){
			char dataType[5];
			ifs.read(dataType, 4);

			//string must be null terminated.
			dataType[4] = '\0';

			if ( strcmp(dataType, "NAME") == 0)
				mId.read(ifs);
			else if ( strcmp(dataType, "FNAM") == 0)
				mName.read(ifs);
			else if ( strcmp(dataType, "MODL") == 0)
				mMesh.read(ifs);
			else if ( strcmp(dataType, "SCRI") == 0)
				mScript.read(ifs);
			else
				handelUnknownRecord(ifs);
		}
	}

};

}//namepsace

#endif