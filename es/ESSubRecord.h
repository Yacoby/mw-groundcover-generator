#ifndef _ESSubRecord_H_
#define _ESSubRecord_H_

#include <fstream>
#include <string>

namespace ES3 {
    class ESSubRecord {
    public:
        virtual ~ESSubRecord() = 0;
        virtual void read(std::ifstream &ifs) = 0;
    };
}
#endif

