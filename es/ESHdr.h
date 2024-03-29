#include "ESRecord.h"
#include "ESSubFloat.h"

namespace ES3 {

    class ESHeader;

    typedef std::shared_ptr<ESHeader> ESHeaderRef;

    class ESHeader : public ESRecord {
    private:
        ESSubFloat mVersion;
        ESSubString mComp;
        ESSubString mDisc;
    public:


        void read(std::ifstream &ifs, long recordSize);

        void write(std::string &ofs) {

        }
    };


}