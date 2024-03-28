#ifndef _ESSUBLONG_H_
#define _ESSUBLONG_H_

#include "ESSubRecord.h"

namespace ES3 {
    class ESSubLong : public ESSubRecord {
    private:
        uint32_t mLong;
    public:
        uint32_t get();

        void read(std::ifstream &ifs);
    };

}

#endif