#ifndef _ESSubString_H_
#define _ESSubString_H_

#include "ESSubRecord.h"

namespace ES3 {
    class ESSubString : public ESSubRecord {
    private:
        std::string mSubStr;
    public:
        std::string get();

        void read(std::ifstream &ifs);
    };
}
#endif