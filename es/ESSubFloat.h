#ifndef _ESSubFloat_H_
#define _ESSubFloat_H_

#include <fstream>

#include "ESSubRecord.h"

namespace ES3 {

    class ESSubFloat : public ESSubRecord {
    private:
        float mFloat;
    public:
        float get();

        void read(std::ifstream &ifs);
    };

}

#endif

