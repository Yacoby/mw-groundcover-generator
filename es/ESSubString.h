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
#ifndef _ESSubString_H_
#define _ESSubString_H_

#include "ESSubRecord.h"
namespace ES3{
/**
* Basicly encapsulates a string
*/
class ESSubString : public ESSubRecord{
private:
	///contains the value of the string, including the null
	std::string mSubStr; 
public:

	///gets the string stored in the subclass
	std::string get();

	///sets the string stored in the subclass
	void set(const std::string &s);

	///reads the string from the given file, and saves the data.
	void read(std::ifstream &ifs);
	void write(std::ofstream &ofs);
	inline long writeSize(){return (long) mSubStr.length() + 4; }
};
}
#endif