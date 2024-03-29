#include "ESSubCellData.h"

void ES3::ESSubCellData::read(std::ifstream &ifs) {
    //we know the size...
    ifs.seekg((long) ifs.tellg() + 4);

    ifs.read((char *) &mFlags, 4);
    ifs.read((char *) &mGridX, 4);
    ifs.read((char *) &mGridY, 4);
}
