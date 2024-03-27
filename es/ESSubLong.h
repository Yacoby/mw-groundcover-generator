#ifndef _ESSUBLONG_H_
#define _ESSUBLONG_H_

#include "ESSubRecord.h"

namespace ES3{

/**
* Basicly encapsulates a float
*/
class ESSubLong : public ESSubRecord{
private:
	///contains the value of the float
	long mLong; 
public:

	///gets the float stored in the subclass
	long get();

	///sets the float stored in the subclass
	void set(long l);

	///reads the string from the given file, and saves the data.
	void read(std::ifstream &ifs);
};

}

#endif