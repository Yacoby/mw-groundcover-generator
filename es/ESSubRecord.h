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
#ifndef _ESSubRecord_H_
#define _ESSubRecord_H_

#include <fstream>
#include <string>
namespace ES3{
class ESSubRecord{
private:
	mutable unsigned int _ref_count;
protected:
	long mRecordSize;	//Size of the record when output

	std::string	mData;			//The record data for an unknown type

	char mRecordType[4]; //conatains the record type as a 4 char string
public:

	void ref_addESRef() const{
		++_ref_count;
	}

	 void ref_subtractESRef() const{
		_ref_count--;
		if ( _ref_count < 1 ) {
			delete this;
		}
	}


	/**
	* Reads the subrecord from the filestream
	*/
	virtual void read(std::ifstream &ifs);
	virtual void write(std::ofstream &ofs);
};
}
#endif

