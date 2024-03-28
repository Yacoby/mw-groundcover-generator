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
    uint32_t mLong;
public:

	///gets the float stored in the subclass
    uint32_t get();

	///sets the float stored in the subclass
	void set(uint32_t l);

	///reads the string from the given file, and saves the data.
	void read(std::ifstream &ifs);
};

}

#endif