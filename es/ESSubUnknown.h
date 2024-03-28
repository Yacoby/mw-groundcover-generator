#ifndef _ESSUBUNKNOWN_H_
#define _ESSUBUNKNOWN_H_

#include "ESSubRecord.h"

namespace ES3 {
/**
* This is used to hold unknown blocks of data
*/
    class ESSubUnknown : public ESSubRecord {
    public:
        ///reads the data from the given file, and saves the data.
        void read(std::ifstream &ifs);
    };
}
#endif