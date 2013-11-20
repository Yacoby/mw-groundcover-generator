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
#ifndef _ESSTATIC_H_
#define _ESSTATIC_H_

#include "ESStandardObject.h"

#include "ESSubString.h"

namespace ES3{

class ESStatic;
typedef ESRef<ESStatic> ESStaticRef;

class ESStatic : public ESStandardObject{
protected:

	/**
	*Is false when no script will manipulate the object. This means that the object can be batch rendered, which
	*is a huge amount more efficant.
	*/
	bool mMightMove;

public:


	/**
	* Returns true if the object might be moved
	*/
	bool getMightMove();

	void read(std::ifstream &ifs, long recordSize);
};


template <class T> ESRef<T> DynamicCast( ESStatic* object ) {
		return dynamic_cast<T*>(object);
}

}//namespace

#endif