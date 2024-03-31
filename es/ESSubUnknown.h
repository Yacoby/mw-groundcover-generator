#ifndef _ESSUBUNKNOWN_H_
#define _ESSUBUNKNOWN_H_

#include <fstream>

#include "ESSubRecord.h"

namespace ES3 {
/**
* This is used to hold unknown blocks of data
*/
    class ESSubUnknown : public ESSubRecord {
    public:
        void read(std::ifstream &ifs);
    };
}
#endif