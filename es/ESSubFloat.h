#ifndef _ESSubFloat_H_
#define _ESSubFloat_H_

#include "ESSubRecord.h"

namespace ES3{

/**
* Basicly encapsulates a float
*/
class ESSubFloat : public ESSubRecord{
private:
	///contains the value of the float
	float mFloat; 
public:

	///gets the float stored in the subclass
	float get();

	///sets the float stored in the subclass
	void set(float f);

	///reads the string from the given file, and saves the data.
	void read(std::ifstream &ifs);
	void write(std::ofstream &ofs);
};

}

#endif

