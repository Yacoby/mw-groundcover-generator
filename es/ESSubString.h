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
};
}
#endif