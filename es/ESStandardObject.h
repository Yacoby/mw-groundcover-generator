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
#ifndef _ESSTANDARDOBJECT_H_
#define _ESSTANDARDOBJECT_H_

#include "ESRecord.h"

namespace ES3{

class ESStandardObject;
typedef ESRef<ESStandardObject> ESStandardObjectRef;

/**
* class contains data for all items that can be placed in the game world. Armor, probes. 
* Everything exluding npcs (because getMesh won't work this them...), cells, land and pathgrids
*/
class ESStandardObject : public ESRecord{
protected:
	///The id of the object
	ESSubString mId;

	///the name of the object
	ESSubString mMesh;

public:
	/**
	* @brief Returns the id of the object
	*/
	std::string getID();

	/**
	* @brief  Returns the mesh of the object
	*/
	std::string getMesh();

	/**
	* @brief does nothing. Makes the class !abstract
	*/
	virtual void read(std::ifstream& ifs, long recordSize){}
};
}//namespace
#endif