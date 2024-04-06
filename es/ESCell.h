#ifndef _ESCELL_H_
#define _ESCELL_H_

#include <memory>
#include <fstream>
#include <string>

#include "EspReader.h"

class ESCell;

typedef std::shared_ptr<ESCell> ESCellRef;

struct CellData {
    uint32_t flags = 0;
    int32_t gridX = std::numeric_limits<int32_t>::max();
    int32_t gridY = std::numeric_limits<int32_t>::max();
};

class ESCell {
private:
    std::string cellName;
    std::string region;
    uint32_t colour = 0;
    CellData data;
public:
    const std::string& getRegn() { return region; }
    bool isInterior() { return ((data.flags & 0x01) != 0); }
    std::string getCellName() { return cellName; }
    int32_t getCellX() { return data.gridX; }
    int32_t getCellY() { return data.gridY; }
    long getNAM0() { return colour; }

    static ESCell load(EspReader::Record& record);
};

#endif
