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
#include "ESActivator.h"

using namespace ES3;

std::string ESActivator::getName(){
	return mName.get();
}


std::string ESActivator::getScript(){
	return mScript.get();
}

void ESActivator::read(std::ifstream &ifs, long recordSize){

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

	return;

}