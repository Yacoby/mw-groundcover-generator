#ifndef _ESSubString_H_
#define _ESSubString_H_

#include <string>
#include <fstream>

#include "ESSubRecord.h"

namespace ES3 {
    class ESSubString : public ESSubRecord {
    private:
        std::string mSubStr;
    public:
        const std::string& get();

        void read(std::ifstream &ifs);
    };
}
#endif